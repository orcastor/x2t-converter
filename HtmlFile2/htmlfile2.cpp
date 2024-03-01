#include "htmlfile2.h"

#include <string>
#include <cwctype>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>

#include "../Common/3dParty/html/htmltoxhtml.h"
#include "../Common/3dParty/html/css/src/CCssCalculator.h"
#include "../Common/3dParty/html/css/src/xhtml/CDocumentStyle.h"
#include "../Common/Network/FileTransporter/include/FileTransporter.h"
#include "../DesktopEditor/common/Types.h"
#include "../DesktopEditor/common/Base64.h"
#include "../DesktopEditor/common/SystemUtils.h"
#include "../DesktopEditor/common/StringBuilder.h"
#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Directory.h"
#include "../DesktopEditor/common/Path.h"
#include "../DesktopEditor/common/ProcessEnv.h"
#include "../DesktopEditor/xml/include/xmlutils.h"
#include "../DesktopEditor/raster/BgraFrame.h"
#include "../DesktopEditor/graphics/pro/Graphics.h"
#include "../DesktopEditor/raster/Metafile/svg/CSvgFile.h"

#include "htmlfile2.h"

#include <boost/regex.hpp>

#ifndef VALUE2STR
#define VALUE_TO_STRING(x) #x
#define VALUE2STR(x) VALUE_TO_STRING(x)
#endif

#define MAXCOLUMNSINTABLE 63
#define MAXROWSINTABLE    32767

#define DEFAULT_PAGE_WIDTH  12240 // Значение в Twips
#define DEFAULT_PAGE_HEIGHT 15840 // Значение в Twips

std::wstring rStyle = L" a area b strong bdo bdi big br center cite dfn em i var code kbd samp tt del s font img ins u mark q rt sup small sub svg input basefont button label data object noscript output abbr time ruby progress hgroup meter span acronym ";

//struct CTree
//{
//    NSCSS::CNode m_oNode;
//    std::vector<CTree> m_arrChild;
//};

// Ячейка таблицы
struct CTc
{
	int i;
	int j;
	std::wstring sGridSpan = L"1";
	std::wstring sPr = L"";

	CTc(int _i, int _j, const std::wstring& sColspan, const std::wstring& sTcPr = L"") 
		: i(_i), j(_j), sGridSpan(sColspan), sPr(sTcPr) {}

	bool operator==(const CTc& c2)
	{
		return (i == c2.i && j == c2.j && sGridSpan == c2.sGridSpan);
	}
};

// Настройки текста
struct CTextSettings
{
	bool bBdo; // Реверс текста
	bool bPre; // Сохранение форматирования (Сохранение пробелов, табуляций, переносов строк)
	int  nLi;  // Уровень списка
	std::wstring sRStyle; // w:rStyle
	std::wstring sPStyle; // w:pStyle

	CTextSettings(bool _bBdo, bool _bPre, int _nLi, const std::wstring& _sRStyle, const std::wstring& _sPStyle) :
		bBdo(_bBdo), bPre(_bPre), nLi(_nLi), sRStyle(_sRStyle), sPStyle(_sPStyle) {}

	CTextSettings(const CTextSettings& oTS) :
		bBdo(oTS.bBdo), bPre(oTS.bPre), nLi(oTS.nLi), sRStyle(oTS.sRStyle), sPStyle(oTS.sPStyle) {}
};

//Необходимые стили таблицы
struct TTableStyles
{
	const NSCSS::NSProperties::CIndent* m_pPadding;
	const NSCSS::NSProperties::CEnum*   m_pCollapse;
	
	int  m_nCellSpacing;
	bool m_bHaveBorderAttribute;
	
	TTableStyles()
		: m_pPadding(NULL), m_pCollapse(NULL), m_nCellSpacing(-1), m_bHaveBorderAttribute(false)
	{}
};

void replace_all(std::wstring& s, const std::wstring& s1, const std::wstring& s2)
{
	size_t pos = s.find(s1);
	size_t l = s2.length();
	while (pos != std::string::npos)
	{
		if (!(s1 == L"&" && s2 == L"&amp;" && s.length() > pos + 4 && s[pos] == L'&' && s[pos + 1] == L'a' && s[pos + 2] == L'm' && s[pos + 3] == L'p' && s[pos + 4] == L';'))
			s.replace(pos, s1.length(), s2);
		pos = s.find(s1, pos + l);
	}
}

void ReplaceSpaces(std::wstring& wsValue)
{
	boost::wregex oRegex(L"\\s+");
	wsValue = boost::regex_replace(wsValue, oRegex, L" ");
}

std::wstring EncodeXmlString(const std::wstring& s)
{
	std::wstring sRes = s;

	replace_all(sRes, L"&", L"&amp;");
	replace_all(sRes, L"<", L"&lt;");
	replace_all(sRes, L">", L"&gt;");
	replace_all(sRes, L"\"", L"&quot;");
	replace_all(sRes, L"\'", L"&#39;");
	replace_all(sRes, L"\n", L"&#xA;");
	replace_all(sRes, L"\r", L"&#xD;");
	replace_all(sRes, L"\t", L"&#x9;");

	return sRes;
}

bool GetStatusUsingExternalLocalFiles()
{
	if (NSProcessEnv::IsPresent(NSProcessEnv::Converter::gc_allowPrivateIP))
		return NSProcessEnv::GetBoolValue(NSProcessEnv::Converter::gc_allowPrivateIP);

	return true;
}

bool CanUseThisPath(const std::wstring& wsPath, bool bIsAllowExternalLocalFiles)
{
	if (bIsAllowExternalLocalFiles)
		return true;

	if (wsPath.length() >= 3 && L"../" == wsPath.substr(0, 3))
		return false;

	return true;
}

class CHtmlFile2_Private
{
public:
	XmlUtils::CXmlLiteReader m_oLightReader;   // SAX Reader
	NSCSS::CCssCalculator m_oStylesCalculator; // Css калькулятор
	NSCSS::CDocumentStyle m_oXmlStyle;         // Ooxml стиль

	NSCSS::NSProperties::CPage m_oPageData; // Стили страницы

	std::wstring m_sTmp;  // Temp папка
	std::wstring m_sSrc;  // Директория источника
	std::wstring m_sDst;  // Директория назначения
	std::wstring m_sBase; // Полный базовый адрес

	NSCSS::CTree m_oTree; // Дерево body html-файла

private:
	int m_nFootnoteId;  // ID сноски
	int m_nHyperlinkId; // ID ссылки
	int m_nCrossId;     // ID перекрестной ссылки
	int m_nNumberingId; // ID списка

	NSStringUtils::CStringBuilder m_oStylesXml;   // styles.xml
	NSStringUtils::CStringBuilder m_oDocXmlRels;  // document.xml.rels
	NSStringUtils::CStringBuilder m_oNoteXmlRels; // footnotes.xml.rels
	NSStringUtils::CStringBuilder m_oDocXml;      // document.xml
	NSStringUtils::CStringBuilder m_oNoteXml;     // footnotes.xml
	NSStringUtils::CStringBuilder m_oNumberXml;   // numbering.xml

	bool m_bInP;       // <w:p> открыт?
	bool m_bWasPStyle; // <w:pStyle> записан?
	bool m_bWasSpace;  // Был пробел?

	std::vector<std::wstring>            m_arrImages;  // Картинки
	std::map<std::wstring, std::wstring> m_mFootnotes; // Сноски
public:

	CHtmlFile2_Private() 
		: m_nFootnoteId(1), m_nHyperlinkId(1), m_nCrossId(1), m_nNumberingId(1), 
		  m_bInP(false), m_bWasPStyle(false), m_bWasSpace(false)
	{
		m_oPageData.SetSize  (std::to_wstring(DEFAULT_PAGE_WIDTH) + L"tw " + std::to_wstring(DEFAULT_PAGE_HEIGHT) + L"tw", 0, true);
		m_oPageData.SetMargin(L"720tw 720tw 720tw 720tw", 0, true);
		m_oPageData.SetFooter(L"720tw", 0, true);
		m_oPageData.SetHeader(L"720tw", 0, true);
	}

	~CHtmlFile2_Private()
	{
		m_oLightReader     .Clear();
		m_oStylesCalculator.Clear();
		m_oXmlStyle        .Clear();
		m_oStylesXml       .Clear();
		m_oDocXmlRels      .Clear();
		m_oNoteXmlRels     .Clear();
		m_oDocXml          .Clear();
		m_oNoteXml         .Clear();
		m_oNumberXml       .Clear();
	}

	// Проверяет наличие тэга html
	bool isHtml()
	{
		return (m_oLightReader.ReadNextNode() ? m_oLightReader.GetName() == L"html" : false);
	}

	// Создаёт основу docx
	void CreateDocxEmpty(CHtmlParams* oParams)
	{
		// Создаем пустые папки
		NSDirectory::CreateDirectory(m_sDst + L"/_rels");
		NSDirectory::CreateDirectory(m_sDst + L"/docProps");
		NSDirectory::CreateDirectory(m_sDst + L"/word");
		NSDirectory::CreateDirectory(m_sDst + L"/word/_rels");
		NSDirectory::CreateDirectory(m_sDst + L"/word/media");
		NSDirectory::CreateDirectory(m_sDst + L"/word/theme");

		// theme1.xml
		std::wstring sTheme = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><a:theme xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\" name=\"Office Theme\"><a:themeElements><a:clrScheme name=\"Office\"><a:dk1><a:sysClr val=\"windowText\" lastClr=\"000000\"/></a:dk1><a:lt1><a:sysClr val=\"window\" lastClr=\"FFFFFF\"/></a:lt1><a:dk2><a:srgbClr val=\"44546A\"/></a:dk2><a:lt2><a:srgbClr val=\"E7E6E6\"/></a:lt2><a:accent1><a:srgbClr val=\"5B9BD5\"/></a:accent1><a:accent2><a:srgbClr val=\"ED7D31\"/></a:accent2><a:accent3><a:srgbClr val=\"A5A5A5\"/></a:accent3><a:accent4><a:srgbClr val=\"FFC000\"/></a:accent4><a:accent5><a:srgbClr val=\"4472C4\"/></a:accent5><a:accent6><a:srgbClr val=\"70AD47\"/></a:accent6><a:hlink><a:srgbClr val=\"0563C1\"/></a:hlink><a:folHlink><a:srgbClr val=\"954F72\"/></a:folHlink></a:clrScheme><a:fontScheme name=\"Office Classic 2\"><a:majorFont><a:latin typeface=\"Arial\"/><a:ea typeface=\"Arial\"/><a:cs typeface=\"Arial\"/></a:majorFont><a:minorFont><a:latin typeface=\"Arial\"/><a:ea typeface=\"Arial\"/><a:cs typeface=\"Arial\"/></a:minorFont></a:fontScheme><a:fmtScheme name=\"Office\"><a:fillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"50000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"35000\"><a:schemeClr val=\"phClr\"><a:tint val=\"37000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:tint val=\"15000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"1\"/></a:gradFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:shade val=\"51000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"80000\"><a:schemeClr val=\"phClr\"><a:shade val=\"93000\"/><a:satMod val=\"130000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"94000\"/><a:satMod val=\"135000\"/></a:schemeClr></a:gs></a:gsLst><a:lin ang=\"16200000\" scaled=\"0\"/></a:gradFill></a:fillStyleLst><a:lnStyleLst><a:ln w=\"6350\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"><a:shade val=\"95000\"/><a:satMod val=\"105000\"/></a:schemeClr></a:solidFill></a:ln><a:ln w=\"12700\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill></a:ln><a:ln w=\"19050\" cap=\"flat\" cmpd=\"sng\" algn=\"ctr\"><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill></a:ln></a:lnStyleLst><a:effectStyleLst><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"20000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"38000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle></a:effectStyleLst><a:bgFillStyleLst><a:solidFill><a:schemeClr val=\"phClr\"/></a:solidFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"40000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"40000\"><a:schemeClr val=\"phClr\"><a:tint val=\"45000\"/><a:shade val=\"99000\"/><a:satMod val=\"350000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"20000\"/><a:satMod val=\"255000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\"/></a:gradFill><a:gradFill><a:gsLst><a:gs pos=\"0\"><a:schemeClr val=\"phClr\"><a:tint val=\"80000\"/><a:satMod val=\"300000\"/></a:schemeClr></a:gs><a:gs pos=\"100000\"><a:schemeClr val=\"phClr\"><a:shade val=\"30000\"/><a:satMod val=\"200000\"/></a:schemeClr></a:gs></a:gsLst><a:path path=\"circle\"/></a:gradFill></a:bgFillStyleLst></a:fmtScheme></a:themeElements><a:objectDefaults/></a:theme>";
		NSFile::CFileBinary oThemeWriter;
		if (oThemeWriter.CreateFileW(m_sDst + L"/word/theme/theme1.xml"))
		{
			oThemeWriter.WriteStringUTF8(sTheme);
			oThemeWriter.CloseFile();
		}

		// app.xml
		std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
		if (sApplication.empty())
			sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;
#if defined(INTVER)
		std::string sVersion = VALUE2STR(INTVER);
#endif
		std::wstring sApp = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Properties xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties\" xmlns:vt=\"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes\"><Application>";
		sApp += sApplication + L"/" + UTF8_TO_U(sVersion);
		sApp += L"</Application><DocSecurity>0</DocSecurity><HyperlinksChanged>false</HyperlinksChanged><LinksUpToDate>false</LinksUpToDate><ScaleCrop>false</ScaleCrop><SharedDoc>false</SharedDoc></Properties>";
		NSFile::CFileBinary oAppWriter;
		if (oAppWriter.CreateFileW(m_sDst + L"/docProps/app.xml"))
		{
			oAppWriter.WriteStringUTF8(sApp);
			oAppWriter.CloseFile();
		}

		// .rels
		std::wstring sRels = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"><Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"word/document.xml\"/><Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties\" Target=\"docProps/core.xml\"/><Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties\" Target=\"docProps/app.xml\"/></Relationships>";
		NSFile::CFileBinary oRelsWriter;
		if (oRelsWriter.CreateFileW(m_sDst + L"/_rels/.rels"))
		{
			oRelsWriter.WriteStringUTF8(sRels);
			oRelsWriter.CloseFile();
		}

		// [Content_Types].xml
		std::wstring sContent = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\"><Default Extension=\"bmp\" ContentType=\"image/bmp\"/><Default Extension=\"svg\" ContentType=\"image/svg+xml\"/><Default Extension=\"jfif\" ContentType=\"image/jpeg\"/><Default Extension=\"wmf\" ContentType=\"image/x-wmf\"/><Default Extension=\"gif\" ContentType=\"image/gif\"/><Default Extension=\"jpe\" ContentType=\"image/jpeg\"/><Default Extension=\"png\" ContentType=\"image/png\"/><Default Extension=\"jpg\" ContentType=\"image/jpeg\"/><Default Extension=\"jpeg\" ContentType=\"image/jpeg\"/><Default Extension=\"xml\" ContentType=\"application/xml\"/><Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/><Default Extension=\"bin\" ContentType=\"application/vnd.openxmlformats-officedocument.oleObject\"/><Override PartName=\"/docProps/core.xml\" ContentType=\"application/vnd.openxmlformats-package.core-properties+xml\"/><Override PartName=\"/word/theme/theme1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/><Override PartName=\"/word/fontTable.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml\"/><Override PartName=\"/word/webSettings.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml\"/><Override PartName=\"/word/styles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml\"/><Override PartName=\"/word/document.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml\"/><Override PartName=\"/word/footnotes.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml\"/><Override PartName=\"/word/settings.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml\"/><Override PartName=\"/docProps/app.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.extended-properties+xml\"/><Override PartName=\"/word/numbering.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.numbering+xml\"/></Types>";
		NSFile::CFileBinary oContentWriter;
		if (oContentWriter.CreateFileW(m_sDst + L"/[Content_Types].xml"))
		{
			oContentWriter.WriteStringUTF8(sContent);
			oContentWriter.CloseFile();
		}

		// fontTable.xml
		std::wstring sFontTable = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:fonts xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" mc:Ignorable=\"w14 w15\"><w:font w:name=\"Wingdings\"><w:panose1 w:val=\"05000000000000000000\"/></w:font><w:font w:name=\"Courier New\"><w:panose1 w:val=\"02070309020205020404\"/></w:font><w:font w:name=\"Symbol\"><w:panose1 w:val=\"05050102010706020507\"/></w:font><w:font w:name=\"Arial\"><w:panose1 w:val=\"020B0604020202020204\"/></w:font><w:font w:name=\"Calibri\"><w:panose1 w:val=\"020F0502020204030204\"/></w:font><w:font w:name=\"Times New Roman\"><w:panose1 w:val=\"02020603050405020304\"/></w:font><w:font w:name=\"Cambria\"><w:panose1 w:val=\"02040503050406030204\"/></w:font></w:fonts>";
		NSFile::CFileBinary oFontTableWriter;
		if (oFontTableWriter.CreateFileW(m_sDst + L"/word/fontTable.xml"))
		{
			oFontTableWriter.WriteStringUTF8(sFontTable);
			oFontTableWriter.CloseFile();
		}

		// settings.xml
		std::wstring sSettings = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?><w:settings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:v=\"urn:schemas-microsoft-com:vml\"><w:clrSchemeMapping w:accent1=\"accent1\" w:accent2=\"accent2\" w:accent3=\"accent3\" w:accent4=\"accent4\" w:accent5=\"accent5\" w:accent6=\"accent6\" w:bg1=\"light1\" w:bg2=\"light2\" w:followedHyperlink=\"followedHyperlink\" w:hyperlink=\"hyperlink\" w:t1=\"dark1\" w:t2=\"dark2\"/><w:defaultTabStop w:val=\"708\"/><m:mathPr/><w:trackRevisions w:val=\"false\"/><w:footnotePr><w:footnote w:id=\"-1\"/><w:footnote w:id=\"0\"/><w:numFmt w:val=\"decimal\"/><w:numRestart w:val=\"continuous\"/><w:numStart w:val=\"1\"/><w:pos w:val=\"pageBottom\"/></w:footnotePr><w:decimalSymbol w:val=\".\"/><w:listSeparator w:val=\",\"/><w:compat><w:compatSetting w:name=\"compatibilityMode\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"15\"/><w:compatSetting w:name=\"overrideTableStyleFontSizeAndJustification\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"enableOpenTypeFeatures\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"doNotFlipMirrorIndents\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"1\"/><w:compatSetting w:name=\"useWord2013TrackBottomHyphenation\" w:uri=\"http://schemas.microsoft.com/office/word\" w:val=\"0\"/></w:compat><w:zoom w:percent=\"100\"/><w:characterSpacingControl w:val=\"doNotCompress\"/><w:themeFontLang w:val=\"en-US\" w:eastAsia=\"zh-CN\"/><w:shapeDefaults><o:shapedefaults v:ext=\"edit\" spidmax=\"1026\"/><o:shapelayout v:ext=\"edit\"><o:idmap v:ext=\"edit\" data=\"1\"/></o:shapelayout></w:shapeDefaults></w:settings>";
		NSFile::CFileBinary oSettingsWriter;
		if (oSettingsWriter.CreateFileW(m_sDst + L"/word/settings.xml"))
		{
			oSettingsWriter.WriteStringUTF8(sSettings);
			oSettingsWriter.CloseFile();
		}

		// webSettings.xml
		std::wstring sWebSettings = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:webSettings xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\"><w:optimizeForBrowser/></w:webSettings>";
		NSFile::CFileBinary oWebWriter;
		if (oWebWriter.CreateFileW(m_sDst + L"/word/webSettings.xml"))
		{
			oWebWriter.WriteStringUTF8(sWebSettings);
			oWebWriter.CloseFile();
		}

		// numbering.xml
		// Маркированный список
		m_oNumberXml += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:numbering xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\"><w:abstractNum w:abstractNumId=\"0\"><w:multiLevelType w:val=\"hybridMultilevel\"/><w:lvl w:ilvl=\"0\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"";
		m_oNumberXml.AddCharSafe(183);
		m_oNumberXml += L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"720\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Symbol\" w:hAnsi=\"Symbol\" w:cs=\"Symbol\" w:eastAsia=\"Symbol\"/></w:rPr></w:lvl><w:lvl w:ilvl=\"1\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"o\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"1440\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Courier New\" w:hAnsi=\"Courier New\" w:cs=\"Courier New\" w:eastAsia=\"Courier New\"/></w:rPr></w:lvl><w:lvl w:ilvl=\"2\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"";
		m_oNumberXml.AddCharSafe(167);
		m_oNumberXml += L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"2160\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Wingdings\" w:hAnsi=\"Wingdings\" w:cs=\"Wingdings\" w:eastAsia=\"Wingdings\"/></w:rPr></w:lvl><w:lvl w:ilvl=\"3\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"";
		m_oNumberXml.AddCharSafe(183);
		m_oNumberXml += L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"2880\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Symbol\" w:hAnsi=\"Symbol\" w:cs=\"Symbol\" w:eastAsia=\"Symbol\"/></w:rPr></w:lvl><w:lvl w:ilvl=\"4\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"o\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"3600\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Courier New\" w:hAnsi=\"Courier New\" w:cs=\"Courier New\" w:eastAsia=\"Courier New\"/></w:rPr></w:lvl><w:lvl w:ilvl=\"5\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"";
		m_oNumberXml.AddCharSafe(167);
		m_oNumberXml += L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"4320\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Wingdings\" w:hAnsi=\"Wingdings\" w:cs=\"Wingdings\" w:eastAsia=\"Wingdings\"/></w:rPr></w:lvl><w:lvl w:ilvl=\"6\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"";
		m_oNumberXml.AddCharSafe(183);
		m_oNumberXml += L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5040\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Symbol\" w:hAnsi=\"Symbol\" w:cs=\"Symbol\" w:eastAsia=\"Symbol\"/></w:rPr></w:lvl><w:lvl w:ilvl=\"7\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"o\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5760\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Courier New\" w:hAnsi=\"Courier New\" w:cs=\"Courier New\" w:eastAsia=\"Courier New\"/></w:rPr></w:lvl><w:lvl w:ilvl=\"8\"><w:start w:val=\"1\"/><w:numFmt w:val=\"bullet\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"";
		m_oNumberXml.AddCharSafe(167);
		m_oNumberXml += L"\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"6480\" w:hanging=\"360\"/></w:pPr><w:rPr><w:rFonts w:ascii=\"Wingdings\" w:hAnsi=\"Wingdings\" w:cs=\"Wingdings\" w:eastAsia=\"Wingdings\"/></w:rPr></w:lvl></w:abstractNum>";

		// core.xml
		std::wstring sCore = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><cp:coreProperties xmlns:cp=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:dcmitype=\"http://purl.org/dc/dcmitype/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">";
		if(oParams)
		{
			if(!oParams->m_sBookTitle.empty())
			{
				sCore += L"<dc:title>";
				sCore += EncodeXmlString(oParams->m_sBookTitle);
				sCore += L"</dc:title>";
			}
			if(!oParams->m_sAuthors.empty())
			{
				sCore += L"<dc:creator>";
				sCore += EncodeXmlString(oParams->m_sAuthors);
				sCore += L"</dc:creator>";
			}
			if(!oParams->m_sGenres.empty())
			{
				sCore += L"<dc:subject>";
				sCore += EncodeXmlString(oParams->m_sGenres);
				sCore += L"</dc:subject>";
			}
			if(!oParams->m_sDate.empty())
			{
				sCore += L"<dcterms:created xsi:type=\"dcterms:W3CDTF\">";
				sCore += EncodeXmlString(oParams->m_sDate);
				sCore += L"</dcterms:created>";
			}
			if(!oParams->m_sDescription.empty())
			{
				sCore += L"<dc:description>";
				sCore += EncodeXmlString(oParams->m_sDescription);
				sCore += L"</dc:description>";
			}
		}
		sCore += L"<cp:lastModifiedBy/></cp:coreProperties>";
		NSFile::CFileBinary oCoreWriter;
		if (oCoreWriter.CreateFileW(m_sDst + L"/docProps/core.xml"))
		{
			oCoreWriter.WriteStringUTF8(sCore);
			oCoreWriter.CloseFile();
		}

		// Начала файлов
		m_oDocXmlRels  += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">";
		m_oDocXmlRels  += L"<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>";
		m_oDocXmlRels  += L"<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings\" Target=\"settings.xml\"/>";
		m_oDocXmlRels  += L"<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings\" Target=\"webSettings.xml\"/>";
		m_oDocXmlRels  += L"<Relationship Id=\"rId4\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable\" Target=\"fontTable.xml\"/>";
		m_oDocXmlRels  += L"<Relationship Id=\"rId5\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme1.xml\"/>";
		m_oDocXmlRels  += L"<Relationship Id=\"rId6\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes\" Target=\"footnotes.xml\"/>";
		m_oDocXmlRels  += L"<Relationship Id=\"rId7\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering\" Target=\"numbering.xml\"/>";
		m_oNoteXmlRels += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">";
		m_oDocXml      += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:document xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\"><w:body>";
		m_oNoteXml     += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:footnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" mc:Ignorable=\"w14 w15 wp14\">";
		m_oNoteXml     += L"<w:footnote w:type=\"separator\" w:id=\"-1\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:separator/></w:r></w:p></w:footnote><w:footnote w:type=\"continuationSeparator\" w:id=\"0\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:continuationSeparator/></w:r></w:p></w:footnote>";
		m_oStylesXml   += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:styles xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" mc:Ignorable=\"w14 w15\">";

		// docDefaults по умолчанию
		if(oParams && !oParams->m_sdocDefaults.empty())
			m_oStylesXml += oParams->m_sdocDefaults;
		else
			m_oStylesXml += L"<w:rPrDefault><w:rPr><w:rFonts w:ascii=\"Arial\" w:eastAsia=\"Times New Roman\" w:hAnsi=\"Arial\" w:cs=\"Arial\"/></w:rPr></w:rPrDefault><w:pPrDefault/>";

		// normal по умолчанию
		if(oParams && !oParams->m_sNormal.empty())
			m_oStylesXml += oParams->m_sNormal;
		else
			m_oStylesXml += L"<w:style w:type=\"paragraph\" w:styleId=\"normal\" w:default=\"1\"><w:name w:val=\"Normal\"/><w:qFormat/></w:style>";

		// Маркированный список
		m_oStylesXml += L"<w:style w:type=\"paragraph\" w:styleId=\"li\"><w:name w:val=\"List Paragraph\"/><w:basedOn w:val=\"normal\"/><w:qFormat/><w:uiPriority w:val=\"34\"/><w:pPr><w:contextualSpacing w:val=\"true\"/><w:ind w:left=\"720\"/></w:pPr></w:style>";
		// Ссылки
		m_oStylesXml += L"<w:style w:type=\"character\" w:styleId=\"a\"><w:name w:val=\"Hyperlink\"/><w:uiPriority w:val=\"99\"/><w:unhideWhenUsed/><w:rPr><w:color w:val=\"0000FF\" w:themeColor=\"hyperlink\"/><w:u w:val=\"single\"/></w:rPr></w:style>";
		// Таблицы
//		m_oStylesXml += L"<w:style w:type=\"table\" w:default=\"1\" w:styleId=\"table-based\"><w:name w:val=\"Normal Table\"/><w:uiPriority w:val=\"99\"/><w:semiHidden/><w:unhideWhenUsed/><w:tblPr><w:tblInd w:w=\"0\" w:type=\"dxa\"/><w:tblCellMar><w:top w:w=\"0\" w:type=\"dxa\"/><w:left w:w=\"108\" w:type=\"dxa\"/><w:bottom w:w=\"0\" w:type=\"dxa\"/><w:right w:w=\"108\" w:type=\"dxa\"/></w:tblCellMar></w:tblPr></w:style><w:style w:type=\"table\" w:styleId=\"table\"><w:name w:val=\"Table Grid\"/><w:basedOn w:val=\"table-based\"/><w:uiPriority w:val=\"59\"/><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:tblPr><w:tblBorders><w:top w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:left w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:bottom w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:right w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:insideH w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/><w:insideV w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"000000\"/></w:tblBorders></w:tblPr></w:style>";
		// Сноски
		m_oStylesXml += L"<w:style w:type=\"character\" w:styleId=\"footnote\"><w:name w:val=\"footnote reference\"/><w:uiPriority w:val=\"99\"/><w:unhideWhenUsed/><w:rPr><w:vertAlign w:val=\"superscript\"/></w:rPr></w:style><w:style w:type=\"paragraph\" w:styleId=\"footnote-p\"><w:name w:val=\"footnote text\"/><w:basedOn w:val=\"normal\"/><w:link w:val=\"footnote-c\"/><w:uiPriority w:val=\"99\"/><w:semiHidden/><w:unhideWhenUsed/><w:rPr><w:sz w:val=\"18\"/></w:rPr><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"40\"/></w:pPr></w:style><w:style w:type=\"character\" w:styleId=\"footnote-c\" w:customStyle=\"1\"><w:name w:val=\"footnote text character\"/><w:link w:val=\"footnote-p\"/><w:uiPriority w:val=\"99\"/><w:rPr><w:sz w:val=\"18\"/></w:rPr></w:style>";
	}

	// Читает файл
	void readSrc()
	{
		// Читаем html
		m_oLightReader.ReadNextNode();

		int nDeath = m_oLightReader.GetDepth();
		while(m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			std::wstring sName = m_oLightReader.GetName();
			if(sName == L"head")
				readHead();
			else if(sName == L"body")
				readBody();
		}
	}

	// Дописывает концы docx
	void write()
	{
		m_oDocXmlRels.WriteString(L"</Relationships>");
		NSFile::CFileBinary oRelsWriter;
		if (oRelsWriter.CreateFileW(m_sDst + L"/word/_rels/document.xml.rels"))
		{
			oRelsWriter.WriteStringUTF8(m_oDocXmlRels.GetData());
			oRelsWriter.CloseFile();
		}

		m_oNoteXmlRels.WriteString(L"</Relationships>");
		if (oRelsWriter.CreateFileW(m_sDst + L"/word/_rels/footnotes.xml.rels"))
		{
			oRelsWriter.WriteStringUTF8(m_oNoteXmlRels.GetData());
			oRelsWriter.CloseFile();
		}

		if (m_bInP)
			m_oDocXml.WriteString(L"</w:p>");

		m_oDocXml.WriteString(L"<w:sectPr w:rsidR=\"0007083F\" w:rsidRPr=\"0007083F\" w:rsidSect=\"0007612E\">");
		m_oDocXml.WriteString(L"<w:pgSz w:w=\"" + std::to_wstring(m_oPageData.GetWidth().ToInt(NSCSS::Twips)) + L"\" ");
		m_oDocXml.WriteString(L" w:h=\"" + std::to_wstring(m_oPageData.GetHeight().ToInt(NSCSS::Twips)) + L"\"/>");
		m_oDocXml.WriteString(L"<w:pgMar w:top=\"" + std::to_wstring(m_oPageData.GetMargin().GetTop().ToInt(NSCSS::Twips)) + L"\" ");
		m_oDocXml.WriteString(L"w:right=\"" + std::to_wstring(m_oPageData.GetMargin().GetRight().ToInt(NSCSS::Twips)) + L"\" ");
		m_oDocXml.WriteString(L"w:bottom=\"" + std::to_wstring(m_oPageData.GetMargin().GetBottom().ToInt(NSCSS::Twips)) + L"\" ");
		m_oDocXml.WriteString(L"w:left=\"" + std::to_wstring(m_oPageData.GetMargin().GetLeft().ToInt(NSCSS::Twips)) + L"\" ");
		m_oDocXml.WriteString(L"w:header=\"" + std::to_wstring(m_oPageData.GetHeader().ToInt(NSCSS::Twips)) + L"\" ");
		m_oDocXml.WriteString(L"w:footer=\"" + std::to_wstring(m_oPageData.GetFooter().ToInt(NSCSS::Twips)) + L"\" ");
		m_oDocXml.WriteString(L"w:gutter=\"0\"/><w:cols w:space=\"720\"/><w:docGrid w:linePitch=\"360\"/></w:sectPr></w:body></w:document>");
		
		NSFile::CFileBinary oDocumentWriter;
		if (oDocumentWriter.CreateFileW(m_sDst + L"/word/document.xml"))
		{
			oDocumentWriter.WriteStringUTF8(m_oDocXml.GetData());
			oDocumentWriter.CloseFile();
		}

		m_oNoteXml.WriteString(L"</w:footnotes>");
		NSFile::CFileBinary oFootnotesWriter;
		if (oFootnotesWriter.CreateFileW(m_sDst + L"/word/footnotes.xml"))
		{
			oFootnotesWriter.WriteStringUTF8(m_oNoteXml.GetData());
			oFootnotesWriter.CloseFile();
		}

		// styles.xml
		m_oStylesXml.WriteString(L"</w:styles>");
		NSFile::CFileBinary oStylesWriter;
		if (oStylesWriter.CreateFileW(m_sDst + L"/word/styles.xml"))
		{
			oStylesWriter.WriteStringUTF8(m_oStylesXml.GetData());
			oStylesWriter.CloseFile();
		}

		// numbering.xml
		// Маркированный список
		m_oNumberXml.WriteString(L"<w:num w:numId=\"1\"><w:abstractNumId w:val=\"0\"/></w:num>");
		// Нумерованный список
		for(int i = 1; i < m_nNumberingId; i++)
		{
			m_oNumberXml.WriteString(L"<w:num w:numId=\"");
			m_oNumberXml.WriteString(std::to_wstring(i + 1));
			m_oNumberXml.WriteString(L"\"><w:abstractNumId w:val=\"");
			m_oNumberXml.WriteString(std::to_wstring(i));
			m_oNumberXml.WriteString(L"\"/></w:num>");
		}
		m_oNumberXml.WriteString(L"</w:numbering>");
		NSFile::CFileBinary oNumberingWriter;
		if (oNumberingWriter.CreateFileW(m_sDst + L"/word/numbering.xml"))
		{
			oNumberingWriter.WriteStringUTF8(m_oNumberXml.GetData());
			oNumberingWriter.CloseFile();
		}
	}

	// Конвертирует html в xhtml
	bool htmlXhtml(const std::wstring& sSrc)
	{
		BYTE* pData;
		DWORD nLength;
		if (!NSFile::CFileBinary::ReadAllBytes(sSrc, &pData, nLength))
			return false;

		std::string sFileContent = XmlUtils::GetUtf8FromFileContent(pData, nLength);
		bool bNeedConvert = true;
		if (nLength > 4)
		{
			if (pData[0] == 0xFF && pData[1] == 0xFE && !(pData[2] == 0x00 && pData[3] == 0x00))
				bNeedConvert = false;
			if (pData[0] == 0xFE && pData[1] == 0xFF)
				bNeedConvert = false;

			if (pData[0] == 0xFF && pData[1] == 0xFE && pData[2] == 0x00 && pData[3] == 0x00)
				bNeedConvert = false;
			if (pData[0] == 0 && pData[1] == 0 && pData[2] == 0xFE && pData[3] == 0xFF)
				bNeedConvert = false;
		}
		RELEASEARRAYOBJECTS(pData);

		size_t nFind = sFileContent.find("version=\"");
		if(nFind != std::string::npos)
		{
			nFind += 9;
			size_t nFindEnd = sFileContent.find("\"", nFind);
			if(nFindEnd != std::string::npos)
				sFileContent.replace(nFind, nFindEnd - nFind, "1.0");
		}

		// Так как для htmlToXhtml стили не нужны, а также их содержимое может пагубно повлиять на резьтат, 
		// то поэтому вырезаем их и вставляем уже после
		size_t nFirstStyleFounded = sFileContent.find("<style>");
		size_t nStyleFounded{nFirstStyleFounded};
		std::string sStyles;

		while (std::string::npos != nStyleFounded)
		{
			size_t nStyleEndFounded = sFileContent.find("</style>", nStyleFounded);
			
			if (std::string::npos == nStyleEndFounded)
				break;
			
			sStyles += sFileContent.substr(nStyleFounded + 7, nStyleEndFounded - nStyleFounded - 7) + " ";
			sFileContent.erase(nStyleFounded, nStyleEndFounded - nStyleFounded + 8);

			nStyleFounded = sFileContent.find("<style>", nStyleFounded);
		}

		std::wstring sRes = htmlToXhtml(sFileContent, bNeedConvert);

		if (!sStyles.empty())
		{
			size_t nHeadFounded = sRes.find(L"<head>");
			if (std::wstring::npos != nHeadFounded)
				sRes.insert(nHeadFounded + 6, L"<style>" + std::wstring(sStyles.begin(), sStyles.end()) + L"</style>");
			else
				sRes.insert(nFirstStyleFounded, L"<style>" + std::wstring(sStyles.begin(), sStyles.end()) + L"</style>");
		}
		
//		NSFile::CFileBinary oWriter;
//		if (oWriter.CreateFileW(m_sTmp + L"/res.html"))
//		{
//			oWriter.WriteStringUTF8(sRes);
//			oWriter.CloseFile();
//		}
		
		return m_oLightReader.FromString(sRes);
	}

	// Конвертирует mht в xhtml
	bool mhtXhtml(const std::wstring& sSrc)
	{
		NSFile::CFileBinary file;
		if (!file.OpenFile(sSrc))
			return false;

		unsigned char* buffer = new unsigned char[4096];
		if (!buffer)
		{
			file.CloseFile();
			return false;
		}

		DWORD dwReadBytes = 0;
		file.ReadFile(buffer, 4096, dwReadBytes);
		file.CloseFile();
		std::string xml_string = XmlUtils::GetUtf8FromFileContent(buffer, dwReadBytes);

		const std::string sContentType = NSStringFinder::FindPropety<std::string>(xml_string, "content-type", ":", ";");
		bool bRes = false;

		if(NSStringFinder::Equals(sContentType, "multipart/related"))
		{
			BYTE* pData;
			DWORD nLength;
			if (!NSFile::CFileBinary::ReadAllBytes(sSrc, &pData, nLength))
				return false;

			std::string sFileContent = XmlUtils::GetUtf8FromFileContent(pData, nLength);
			RELEASEARRAYOBJECTS(pData);
			std::wstring sRes = mhtToXhtml(sFileContent);
			/*
			NSFile::CFileBinary oWriter;
			if (oWriter.CreateFileW(m_sTmp + L"/res.html"))
			{
				oWriter.WriteStringUTF8(sRes);
				oWriter.CloseFile();
			}
			*/
			bRes = m_oLightReader.FromString(sRes);
		}
		else
			bRes = htmlXhtml(sSrc);

		RELEASEARRAYOBJECTS(buffer);
		return bRes;
	}

	// Читает стили
	void readStyle()
	{
		if(m_oLightReader.IsEmptyNode())
			return;

		int nDeath = m_oLightReader.GetDepth();
		while(m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			std::wstring sName = m_oLightReader.GetName();

			if(sName == L"body")
				readStyle2(m_oTree);
			else
			{
				// Стиль по ссылке
				if(sName == L"link")
				{
					while(m_oLightReader.MoveToNextAttribute())
					{
						if(m_oLightReader.GetName() != L"href")
							continue;
						std::wstring sRef = m_oLightReader.GetText();
						if(NSFile::GetFileExtention(sRef) != L"css")
							continue;
						std::wstring sFName = NSFile::GetFileName(sRef);
						// Стиль в сети
						if(sRef.substr(0, 4) == L"http")
						{
							sFName = m_sTmp + L'/' + sFName;
							NSNetwork::NSFileTransport::CFileDownloader oDownloadStyle(sRef, false);
							oDownloadStyle.SetFilePath(sFName);
							if(oDownloadStyle.DownloadSync())
							{
								m_oStylesCalculator.AddStylesFromFile(sFName);
								NSFile::CFileBinary::Remove(sFName);
							}
						}
						else
						{
							m_oStylesCalculator.AddStylesFromFile(m_sSrc + L'/' + sFName);
							m_oStylesCalculator.AddStylesFromFile(m_sSrc + L'/' + sRef);
						}
					}
					m_oLightReader.MoveToElement();
				}
				// тэг style содержит стили для styles.xml
				else if(sName == L"style")
					m_oStylesCalculator.AddStyles(m_oLightReader.GetText2());
				else
					readStyle();
			}
		}
	}

	void readStyle2(NSCSS::CTree& oTree)
	{
		std::wstring sName = m_oLightReader.GetName();
		// Стиль по ссылке
		if(sName == L"link")
		{
			while(m_oLightReader.MoveToNextAttribute())
			{
				if(m_oLightReader.GetName() != L"href")
					continue;
				std::wstring sRef = m_oLightReader.GetText();
				if(NSFile::GetFileExtention(sRef) != L"css")
					continue;
				std::wstring sFName = NSFile::GetFileName(sRef);
				// Стиль в сети
				if(sRef.substr(0, 4) == L"http")
				{
					sFName = m_sTmp + L'/' + sFName;
					NSNetwork::NSFileTransport::CFileDownloader oDownloadStyle(sRef, false);
					oDownloadStyle.SetFilePath(sFName);
					if(oDownloadStyle.DownloadSync())
					{
						m_oStylesCalculator.AddStylesFromFile(sFName);
						NSFile::CFileBinary::Remove(sFName);
					}
				}
				else
				{
					m_oStylesCalculator.AddStylesFromFile(m_sSrc + L'/' + sFName);
					m_oStylesCalculator.AddStylesFromFile(m_sSrc + L'/' + sRef);
				}
			}
			m_oLightReader.MoveToElement();
		}
		// тэг style содержит стили для styles.xml
		else if(sName == L"style")
			m_oStylesCalculator.AddStyles(m_oLightReader.GetText2());

		oTree.m_oNode.m_wsName = sName;
		// Стиль по атрибуту
		while(m_oLightReader.MoveToNextAttribute())
		{
			std::wstring sNameA  = m_oLightReader.GetName();
			if(sNameA == L"class")
				oTree.m_oNode.m_wsClass  = m_oLightReader.GetText();
			else if(sNameA == L"id")
				oTree.m_oNode.m_wsId = m_oLightReader.GetText();
			else if(sNameA == L"style")
				oTree.m_oNode.m_wsStyle += m_oLightReader.GetText();
			else
				oTree.m_oNode.m_mAttributes[sNameA] = m_oLightReader.GetText();
		}
		m_oLightReader.MoveToElement();

		int nDeath = m_oLightReader.GetDepth();
		while(m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			if(!m_oLightReader.IsEmptyNode())
			{
				NSCSS::CTree oChildTree;
				readStyle2(oChildTree);
				oTree.m_arrChild.push_back(oChildTree);
			}
		}
	}

	void PageBreakBefore()
	{
		m_oDocXml.WriteString(L"<w:p><w:pPr><w:pageBreakBefore/></w:pPr></w:p>");
	}

private:
	bool NodeBelongToTable(const std::wstring& wsNodeName)
	{
		return L"table" == wsNodeName || L"tbody" == wsNodeName || L"th" == wsNodeName || L"td" == wsNodeName ||
		       L"tr" == wsNodeName    || L"thead" == wsNodeName || L"tfoot" == wsNodeName;
	}

	// Так как CSS калькулятор не знает для какой ноды производится расчет стиля
	// и не знает, что некоторые стили предназначены только определенной ноде,
	// то проще пока обрабатывать это заранее
	// ! Используется для стилей, заданных через аргументы !
	bool CheckArgumentMath(const std::wstring& wsNodeName, const std::wstring& wsStyleName)
	{
		if (L"border" == wsStyleName && L"table" != wsNodeName)
			return false;

		return true;
	}

	void CloseP(NSStringUtils::CStringBuilder* pXml, const std::vector<NSCSS::CNode>& arSelectors)
	{
		m_bWasSpace = false;

		if (!m_bInP)
			return;
	
		for (const NSCSS::CNode& item : arSelectors)
			if (item.m_wsName == L"a")
				pXml->WriteString(L"</w:hyperlink>");
	
		pXml->WriteString(L"</w:p>");
		m_bInP      = false;
	}
	
	std::wstring GetSubClass(NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors)
	{
		NSCSS::CNode oNode;
		std::wstring sNote;
		oNode.m_wsName = m_oLightReader.GetName();
		// Стиль по атрибуту
		while(m_oLightReader.MoveToNextAttribute())
		{
			std::wstring sName  = m_oLightReader.GetName();
			if(sName == L"class")
				oNode.m_wsClass  = m_oLightReader.GetText();
			else if(sName == L"id")
			{
				oNode.m_wsId = m_oLightReader.GetText();
				std::wstring sCrossId = std::to_wstring(m_nCrossId++);
				oXml->WriteString(L"<w:bookmarkStart w:id=\"");
				oXml->WriteString(sCrossId);
				oXml->WriteString(L"\" w:name=\"");
				oXml->WriteEncodeXmlString(oNode.m_wsId);
				oXml->WriteString(L"\"/><w:bookmarkEnd w:id=\"");
				oXml->WriteString(sCrossId);
				oXml->WriteString(L"\"/>");
			}
			else if(sName == L"style")
				oNode.m_wsStyle += m_oLightReader.GetText();
			else if(sName == L"title")
				sNote           = m_oLightReader.GetText();
			else
			{
				if (CheckArgumentMath(oNode.m_wsName, sName))
					oNode.m_mAttributes[sName] = m_oLightReader.GetText();
			}
		}
		m_oLightReader.MoveToElement();
		sSelectors.push_back(oNode);
		return sNote;
	}

	std::wstring GetStyle(const NSCSS::CCompiledStyle& oStyle, bool bP)
	{
//        NSCSS::CCompiledStyle oStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors);
		bP ? m_oXmlStyle.WritePStyle(oStyle) : m_oXmlStyle.WriteRStyle(oStyle);
		m_oStylesXml.WriteString(m_oXmlStyle.GetStyle());
		return m_oXmlStyle.GetIdAndClear();
	}

	void readHead()
	{
		if(m_oLightReader.IsEmptyNode())
			return;
		int nDeath = m_oLightReader.GetDepth();
		while (m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			// Базовый адрес
			if (m_oLightReader.GetName() == L"base")
			{
				while (m_oLightReader.MoveToNextAttribute())
					if (m_oLightReader.GetName() == L"href")
						m_sBase = m_oLightReader.GetText();
				m_oLightReader.MoveToElement();
			}
		}
	}

	void readBody()
	{
		std::vector<NSCSS::CNode> sSelectors;
		
		sSelectors.push_back(NSCSS::CNode(L"html", L"", L""));
		
		GetSubClass(&m_oDocXml, sSelectors);
		/*
		std::wstring sCrossId = std::to_wstring(m_nCrossId++);
		m_oDocXml.WriteString(L"<w:bookmarkStart w:id=\"");
		m_oDocXml.WriteString(sCrossId);
		m_oDocXml.WriteString(L"\" w:name=\"");
		m_oDocXml.WriteString(sFileName);
		m_oDocXml.WriteString(L"\"/><w:bookmarkEnd w:id=\"");
		m_oDocXml.WriteString(sCrossId);
		m_oDocXml.WriteString(L"\"/>");
		*/

		readStream(&m_oDocXml, sSelectors, { false, false, -1, L"", L"" });
	}

	void readInside (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS, const std::wstring& sName)
	{
		if(sName == L"#text")
		{
			std::wstring sText = m_oLightReader.GetText();

			size_t find = sText.find_first_not_of(L" \n\t\r");
			if (find == std::wstring::npos)
				return;

			if (m_bInP && !iswspace(sText.front()) && !m_bWasSpace)
				oXml->WriteString(L"<w:r><w:rPr><w:rFonts w:eastAsia=\"Times New Roman\"/></w:rPr><w:t xml:space=\"preserve\"> </w:t></w:r>");

			std::wstring sPStyle = wrP(oXml, sSelectors, oTS);
			oXml->WriteString(L"<w:r>");
			std::wstring sRStyle = wrR(oXml, sSelectors, oTS);
			oXml->WriteString(L"<w:t xml:space=\"preserve\">");

			if(oTS.bBdo)
				std::reverse(sText.begin(), sText.end());

			if(oTS.bPre)
			{
				size_t nAfter = sText.find_first_of(L"\n\r");
				while(nAfter != std::wstring::npos)
				{
					oXml->WriteEncodeXmlString(sText.c_str(), nAfter);
					oXml->WriteString(L"</w:t></w:r></w:p><w:p>");
					if(!sPStyle.empty())
					{
						oXml->WriteString(L"<w:pPr><w:pStyle w:val=\"");
						oXml->WriteString(sPStyle);
						oXml->WriteString(L"\"/>");
						oXml->WriteString(oTS.sPStyle);
						oXml->WriteString(L"</w:pPr>");
					}
					oXml->WriteString(L"<w:r><w:rPr><w:rStyle w:val=\"");
					oXml->WriteString(sRStyle);
					oXml->WriteString(L"\"/>");
					oXml->WriteString(oTS.sRStyle);
					oXml->WriteString(L"</w:rPr><w:t xml:space=\"preserve\">");
					sText.erase(0, nAfter + 1);
					nAfter = sText.find_first_of(L"\n\r");
				}
			}
			else
				ReplaceSpaces(sText);

			if (1 == sText.length() && std::iswspace(sText.front()))
				sText = L"\u00A0";

			oXml->WriteEncodeXmlString(sText);
			oXml->WriteString(L"</w:t></w:r>");

			if (!sText.empty())
				m_bWasSpace = std::iswspace(sText.back());

			return;
		}

		std::wstring sNote = GetSubClass(oXml, sSelectors);

		// Ссылка
		// Область ссылки
		if(sName == L"a" || sName == L"area")
			readA(oXml, sSelectors, oTS, sNote);
		// Полужирный текст
		// Акцентированный текст
		else if(sName == L"b" || sName == L"strong")
		{
			CTextSettings oTSR(oTS);
			oTSR.sRStyle += L"<w:b/>";
			readStream(oXml, sSelectors, oTSR);
		}
		// Направление текста
		else if(sName == L"bdo")
		{
			std::wstring sDir;
			while(m_oLightReader.MoveToNextAttribute())
				if(m_oLightReader.GetName() == L"dir")
					sDir = m_oLightReader.GetText();
			m_oLightReader.MoveToElement();

			CTextSettings oTSBdo(oTS);
			oTSBdo.bBdo = (sDir == L"rtl");
			readStream(oXml, sSelectors, oTSBdo);
		}
		// Отмена направления текста
		else if(sName == L"bdi")
		{
			CTextSettings oTSBdo(oTS);
			oTSBdo.bBdo = false;
			readStream(oXml, sSelectors, oTSBdo);
		}
		// Увеличивает размер шрифта
		else if(sName == L"big")
		{
			CTextSettings oTSR(oTS);
			oTSR.sRStyle += L"<w:sz w:val=\"26\"/>";
			readStream(oXml, sSelectors, oTSR);
		}
		// Перенос строки
		else if(sName == L"br")
		{
			wrP(oXml, sSelectors, oTS);
			oXml->WriteString(L"<w:r>");
			NSCSS::CCompiledStyle oStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors);
			if(oStyle.m_oText.GetAlign() == L"both")
				oXml->WriteString(L"<w:tab/>");
			oXml->WriteString(L"<w:br/></w:r>");
			m_bWasSpace = false;
		}
		else if(sName == L"center")
		{
			CTextSettings oTSP(oTS);
			oTSP.sPStyle += L"<w:jc w:val=\"center\"/>";
			readStream(oXml, sSelectors, oTSP);
		}
		// Цитата, обычно выделяется курсивом
		// Новый термин, обычно выделяется курсивом
		// Акцентированный текст
		// Курсивный текст
		// Переменная, обычно выделяется курсивом
		else if(sName == L"cite" || sName == L"dfn" || sName == L"em" || sName == L"i" || sName == L"var")
		{
			CTextSettings oTSR(oTS);
			oTSR.sRStyle += L"<w:i/>";
			readStream(oXml, sSelectors, oTSR);
		}
		// Код
		// Моноширинный шрифт, например, Consolas
		// Результат скрипта
		else if(sName == L"code" || sName == L"kbd" || sName == L"samp" || sName == L"tt")
		{
			CTextSettings oTSR(oTS);
			oTSR.sRStyle += L"<w:rFonts w:ascii=\"Consolas\" w:hAnsi=\"Consolas\"/>";
			readStream(oXml, sSelectors, oTSR);
		}
		// Зачеркнутый текст
		else if(sName == L"del" || sName == L"s")
		{
			CTextSettings oTSR(oTS);
			oTSR.sRStyle += L"<w:strike/>";
			readStream(oXml, sSelectors, oTSR);
		}
		else if(sName == L"font")
		{
			while(m_oLightReader.MoveToNextAttribute())
			{
				std::wstring sAName = m_oLightReader.GetName();
				if(sAName == L"color")
					sSelectors.back().m_wsStyle += L"; color: " + m_oLightReader.GetText();
				else if(sAName == L"face")
					sSelectors.back().m_wsStyle += L"; font-family: " + m_oLightReader.GetText();
				else if(sAName == L"size")
				{
					int nSize = 3;
					const std::wstring sSize = m_oLightReader.GetText();
					if(!sSize.empty())
					{
						if(sSize.front() == L'+')
							nSize += NSStringFinder::ToInt(sSize.substr(1));
						else if(sSize.front() == L'-')
							nSize -= NSStringFinder::ToInt(sSize.substr(1));
						else
							nSize = NSStringFinder::ToInt(sSize);
					}

					switch (nSize)
					{
						case 1:  nSize = 10; break;
						case 2:  nSize = 12; break;
						case 3: 
						default: nSize = 14; break;
						case 4:  nSize = 18; break;
						case 5:  nSize = 24; break;
						case 6:  nSize = 32; break;
						case 7:  nSize = 48; break;
					}
					
					sSelectors.back().m_wsStyle += L"; font-size: " + std::to_wstring(nSize) + L"px";
				}
			}
			m_oLightReader.MoveToElement();
			readStream(oXml, sSelectors, oTS);
		}
		// Картинки
		else if(sName == L"img")
			readImage(oXml, sSelectors, oTS);
		// Подчеркнутый
		else if(sName == L"ins" || sName == L"u")
		{
			CTextSettings oTSR(oTS);
			oTSR.sRStyle += L"<w:u w:val=\"single\"/>";
			readStream(oXml, sSelectors, oTSR);
		}
		// Выделенный текст, обычно выделяется желтым
		else if(sName == L"mark")
		{
			CTextSettings oTSR(oTS);
			oTSR.sRStyle += L"<w:highlight w:val=\"yellow\"/>";
			readStream(oXml, sSelectors, oTSR);
		}
		// Цитата, выделенная кавычками, обычно выделяется курсивом
		else if(sName == L"q")
		{
			wrP(oXml, sSelectors, oTS);
			oXml->WriteString(L"<w:r>");
			std::wstring sRStyle = wrR(oXml, sSelectors, oTS);
			oXml->WriteString(L"<w:t xml:space=\"preserve\">&quot;</w:t></w:r>");

			CTextSettings oTSR(oTS);
			oTSR.sRStyle += L"<w:i/>";
			readStream(oXml, sSelectors, oTSR);

			wrP(oXml, sSelectors, oTS);
			oXml->WriteString(L"<w:r><w:rPr><w:rStyle w:val=\"");
			oXml->WriteString(sRStyle);
			oXml->WriteString(L"\"/>");
			oXml->WriteString(oTS.sRStyle);
			oXml->WriteString(L"</w:rPr><w:t xml:space=\"preserve\">&quot;</w:t></w:r>");
		}
		// Текст верхнего регистра
		else if(sName == L"rt" || sName == L"sup")
		{
			CTextSettings oTSR(oTS);
			oTSR.sRStyle += L"<w:vertAlign w:val=\"superscript\"/>";
			readStream(oXml, sSelectors, oTSR);
		}
		// Уменьшает размер шрифта
		else if(sName == L"small")
		{
			CTextSettings oTSR(oTS);
			oTSR.sRStyle += L"<w:sz w:val=\"18\"/>";
			readStream(oXml, sSelectors, oTSR);
		}
		// Текст нижнего регистра
		else if(sName == L"sub")
		{
			CTextSettings oTSR(oTS);
			oTSR.sRStyle += L"<w:vertAlign w:val=\"subscript\"/>";
			readStream(oXml, sSelectors, oTSR);
		}
		// Векторная картинка
		else if(sName == L"svg" || (sName.length() > 3 && sName.compare(sName.length() - 3, 3, L"svg") == 0))
		{
			wrP(oXml, sSelectors, oTS);
			if (readSVG(m_oLightReader.GetOuterXml()))
				ImageRels(oXml, -1, L"", L"png");
		}
		else if(sName == L"input")
			readInput(oXml, sSelectors, oTS);
		// Игнорируются тэги выполняющие скрипт
		else if(sName == L"template" || sName == L"canvas" || sName == L"video" || sName == L"math" || sName == L"rp"    ||
				sName == L"command"  || sName == L"iframe" || sName == L"embed" || sName == L"wbr"  || sName == L"audio" ||
				sName == L"bgsound"  || sName == L"applet" || sName == L"blink" || sName == L"keygen"|| sName == L"script" ||
				sName == L"comment"  || sName == L"title"  || sName == L"style")
		{
			sSelectors.pop_back();
			return;
		}
		else if (sName == L"span")
		{
			if (sSelectors.back().m_wsClass == L"MsoFootnoteReference")
			{
				sSelectors.pop_back();
				return;
			}
			readStream(oXml, sSelectors, oTS);
		}
		// Без нового абзаца
		else if(sName == L"basefont" || sName == L"button" || sName == L"label" || sName == L"data" || sName == L"object" ||
				sName == L"noscript" || sName == L"output" || sName == L"abbr"  || sName == L"time" || sName == L"ruby"   ||
				sName == L"progress" || sName == L"hgroup" || sName == L"meter" || sName == L"acronym")
			readStream(oXml, sSelectors, oTS);
		// С нового абзаца
		else
		{
			CloseP(oXml, sSelectors);

			// Адрес
			if(sName == L"address")
			{
				CTextSettings oTSR(oTS);
				oTSR.sRStyle += L"<w:i/>";
				readStream(oXml, sSelectors, oTSR);
			}
			// Определение термина, отступ от левого края
			else if(sName == L"dd")
			{
				CTextSettings oTSP(oTS);
				oTSP.sPStyle += L"<w:ind w:left=\"567\"/>";
				readStream(oXml, sSelectors, oTSP);
			}
			// aside возможно использовать для сносок в epub
			else if (sName == L"aside" || sName == L"div")
			{
				m_oStylesCalculator.CalculatePageStyle(m_oPageData, sSelectors);

				int bMsoFootnote = 0;
				std::wstring sFootnoteID;
				while (m_oLightReader.MoveToNextAttribute())
				{
					std::wstring sAName = m_oLightReader.GetName();
					std::wstring sAText = m_oLightReader.GetText();
					if (sAName == L"epub:type" && sAText == L"footnote")
						bMsoFootnote++;
					else if (sAName == L"style" && sAText == L"mso-element:footnote")
						bMsoFootnote++;
					else if (sAName == L"id")
					{
						std::map<std::wstring, std::wstring>::iterator it = m_mFootnotes.find(sAText);
						if (it != m_mFootnotes.end())
						{
							bMsoFootnote++;
							sFootnoteID = it->second;
						}
					}
				}
				m_oLightReader.MoveToElement();
				if (bMsoFootnote >= 2)
				{
					m_oNoteXml.WriteString(L"<w:footnote w:id=\"");
					m_oNoteXml.WriteString(sFootnoteID);
					m_oNoteXml.WriteString(L"\">");
					readStream(&m_oNoteXml, sSelectors, oTS);
					m_oNoteXml.WriteString(L"</w:footnote>");
				}
				else
					readStream(oXml, sSelectors, oTS);
			}
			// С нового абзаца
			else if(sName == L"article" || sName == L"header" || sName == L"blockquote" || sName == L"main" || sName == L"dir" ||
					sName == L"summary" || sName == L"footer" || sName == L"nav" || sName == L"figcaption" || sName == L"form" ||
					sName == L"details" || sName == L"option" || sName == L"dt"  || sName == L"p"    ||
					sName == L"section" || sName == L"figure" || sName == L"dl"  || sName == L"legend"     || sName == L"map"  ||
					sName == L"h1" || sName == L"h2" || sName == L"h3" || sName == L"h4" || sName == L"h5" || sName == L"h6")
				readStream(oXml, sSelectors, oTS);
			// Горизонтальная линия
			else if(sName == L"hr")
			{
				bool bPrint = true;
				for (const NSCSS::CNode& item : sSelectors)
				{
					if (item.m_wsName == L"div" && item.m_wsStyle == L"mso-element:footnote-list")
					{
						bPrint = false;
						break;
					}
				}
				if (bPrint)
					oXml->WriteString(L"<w:p><w:pPr><w:pBdr><w:bottom w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/></w:pBdr></w:pPr></w:p>");
			}
			// Меню
			// Маркированный список
			else if(sName == L"menu" || sName == L"ul" || sName == L"select" || sName == L"datalist")
				readLi(oXml, sSelectors, oTS, true);
			// Нумерованный список
			else if(sName == L"ol")
				readLi(oXml, sSelectors, oTS, false);
			// Предварительно форматированный текст
			else if(sName == L"pre" || sName == L"xmp")
			{
				CTextSettings oTSPre(oTS);
				sSelectors.back().m_wsStyle += L"; font-family:Consolas";
				oTSPre.bPre = true;
				readStream(oXml, sSelectors, oTSPre);
			}
			// Таблицы
			else if(sName == L"table")
				readTable(oXml, sSelectors, oTS);
			// Текст с границами
			else if(sName == L"textarea" || sName == L"fieldset")
			{
				CTextSettings oTSP(oTS);
				oTSP.sPStyle += L"<w:pBdr><w:left w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:top w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:right w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:bottom w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/></w:pBdr>";
				readStream(oXml, sSelectors, oTSP);
			}
			else if (sName == L"xml")
			{
				sSelectors.pop_back();
				return;
			}
			// Неизвестный тэг. Выделять ли его абзацем?
			else
				readStream(oXml, sSelectors, oTS);

			readNote(oXml, sSelectors, sNote);
			sNote = L"";

			CloseP(oXml, sSelectors);
		}
		readNote(oXml, sSelectors, sNote);
		sSelectors.pop_back();
	}

	bool readStream (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
	{
		int nDeath = m_oLightReader.GetDepth();
		if(m_oLightReader.IsEmptyNode() || !m_oLightReader.ReadNextSiblingNode2(nDeath))
			return false;

		do
		{
			readInside(oXml, sSelectors, oTS, m_oLightReader.GetName());
		} while(m_oLightReader.ReadNextSiblingNode2(nDeath));
		return true;
	}

	std::wstring CreateBorders(const NSCSS::NSProperties::CBorder& oBorder)
	{
		if (oBorder.EqualSides())
		{
			const std::wstring wsBorderStyle = NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetLeftBorder());

			return L"<w:top "    + wsBorderStyle + L"/>" +
			       L"<w:left "   + wsBorderStyle + L"/>" +
			       L"<w:bottom " + wsBorderStyle + L"/>" +
			       L"<w:right "  + wsBorderStyle + L"/>";
		}
		else
		{
			std::wstring wsTable;

			if (!oBorder.GetTopBorder().Empty())
				wsTable += L"<w:top "    + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetTopBorder())    + L"/>";

			if (!oBorder.GetLeftBorder().Empty())
				wsTable += L"<w:left "   + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetLeftBorder())   + L"/>";

			if (!oBorder.GetBottomBorder().Empty())
				wsTable += L"<w:bottom " + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetBottomBorder()) + L"/>";

			if (!oBorder.GetRightBorder().Empty())
				wsTable += L"<w:right "  + NSCSS::CDocumentStyle::CalculateBorderStyle(oBorder.GetRightBorder())  + L"/>";

			return wsTable;
		}

		return L"";
	}

	void readTr     (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS, const TTableStyles& oTableStyles)
	{
		const std::wstring wsName = m_oLightReader.GetName();

		std::vector<CTc> mTable;
		int nDeath = m_oLightReader.GetDepth();
		int i = 1; // Строка

		UINT unMaxColumns = 0;

		bool bTableHasBorderAttribute = false;

		for (std::vector<NSCSS::CNode>::const_reverse_iterator oIter = sSelectors.crbegin(); oIter < sSelectors.crend(); ++oIter)
		{
			if (L"table" != oIter->m_wsName)
				continue;

			if (oIter->m_mAttributes.end() != oIter->m_mAttributes.find(L"border"))
			{
				bTableHasBorderAttribute = true;
				break;
			}
		}

		while(m_oLightReader.ReadNextSiblingNode(nDeath) && i < MAXROWSINTABLE)
		{
			// tr - строки в таблице
			if(m_oLightReader.GetName() != L"tr")
				continue;
			int nTrDeath = m_oLightReader.GetDepth();
			if(m_oLightReader.IsEmptyNode() || !m_oLightReader.ReadNextSiblingNode(nTrDeath))
				continue;

			int j = 1; // Столбец
			oXml->WriteString(L"<w:tr>");

			std::wstring wsTrPr;

			if (L"thead" == wsName)
				wsTrPr += L"<w:tblHeader/>";

			if (0 <= oTableStyles.m_nCellSpacing)
				wsTrPr += L"<w:tblCellSpacing w:w=\"" + std::to_wstring(oTableStyles.m_nCellSpacing) + L"\" w:type=\"dxa\"/>";
			else if (!bTableHasBorderAttribute && NULL != oTableStyles.m_pCollapse && *oTableStyles.m_pCollapse == NSCSS::NSProperties::BorderCollapse::Separate)
				wsTrPr += L"<w:tblCellSpacing w:w=\"15\" w:type=\"dxa\"/>";

			if (!wsTrPr.empty())
				oXml->WriteString(L"<w:trPr>" + wsTrPr + L"</w:trPr>");

			do
			{
				int nColspan = 1;
				int nRowspan = 1;
				while(m_oLightReader.MoveToNextAttribute())
				{
					if(m_oLightReader.GetName() == L"colspan")
						nColspan = std::min((MAXCOLUMNSINTABLE - j), NSStringFinder::ToInt(m_oLightReader.GetText(), 1));
					else if(m_oLightReader.GetName() == L"rowspan")
						nRowspan = std::min((MAXROWSINTABLE - i), NSStringFinder::ToInt(m_oLightReader.GetText(), 1));
				}
				m_oLightReader.MoveToElement();

				// Вставляем ячейки до
				std::vector<CTc>::iterator it1 = std::find_if(mTable.begin(), mTable.end(), [i, j](const CTc& item){ return item.i == i && item.j == j; });
				std::vector<CTc>::iterator it2 = std::find_if(mTable.begin(), mTable.end(), [j]   (const CTc& item){ return item.i == 0 && item.j == j; });
				while(it1 != mTable.end() || it2 != mTable.end())
				{
					oXml->WriteString(L"<w:tc><w:tcPr>");
					oXml->WriteString(it1->sPr);
					oXml->WriteString(L"<w:vMerge w:val=\"continue\"/><w:gridSpan w:val=\"");
					std::wstring sCol = (it1 != mTable.end() ? it1->sGridSpan : it2->sGridSpan);
					oXml->WriteString(sCol);
					oXml->WriteString(L"\"/><w:noWrap w:val=\"false\"/><w:textDirection w:val=\"lrTb\"/></w:tcPr><w:p></w:p></w:tc>");
					j += stoi(sCol);
					it1 = std::find_if(mTable.begin(), mTable.end(), [i, j](const CTc& item){ return item.i == i && item.j == j; });
					it2 = std::find_if(mTable.begin(), mTable.end(), [j]   (const CTc& item){ return item.i == 0 && item.j == j; });
				}

				GetSubClass(oXml, sSelectors);
				oXml->WriteString(L"<w:tc><w:tcPr>");

				std::vector<NSCSS::CNode> arNewSelectors;

				for (std::vector<NSCSS::CNode>::const_iterator oIter = sSelectors.cbegin(); oIter < sSelectors.cend(); ++oIter)
				{
					if (L"table" == oIter->m_wsName)
					{
						arNewSelectors.insert(arNewSelectors.end(), oIter, sSelectors.cend());
						break;
					}
				}

				NSCSS::CCompiledStyle oStyleSetting = m_oStylesCalculator.GetCompiledStyle(arNewSelectors, true);
				NSCSS::CCompiledStyle oStyle = m_oStylesCalculator.GetCompiledStyle(arNewSelectors, false);

				NSCSS::CCompiledStyle::StyleEquation(oStyle, oStyleSetting);

				std::wstring wsTcPr;

				if (!oStyle.m_oDisplay.GetWidth().Empty())
				{
					if (NSCSS::UnitMeasure::Percent == oStyle.m_oDisplay.GetWidth().GetUnitMeasure())
						wsTcPr += L"<w:tcW w:w=\"" + std::to_wstring(oStyle.m_oDisplay.GetWidth().ToInt(NSCSS::UnitMeasure::Percent, 5000)) + L"\" w:type=\"pct\"/>";
					else
						wsTcPr += L"<w:tcW w:w=\"" + std::to_wstring(oStyle.m_oDisplay.GetWidth().ToInt(NSCSS::UnitMeasure::Twips)) + L"\" w:type=\"dxa\"/>";
				}
				else
					wsTcPr += L"<w:tcW w:w=\"0\" w:type=\"auto\"/>";

				if(nColspan != 1)
					wsTcPr += L"<w:gridSpan w:val=\"" + std::to_wstring(nColspan) + L"\"/>";

				if (!oStyle.m_oBorder.Empty())
					wsTcPr += L"<w:tcBorders>" + CreateBorders(oStyle.m_oBorder) + L"</w:tcBorders>";
				else if (bTableHasBorderAttribute)
					wsTcPr += L"<w:tcBorders><w:top w:val=\"outset\" w:sz=\"6\" w:space=\"0\" w:color=\"auto\"/><w:left w:val=\"outset\" w:sz=\"6\" w:space=\"0\" w:color=\"auto\"/><w:bottom w:val=\"outset\" w:sz=\"6\" w:space=\"0\" w:color=\"auto\"/><w:right w:val=\"outset\" w:sz=\"6\" w:space=\"0\" w:color=\"auto\"/></w:tcBorders>";

				if (!oStyle.m_oBackground.Empty() && !oStyle.m_oBackground.GetColor().Empty())
					wsTcPr += L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" + oStyle.m_oBackground.GetColor().ToWString() + L"\"/>";

				std::wstring wsVerticalAlign = oStyle.m_oDisplay.GetVAlign().ToWString();

				if (wsVerticalAlign.empty())
					wsTcPr += L"<w:vAlign w:val=\"center\"/>";
				else
					wsTcPr += L"<w:vAlign w:val=\"" + wsVerticalAlign + L"\"/>";

				if (NULL != oTableStyles.m_pPadding && !oStyle.m_oPadding.Empty() && oStyle.m_oPadding != *oTableStyles.m_pPadding)
				{
					const int nTopPadding    = std::max(oTableStyles.m_pPadding->GetTop()   .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT),
					                                    oStyle      .m_oPadding.GetTop()    .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
					const int nLeftPadding   = std::max(oTableStyles.m_pPadding->GetLeft()  .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH ),
					                                    oStyle      .m_oPadding.GetLeft()   .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH ));
					const int nBottomPadding = std::max(oTableStyles.m_pPadding->GetBottom().ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT),
					                                    oStyle      .m_oPadding.GetBottom() .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
					const int nRightPadding  = std::max(oTableStyles.m_pPadding->GetRight() .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH ),
					                                    oStyle      .m_oPadding.GetRight()  .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH ));
		
					wsTcPr += L"<w:tcMar>"
							       "<w:top w:w=\""    + std::to_wstring(nTopPadding)    + L"\" w:type=\"dxa\"/>"
							       "<w:left w:w=\""   + std::to_wstring(nLeftPadding)   + L"\" w:type=\"dxa\"/>"
							       "<w:bottom w:w=\"" + std::to_wstring(nBottomPadding) + L"\" w:type=\"dxa\"/>"
							       "<w:right w:w=\""  + std::to_wstring(nRightPadding)  + L"\" w:type=\"dxa\"/>"
							   "</w:tcMar>";
				}

				if(nRowspan != 1)
				{
					oXml->WriteString(L"<w:vMerge w:val=\"restart\"/>");
					std::wstring sColspan = std::to_wstring(nColspan);
					if(nRowspan == 0)
						mTable.push_back({0, j, sColspan, wsTcPr});
					else
						for(int k = i + 1; k < i + nRowspan; k++)
							mTable.push_back({k, j, sColspan, wsTcPr});
				}

				if(nColspan != 1)
					j += nColspan - 1;

				oXml->WriteString(wsTcPr);

				oXml->WriteString(L"<w:noWrap w:val=\"false\"/><w:textDirection w:val=\"lrTb\"/><w:hideMark/></w:tcPr>");
				m_bWasPStyle = false;

				// Читаем th. Ячейка заголовка таблицы. Выравнивание посередине. Выделяется полужирным
				if(m_oLightReader.GetName() == L"th")
				{
					CTextSettings oTSR(oTS);
					oTSR.sRStyle += L"<w:b/>";
					readStream(oXml, sSelectors, oTSR);
				}
				// Читаем td. Ячейка таблицы. Выравнивание вправо
				else if(m_oLightReader.GetName() == L"td")
				{
					readStream(oXml, sSelectors, oTS);
				}
				sSelectors.pop_back();
				
				if (m_bInP)
					wrP(oXml, sSelectors, oTS);
				else if (oXml->GetSubData(oXml->GetCurSize() - 6) != L"</w:p>")
					oXml->WriteString(L"<w:p></w:p>");

				CloseP(oXml, sSelectors);

				oXml->WriteString(L"</w:tc>");
				j++;

				// Вставляем ячейки после
				it1 = std::find_if(mTable.begin(), mTable.end(), [i, j](const CTc& item){ return item.i == i && item.j == j; });
				it2 = std::find_if(mTable.begin(), mTable.end(), [j]   (const CTc& item){ return item.i == 0 && item.j == j; });
				while(it1 != mTable.end() || it2 != mTable.end())
				{
					oXml->WriteString(L"<w:tc><w:tcPr>");
					oXml->WriteString(it1->sPr);
					oXml->WriteString(L"<w:vMerge w:val=\"continue\"/><w:gridSpan w:val=\"");
					std::wstring sCol = (it1 != mTable.end() ? it1->sGridSpan : it2->sGridSpan);
					oXml->WriteString(sCol);
					oXml->WriteString(L"\"/><w:noWrap w:val=\"false\"/><w:textDirection w:val=\"lrTb\"/></w:tcPr><w:p></w:p></w:tc>");
					j += stoi(sCol);
					it1 = std::find_if(mTable.begin(), mTable.end(), [i, j](const CTc& item){ return item.i == i && item.j == j; });
					it2 = std::find_if(mTable.begin(), mTable.end(), [j]   (const CTc& item){ return item.i == 0 && item.j == j; });
				}
			} while(m_oLightReader.ReadNextSiblingNode(nTrDeath) && j < MAXCOLUMNSINTABLE);

			if (j > unMaxColumns)
				unMaxColumns = j;
			else
			{
				for (; j < unMaxColumns; ++j)
					oXml->WriteString(L"<w:tc><w:tcPr><w:tcW w:w=\"0\" w:type=\"auto\"/><w:tcBorders><w:left w:val=\"none\" w:sz=\"4\" w:color=\"auto\" w:space=\"0\"/><w:top w:val=\"none\" w:sz=\"4\" w:color=\"auto\" w:space=\"0\"/><w:right w:val=\"none\" w:sz=\"4\" w:color=\"auto\" w:space=\"0\"/><w:bottom w:val=\"none\" w:color=\"auto\" w:sz=\"4\" w:space=\"0\"/></w:tcBorders><w:noWrap w:val=\"false\"/><w:textDirection w:val=\"lrTb\"/><w:hideMark/></w:tcPr><w:p></w:p></w:tc>");
			}

			oXml->WriteString(L"</w:tr>");
			i++;
		}
	}

	void readTable  (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
	{
		if(m_oLightReader.IsEmptyNode())
			return;

		NSStringUtils::CStringBuilder oHead;
		NSStringUtils::CStringBuilder oBody;
		NSStringUtils::CStringBuilder oFoot;

		NSCSS::CCompiledStyle oStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors, false);

		TTableStyles oTableStyles;

		oTableStyles.m_pCollapse = &oStyle.m_oBorder.GetCollapse();
		if (sSelectors.back().m_mAttributes.end() != sSelectors.back().m_mAttributes.find(L"border"))
			oTableStyles.m_bHaveBorderAttribute = true;

//		if (oXml->GetSubData(oXml->GetCurSize() - 6) != L"</w:p>")
//			oXml->WriteString(L"<w:p><w:pPr><w:spacing w:beforeLines=\"0\" w:before=\"0\" w:afterLines=\"0\" w:after=\"0\"/><w:rPr><w:vanish/><w:sz w:val=\"2\"/><w:szCs w:val=\"2\"/></w:rPr></w:pPr></w:p>");

		m_bWasSpace = false;

		// Начало таблицы
		std::wstring wsTable = L"<w:tbl><w:tblPr>";

		if (!oStyle.m_oDisplay.GetWidth().Empty())
		{
			if (NSCSS::UnitMeasure::Percent == oStyle.m_oDisplay.GetWidth().GetUnitMeasure())
			{
				
				wsTable += L"<w:tblW w:w=\"" + std::to_wstring(oStyle.m_oDisplay.GetWidth().ToInt(NSCSS::UnitMeasure::Percent, 5000)) + L"\" w:type=\"pct\"/>";
			}
			else
				wsTable += L"<w:tblW w:w=\"" + std::to_wstring(oStyle.m_oDisplay.GetWidth().ToInt(NSCSS::UnitMeasure::Twips)) + L"\" w:type=\"dxa\"/>";
		}
		else
			wsTable += L"<w:tblW w:w=\"0\" w:type=\"auto\"/>";

		if (sSelectors.back().m_mAttributes.end() != sSelectors.back().m_mAttributes.find(L"cellspacing"))
		{
			oTableStyles.m_nCellSpacing = NSStringFinder::ToInt(sSelectors.back().m_mAttributes[L"cellspacing"]);

			wsTable += L"<w:tblCellSpacing w:w=\"" + std::to_wstring(oTableStyles.m_nCellSpacing) + L"\" w:type=\"dxa\"/>";
		}

		std::wstring wsAlign = oStyle.m_oDisplay.GetHAlign().ToWString();

		if (wsAlign.empty())
		{
			NSCSS::CNode oLastNode = sSelectors.back();
			sSelectors.pop_back();

			NSCSS::CCompiledStyle oTempSettingsStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors, true);

			wsAlign = oTempSettingsStyle.m_oText.GetAlign().ToWString();

			if (wsAlign.empty())
			{
				NSCSS::CCompiledStyle oTempStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors, false);

				wsAlign = oTempStyle.m_oText.GetAlign().ToWString();
			}

			sSelectors.push_back(oLastNode);
		}

		// borders
		if (!oStyle.m_oBorder.Empty())
			wsTable += L"<w:tblBorders>" + CreateBorders(oStyle.m_oBorder) + L"</w:tblBorders>";

		if (!oStyle.m_oPadding.Empty())
		{
			const int nTopPadding    = std::max(0, oStyle.m_oPadding.GetTop()   .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
			const int nLeftPadding   = std::max(0, oStyle.m_oPadding.GetLeft()  .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH ));
			const int nBottomPadding = std::max(0, oStyle.m_oPadding.GetBottom().ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_HEIGHT));
			const int nRightPadding  = std::max(0, oStyle.m_oPadding.GetRight() .ToInt(NSCSS::UnitMeasure::Twips, DEFAULT_PAGE_WIDTH ));

			wsTable += L"<w:tblCellMar>"
			               "<w:top w:w=\""    + std::to_wstring(nTopPadding)    + L"\" w:type=\"dxa\"/>"
			               "<w:left w:w=\""   + std::to_wstring(nLeftPadding)   + L"\" w:type=\"dxa\"/>"
			               "<w:bottom w:w=\"" + std::to_wstring(nBottomPadding) + L"\" w:type=\"dxa\"/>"
			               "<w:right w:w=\""  + std::to_wstring(nRightPadding)  + L"\" w:type=\"dxa\"/>"
			           "</w:tblCellMar>";

			oTableStyles.m_pPadding = &oStyle.m_oPadding;
		}
		else
			wsTable += L"<w:tblCellMar><w:top w:w=\"15\" w:type=\"dxa\"/><w:left w:w=\"15\" w:type=\"dxa\"/><w:bottom w:w=\"15\" w:type=\"dxa\"/><w:right w:w=\"15\" w:type=\"dxa\"/></w:tblCellMar>";

		if (!wsAlign.empty())
			wsTable += L"<w:jc w:val=\"" + wsAlign + L"\"/>";

		wsTable += L"<w:tblLook w:val=\"04A0\" w:noVBand=\"1\" w:noHBand=\"0\" w:lastColumn=\"0\" w:firstColumn=\"1\" w:lastRow=\"0\" w:firstRow=\"1\"/>";
		wsTable += L"</w:tblPr>";

		oXml->WriteString(wsTable);

		/*
		NSCSS::CCompiledStyle oStyleSetting = m_oStylesCalculator.GetCompiledStyle(sSelectors, true);
		oStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors);
		NSCSS::CCompiledStyle::StyleEquation(oStyle, oStyleSetting);
		m_oXmlStyle.WriteLitePStyle(oStyleSetting);
		std::wstring sPSettings = m_oXmlStyle.GetStyle();
		m_oXmlStyle.Clear();
		size_t nBdr = sPSettings.find(L"<w:pBdr>");
		if (nBdr != std::wstring::npos)
		{
			nBdr += 8;
			size_t nBdrEnd = sPSettings.find(L"</w:pBdr>", nBdr);
			if (nBdrEnd != std::wstring::npos)
			{
				sBorders = sPSettings.substr(nBdr, nBdrEnd - nBdr);
				size_t nSpace = sBorders.find(L"w:space=\"");
				while (nSpace != std::wstring::npos)
				{
					nSpace += 9;
					size_t nSpaceEnd = sBorders.find(L'\"', nSpace);
					sBorders.replace(nSpace, nSpaceEnd - nSpace, L"0");
					nSpace = sBorders.find(L"w:space=\"", nSpace);
				}
			}
		}
		*/

		int nDeath = m_oLightReader.GetDepth();
		while(m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			std::wstring sName = m_oLightReader.GetName();
			GetSubClass(oXml, sSelectors);
			// Заголовок таблицы
			if(sName == L"caption")
			{
				size_t nHyp = 0;
				if (!m_bInP)
				{
					oXml->WriteString(L"<w:p>");
					for (const NSCSS::CNode& item : sSelectors)
					{
						if (item.m_wsName == L"a")
						{
							oXml->WriteString(L"<w:hyperlink>");
							nHyp++;
						}
					}
					m_bInP = true;
					m_bWasPStyle = false;
				}
				// Заголовок таблицы выравнивание посередине
				CTextSettings oTSP { oTS.bBdo, oTS.bPre, oTS.nLi, oTS.sRStyle, oTS.sPStyle + L"<w:jc w:val=\"center\"/>" };
				readStream(oXml, sSelectors, oTSP);
				if (m_bInP)
					m_bWasPStyle = false;
				CloseP(oXml, sSelectors);
			}
			if(sName == L"thead")
				readTr(&oHead, sSelectors, oTS, oTableStyles);
			else if(sName == L"tbody")
				readTr(&oBody, sSelectors, oTS, oTableStyles);
			else if(sName == L"tfoot")
				readTr(&oFoot, sSelectors, oTS, oTableStyles);
			sSelectors.pop_back();
		}

		// Конец таблицы
		oXml->WriteString(oHead.GetData());
		oXml->WriteString(oBody.GetData());
		oXml->WriteString(oFoot.GetData());
		oXml->WriteString(L"</w:tbl>");
	}

	void readInput  (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
	{
		std::wstring sValue;
		std::wstring sAlt;
		std::wstring sType;
		while(m_oLightReader.MoveToNextAttribute())
		{
			std::wstring sName = m_oLightReader.GetName();
			if(sName == L"value")
				sValue = m_oLightReader.GetText();
			else if(sName == L"alt")
				sAlt = m_oLightReader.GetText();
			else if(sName == L"type")
				sType = m_oLightReader.GetText();
		}
		m_oLightReader.MoveToElement();
		if(sType == L"hidden")
			return;
		if(sValue.empty())
			sValue = sAlt;
		if(!sValue.empty())
		{
			wrP(oXml, sSelectors, oTS);
			oXml->WriteString(L"<w:r>");
			wrR(oXml, sSelectors, oTS);
			oXml->WriteString(L"<w:t xml:space=\"preserve\">");
			oXml->WriteEncodeXmlString(sValue + L' ');
			oXml->WriteString(L"</w:t></w:r>");
		}
		readStream(oXml, sSelectors, oTS);
	}

	void readLi     (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS, bool bType)
	{
		if(m_oLightReader.IsEmptyNode())
			return;

		std::wstring sStart = L"1";
		while(m_oLightReader.MoveToNextAttribute())
			if(m_oLightReader.GetName() == L"start")
				sStart = m_oLightReader.GetText();
		m_oLightReader.MoveToElement();

		int nDeath = m_oLightReader.GetDepth();
		while(m_oLightReader.ReadNextSiblingNode(nDeath))
		{
			std::wstring sName = m_oLightReader.GetName();
			if (sName == L"optgroup")
			{
				GetSubClass(oXml, sSelectors);
				while(m_oLightReader.MoveToNextAttribute())
				{
					if(m_oLightReader.GetName() != L"label")
						continue;
					CloseP(oXml, sSelectors);
					wrP(oXml, sSelectors, oTS);
					oXml->WriteString(L"<w:r>");
					wrR(oXml, sSelectors, oTS);
					oXml->WriteString(L"<w:t xml:space=\"preserve\">");
					oXml->WriteEncodeXmlString(m_oLightReader.GetText());
					oXml->WriteString(L"</w:t></w:r>");
				}
				m_oLightReader.MoveToElement();
				readLi(oXml, sSelectors, oTS, true);
				sSelectors.pop_back();
				continue;
			}
			if (sName != L"li" && sName != L"option")
			{
				readInside(oXml, sSelectors, oTS, sName);
				continue;
			}

			GetSubClass(oXml, sSelectors);
			while(m_oLightReader.MoveToNextAttribute())
				if(m_oLightReader.GetName() == L"value")
					sStart = m_oLightReader.GetText();
			m_oLightReader.MoveToElement();

			CloseP(oXml, sSelectors);

			CTextSettings oTSLiP(oTS);
			oTSLiP.nLi++;
			oTSLiP.sPStyle += L"<w:numPr><w:ilvl w:val=\"" + std::to_wstring(oTSLiP.nLi) + L"\"/><w:numId w:val=\"" +
			                  (bType ? L"1" : std::to_wstring(m_nNumberingId + 1)) + L"\"/></w:numPr>";
			readStream(oXml, sSelectors, oTSLiP);

			CloseP(oXml, sSelectors);

			sSelectors.pop_back();
		}
		// Нумерованный список
		if(!bType)
		{
			m_oNumberXml.WriteString(L"<w:abstractNum w:abstractNumId=\"");
			m_oNumberXml.WriteString(std::to_wstring(m_nNumberingId++));
			m_oNumberXml.WriteString(L"\"><w:multiLevelType w:val=\"hybridMultilevel\"/><w:lvl w:ilvl=\"0\"><w:start w:val=\"");
			m_oNumberXml.WriteString(sStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%1.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"709\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"1\"><w:start w:val=\"");
			m_oNumberXml.WriteString(sStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"lowerLetter\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%2.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"1429\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"2\"><w:start w:val=\"");
			m_oNumberXml.WriteString(sStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"lowerRoman\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%3.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"2149\" w:hanging=\"180\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"3\"><w:start w:val=\"");
			m_oNumberXml.WriteString(sStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%4.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"2869\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"4\"><w:start w:val=\"");
			m_oNumberXml.WriteString(sStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"lowerLetter\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%5.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"3589\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"5\"><w:start w:val=\"");
			m_oNumberXml.WriteString(sStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"lowerRoman\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%6.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"4309\" w:hanging=\"180\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"6\"><w:start w:val=\"");
			m_oNumberXml.WriteString(sStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"decimal\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%7.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5029\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"7\"><w:start w:val=\"");
			m_oNumberXml.WriteString(sStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"lowerLetter\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%8.\"/><w:lvlJc w:val=\"left\"/><w:pPr><w:ind w:left=\"5749\" w:hanging=\"360\"/></w:pPr></w:lvl><w:lvl w:ilvl=\"8\"><w:start w:val=\"");
			m_oNumberXml.WriteString(sStart);
			m_oNumberXml.WriteString(L"\"/><w:numFmt w:val=\"lowerRoman\"/><w:isLgl w:val=\"false\"/><w:suff w:val=\"tab\"/><w:lvlText w:val=\"%9.\"/><w:lvlJc w:val=\"right\"/><w:pPr><w:ind w:left=\"6469\" w:hanging=\"180\"/></w:pPr></w:lvl></w:abstractNum>");
		}
	}

	void readA      (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS, std::wstring& sNote)
	{
		std::wstring sRef;
		std::wstring sAlt;
		bool bCross = false;
		std::wstring sFootnote;
		while(m_oLightReader.MoveToNextAttribute())
		{
			std::wstring sName = m_oLightReader.GetName();
			std::wstring sText = m_oLightReader.GetText();
			if(sName == L"href")
			{
				sRef = sText;
				if(sRef.find('#') != std::wstring::npos)
					bCross = true;
			}
			else if(sName == L"name")
			{
				std::wstring sCrossId = std::to_wstring(m_nCrossId++);
				oXml->WriteString(L"<w:bookmarkStart w:id=\"");
				oXml->WriteString(sCrossId);
				oXml->WriteString(L"\" w:name=\"");
				oXml->WriteString(sText);
				oXml->WriteString(L"\"/><w:bookmarkEnd w:id=\"");
				oXml->WriteString(sCrossId);
				oXml->WriteString(L"\"/>");
			}
			else if(sName == L"alt")
				sAlt = sText;
			else if (sName == L"style" && sText.find(L"mso-footnote-id") != std::wstring::npos)
				sFootnote = sText.substr(sText.rfind(L':') + 1);
			else if (sName == L"epub:type" && sText.find(L"noteref"))
				sFootnote = L"href";
		}
		m_oLightReader.MoveToElement();
		if(sNote.empty())
			sNote = sRef;

		if (bCross && sFootnote == L"href")
			sFootnote = sRef.substr(sRef.find('#') + 1);

		if (!m_bInP)
		{
			oXml->WriteString(L"<w:p>");
			for (size_t i = 0; i < sSelectors.size() - 1; i++)
				if (sSelectors[i].m_wsName == L"a")
					oXml->WriteString(L"<w:hyperlink>");
			m_bInP = true;
			m_bWasPStyle = false;
		}
		wrP(oXml, sSelectors, oTS);
		// Перекрестная ссылка внутри файла
		if(bCross)
		{
			oXml->WriteString(L"<w:hyperlink w:tooltip=\"Current Document\" w:anchor=\"");
			size_t nSharp = sRef.find('#');
			if(nSharp == std::wstring::npos)
				oXml->WriteString(NSFile::GetFileName(sRef));
			else
				oXml->WriteEncodeXmlString(sRef.c_str() + nSharp + 1);
		}
		// Внешняя ссылка
		else
		{
			// Пишем рельсы
			NSStringUtils::CStringBuilder* oRelationshipXml = &m_oDocXmlRels;
			if (oXml == &m_oNoteXml)
				oRelationshipXml = &m_oNoteXmlRels;
			oRelationshipXml->WriteString(L"<Relationship Id=\"rHyp");
			oRelationshipXml->WriteString(std::to_wstring(m_nHyperlinkId));
			oRelationshipXml->WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink\" Target=\"");
			oRelationshipXml->WriteEncodeXmlString(sRef);
			oRelationshipXml->WriteString(L"\" TargetMode=\"External\"/>");

			// Пишем в document.xml
			oXml->WriteString(L"<w:hyperlink w:tooltip=\"");
			oXml->WriteEncodeXmlString(sNote);
			oXml->WriteString(L"\" r:id=\"rHyp");
			oXml->WriteString(std::to_wstring(m_nHyperlinkId++));
		}
		oXml->WriteString(L"\">");

		if(!readStream(oXml, sSelectors, oTS))
		{
			oXml->WriteString(L"<w:r>");
			wrR(oXml, sSelectors, oTS);
			oXml->WriteString(L"<w:t xml:space=\"preserve\">");
			oXml->WriteEncodeXmlString(sAlt);
			oXml->WriteString(L"</w:t></w:r>");
		}
		if (m_bInP)
		{
			oXml->WriteString(L"</w:hyperlink>");

			bool bFootnote = false;
			if (sSelectors.size() > 1)
			{
				const NSCSS::CNode& oNode = sSelectors[sSelectors.size() - 2];
				bFootnote = oNode.m_wsName == L"p" && oNode.m_wsClass == L"MsoFootnoteText";
			}

			// Сноска
			if (bCross && !sFootnote.empty())
			{
				if (!bFootnote)
				{
					std::wstring sFootnoteID = std::to_wstring(m_nFootnoteId++);
					oXml->WriteString(L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteReference w:id=\"");
					oXml->WriteString(sFootnoteID);
					oXml->WriteString(L"\"/></w:r>");
					m_mFootnotes.insert(std::make_pair(sFootnote, sFootnoteID));
				}
				else
					oXml->WriteString(L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteRef/></w:r>");
			}
		}
		sNote = L"";
	}

	bool readBase64 (const std::wstring& sSrcM, std::wstring& sExtention)
	{
		bool bRes = false;
		size_t nBase = sSrcM.find(L"/", 4);
		nBase++;

		size_t nEndBase = sSrcM.find(L";", nBase);
		if (nEndBase == std::wstring::npos)
			return bRes;
		sExtention = sSrcM.substr(nBase, nEndBase - nBase);
		if (sExtention == L"octet-stream")
			sExtention = L"jpg";

		nBase = sSrcM.find(L"base64", nEndBase);
		if (nBase == std::wstring::npos)
			return bRes;

		int nOffset = nBase + 7;
		int nSrcLen = (int)(sSrcM.length() - nBase + 1);
		int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(nSrcLen);
		if (nDecodeLen != 0)
		{
			BYTE* pImageData = new BYTE[nDecodeLen];

			if (!pImageData || FALSE == NSBase64::Base64Decode(sSrcM.c_str() + nOffset, nSrcLen, pImageData, &nDecodeLen))
				return bRes;

			if (L"svg" == sExtention || L"svg+xml" == sExtention)
			{
				std::wstring wsSvg(pImageData, pImageData + nDecodeLen);
				bRes = readSVG(wsSvg);
			}
			else
			{
				NSFile::CFileBinary oImageWriter;
				std::wstring sImageName = std::to_wstring(m_arrImages.size()) + L'.' + sExtention;

				if (oImageWriter.CreateFileW(m_sDst + L"/word/media/i" + sImageName))
					bRes = oImageWriter.WriteFile(pImageData, (DWORD)nDecodeLen);

				oImageWriter.CloseFile();
			}

			RELEASEARRAYOBJECTS(pImageData);
		}

		return bRes;
	}

	bool CopyImage  (const std::wstring& wscSrc, const std::wstring& wsDst, bool bIsAllowExternalLocalFiles)
	{
		bool bRes = false;
		bool bAllow = true;

		std::wstring wsSrc = wscSrc;
		if (!bIsAllowExternalLocalFiles)
		{
			wsSrc = NSSystemPath::NormalizePath(wsSrc);
			std::wstring wsStartSrc = NSSystemPath::NormalizePath(m_sSrc);
			bAllow = wsSrc.substr(0, wsStartSrc.length()) == wsStartSrc;
		}
		if (bAllow)
			bRes = NSFile::CFileBinary::Copy(wsSrc, wsDst);

		return bRes;
	}

	inline bool NotValidExtension(const std::wstring& sExtention)
	{
		return  sExtention != L"bmp" && sExtention != L"emf"  && sExtention != L"emz"  && sExtention != L"eps"  && sExtention != L"fpx" && sExtention != L"gif"  &&
				sExtention != L"jpe" && sExtention != L"jpeg" && sExtention != L"jpg"  && sExtention != L"jfif" && sExtention != L"pct" && sExtention != L"pict" &&
				sExtention != L"png" && sExtention != L"pntg" && sExtention != L"psd"  && sExtention != L"qtif" && sExtention != L"sgi" && sExtention != L"svg"  &&
				sExtention != L"tga" && sExtention != L"tpic" && sExtention != L"tiff" && sExtention != L"tif"  && sExtention != L"wmf" && sExtention != L"wmz";
	}

	void ImageAlternative(NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS, const std::wstring& wsAlt)
	{
		if (wsAlt.empty())
			return;

		wrP(oXml, sSelectors, oTS);
		oXml->WriteString(L"<w:r>");
		wrR(oXml, sSelectors, oTS);
		oXml->WriteString(L"<w:t xml:space=\"preserve\">");
		oXml->WriteEncodeXmlString(wsAlt);
		oXml->WriteString(L"</w:t></w:r>");
	}

	void readImage  (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
	{
		std::wstring wsAlt, sSrcM;
		bool bRes = false;
		while (m_oLightReader.MoveToNextAttribute())
		{
			std::wstring wsName = m_oLightReader.GetName();
			if (wsName == L"alt")
				wsAlt = m_oLightReader.GetText();
			else if (wsName == L"src")
				sSrcM = m_oLightReader.GetText();
		}
		m_oLightReader.MoveToElement();

		if (sSrcM.empty())
		{
			ImageAlternative(oXml, sSelectors, oTS, wsAlt);
			return;
		}

		const bool bIsAllowExternalLocalFiles = GetStatusUsingExternalLocalFiles();

		bool bIsBase64 = false;
		if (sSrcM.length() > 4 && sSrcM.substr(0, 4) == L"data" && sSrcM.find(L"/", 4) != std::wstring::npos)
			bIsBase64 = true;

		if (!bIsBase64)
			sSrcM = NSSystemPath::ShortenPath(sSrcM);

		if (!CanUseThisPath(sSrcM, bIsAllowExternalLocalFiles))
			return;

		int nImageId = -1;
		std::wstring sImageSrc, sExtention;
		// Предполагаем картинку в Base64
		if (bIsBase64)
			bRes = readBase64(sSrcM, sExtention);

		if (!bRes)
		{
			// Проверка расширения
			sExtention = NSFile::GetFileExtention(sSrcM);
			std::transform(sExtention.begin(), sExtention.end(), sExtention.begin(), tolower);
			if (NotValidExtension(sExtention))
			{
				ImageAlternative(oXml, sSelectors, oTS, wsAlt);
				return;
			}

			// Проверка на повтор
			std::vector<std::wstring>::iterator nFind = std::find(m_arrImages.begin(), m_arrImages.end(), sSrcM);
			if (nFind != m_arrImages.end())
			{
				bRes = true;
				nImageId = nFind - m_arrImages.begin();
			}
		}

		if (!bRes)
		{
			sImageSrc = sSrcM;
			std::wstring wsDst = m_sDst + L"/word/media/i" + std::to_wstring(m_arrImages.size()) + L'.' + sExtention;

			// Предполагаем картинку по локальному пути
			if (!((!m_sBase.empty() && m_sBase.length() > 4 && m_sBase.substr(0, 4) == L"http") || (sSrcM.length() > 4 && sSrcM.substr(0, 4) == L"http")))
			{
				if (!m_sBase.empty())
				{
					if (!bRes)
						bRes = CopyImage(NSSystemPath::Combine(m_sBase, sSrcM), wsDst, bIsAllowExternalLocalFiles);
					if (!bRes)
						bRes = CopyImage(NSSystemPath::Combine(m_sSrc, m_sBase + sSrcM), wsDst, bIsAllowExternalLocalFiles);
				}
				if (!bRes)
					bRes = CopyImage(NSSystemPath::Combine(m_sSrc, sSrcM), wsDst, bIsAllowExternalLocalFiles);
				if (!bRes)
					bRes = CopyImage(m_sSrc + L"/" + NSFile::GetFileName(sSrcM), wsDst, bIsAllowExternalLocalFiles);
				if (!bRes)
					bRes = CopyImage(sSrcM, wsDst, bIsAllowExternalLocalFiles);
			}
			// Предполагаем картинку в сети
			else
			{
				// Проверка gc_allowNetworkRequest предполагается в kernel_network
				NSNetwork::NSFileTransport::CFileDownloader oDownloadImg(m_sBase + sSrcM, false);
				oDownloadImg.SetFilePath(wsDst);
				bRes = oDownloadImg.DownloadSync();
			}
		}

		if (!bRes)
			ImageAlternative(oXml, sSelectors, oTS, wsAlt);
		else
		{
			wrP(oXml, sSelectors, oTS);
			ImageRels(oXml, nImageId, sImageSrc, sExtention);
		}
	}

	std::wstring wrP(NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
	{
		if (!m_bInP)
		{
			oXml->WriteString(L"<w:p>");
			for (const NSCSS::CNode& item : sSelectors)
				if (item.m_wsName == L"a")
					oXml->WriteString(L"<w:hyperlink>");
			m_bInP = true;
			m_bWasPStyle = false;
		}

		if (m_bWasPStyle)
			return L"";

		oXml->WriteString(L"<w:pPr><w:pStyle w:val=\"");

		std::vector<std::pair<size_t, NSCSS::CNode>> temporary;
		size_t i = 0;
		while(i != sSelectors.size())
		{
			if (sSelectors[i].Empty() && rStyle.find(L' ' + sSelectors[i].m_wsName + L' ') != std::wstring::npos)
			{
				temporary.push_back(std::make_pair(i, sSelectors[i]));
				sSelectors.erase(sSelectors.begin() + i);
			}
			else
				i++;
		}
		NSCSS::CCompiledStyle oStyleSetting = m_oStylesCalculator.GetCompiledStyle(sSelectors, true);
		NSCSS::CCompiledStyle oStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors);

		NSCSS::CCompiledStyle::StyleEquation(oStyle, oStyleSetting);
		
		std::wstring sPStyle = GetStyle(oStyle, true);

		m_oXmlStyle.WriteLitePStyle(oStyleSetting);
		std::wstring sPSettings = m_oXmlStyle.GetStyle();
		m_oXmlStyle.Clear();

		for(int i = temporary.size() - 1; i >= 0; i--)
			sSelectors.insert(sSelectors.begin() + temporary[i].first, temporary[i].second);

		// Если в таблице, то игнориуются Paragraph Borders
		bool bInTable = false;
		for (const NSCSS::CNode& item : sSelectors)
			if (item.m_wsName == L"table")
				bInTable = true;
		if (bInTable)
		{
			size_t nBdr = sPSettings.find(L"<w:pBdr>");
			if (nBdr != std::wstring::npos)
			{
				size_t nBdrEnd = sPSettings.find(L"</w:pBdr>", nBdr);
				if (nBdrEnd != std::wstring::npos)
					sPSettings.erase(nBdr, nBdrEnd + 9 - nBdr);
			}
		}

		oXml->WriteString(sPStyle);
		oXml->WriteString(L"\"/>");
		oXml->WriteString(oTS.sPStyle + L' ' + sPSettings);
		oXml->WriteString(L"</w:pPr>");
		m_bWasPStyle = true;
		return sPStyle;
	}

	std::wstring wrR(NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const CTextSettings& oTS)
	{
		if (!m_bInP)
			return L"";

		NSCSS::CCompiledStyle oStyleSetting = m_oStylesCalculator.GetCompiledStyle(sSelectors, true);
		NSCSS::CCompiledStyle oStyle = m_oStylesCalculator.GetCompiledStyle(sSelectors);

		NSCSS::CCompiledStyle::StyleEquation(oStyle, oStyleSetting);

		std::wstring sRStyle = GetStyle(oStyle, false);

		m_oXmlStyle.WriteLiteRStyle(oStyleSetting);
		const std::wstring sRSettings = m_oXmlStyle.GetStyle();
		m_oXmlStyle.Clear();

		oXml->WriteString(L"<w:rPr><w:rStyle w:val=\"");
		oXml->WriteString(sRStyle);
		oXml->WriteString(L"\"/>");

		oXml->WriteString(oTS.sRStyle + L' ' + sRSettings);
		oXml->WriteString(L"</w:rPr>");
		return sRStyle;
	}

	void ImageRels  (NSStringUtils::CStringBuilder* oXml, int nImageId, const std::wstring& sImageSrc, const std::wstring& sExtention)
	{
		bool bNew = nImageId < 0;
		if (bNew)
			nImageId = m_arrImages.size();

		std::wstring sImageId = std::to_wstring(nImageId);
		std::wstring sImageName = sImageId + L'.' + sExtention;
		CBgraFrame oBgraFrame;
		if (!oBgraFrame.OpenFile(m_sDst + L"/word/media/i" + sImageName))
			return;

		// Прописать рельсы
		if (bNew)
		{
			m_arrImages.push_back(sImageSrc);

			m_oDocXmlRels.WriteString(L"<Relationship Id=\"rPic");
			m_oDocXmlRels.WriteString(sImageId);
			m_oDocXmlRels.WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/i");
			m_oDocXmlRels.WriteEncodeXmlString(sImageName);
			m_oDocXmlRels.WriteString(L"\"/>");
		}

		// Получаем размеры картинки
		int nHy = oBgraFrame.get_Height();
		int nWx = oBgraFrame.get_Width();
		if (nWx > nHy)
		{
			int nW = nWx * 9525;
			nW = (nW > 7000000 ? 7000000 : nW);
			nHy = (int)((double)nHy * (double)nW / (double)nWx);
			nWx = nW;
		}
		else
		{
			int nH = nHy * 9525;
			nH = (nH > 8000000 ? 8000000 : nH);
			int nW = (int)((double)nWx * (double)nH / (double)nHy);
			if (nW > 7000000)
			{
				nW = 7000000;
				nHy = (int)((double)nHy * (double)nW / (double)nWx);
			}
			else
				nHy = nH;
			nWx = nW;
		}

		// Пишем в document.xml
		oXml->WriteString(L"<w:r><w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\"><wp:extent cx=\"");
		oXml->WriteString(std::to_wstring(nWx));
		oXml->WriteString(L"\" cy=\"");
		oXml->WriteString(std::to_wstring(nHy));
		oXml->WriteString(L"\"/><wp:docPr id=\"");
		oXml->WriteString(sImageId);
		oXml->WriteString(L"\" name=\"\"/><a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:nvPicPr><pic:cNvPr id=\"");
		oXml->WriteString(sImageId);
		oXml->WriteString(L"\" name=\"\"/><pic:cNvPicPr></pic:cNvPicPr></pic:nvPicPr><pic:blipFill><a:blip r:embed=\"rPic");
		oXml->WriteString(sImageId);
		oXml->WriteString(L"\"/><a:stretch/></pic:blipFill><pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"");
		oXml->WriteString(std::to_wstring(nWx));
		oXml->WriteString(L"\" cy=\"");
		oXml->WriteString(std::to_wstring(nHy));
		oXml->WriteString(L"\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></pic:spPr></pic:pic></a:graphicData></a:graphic></wp:inline></w:drawing></w:r>");
	}

	void readNote   (NSStringUtils::CStringBuilder* oXml, std::vector<NSCSS::CNode>& sSelectors, const std::wstring& sNote)
	{
		if(sNote.empty())
			return;
		if (!m_bInP)
		{
			oXml->WriteString(L"<w:p>");
			for (const NSCSS::CNode& item : sSelectors)
				if (item.m_wsName == L"a")
					oXml->WriteString(L"<w:hyperlink>");
			m_bInP = true;
			m_bWasPStyle = false;
		}
		oXml->WriteString(L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteReference w:id=\"");
		oXml->WriteString(std::to_wstring(m_nFootnoteId));
		oXml->WriteString(L"\"/></w:r>");
		m_oNoteXml.WriteString(L"<w:footnote w:id=\"");
		m_oNoteXml.WriteString(std::to_wstring(m_nFootnoteId++));
		m_oNoteXml.WriteString(L"\"><w:p><w:pPr><w:pStyle w:val=\"footnote-p\"/></w:pPr><w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr></w:r><w:r><w:t xml:space=\"preserve\">");
		m_oNoteXml.WriteEncodeXmlString(sNote);
		m_oNoteXml.WriteString(L"</w:t></w:r></w:p></w:footnote>");
	}

	bool readSVG    (const std::wstring& wsSvg)
	{
		if (wsSvg.empty())
			return false;

		CSvgFile oSvgReader;

		NSFonts::IApplicationFonts* pFonts = NSFonts::NSApplication::Create();
		NSFonts::IFontManager* pFontManager = pFonts->GenerateFontManager();
		NSFonts::IFontsCache* pFontCache = NSFonts::NSFontCache::Create();

		pFontCache->SetStreams(pFonts->GetStreams());
		pFontManager->SetOwnerCache(pFontCache);

		oSvgReader.SetFontManager(pFontManager);

		if (!oSvgReader.ReadFromWString(wsSvg))
		{
			RELEASEINTERFACE(pFontManager);
			pFonts->Release();
			return false;
		}

		NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();
		pGrRenderer->SetFontManager(pFontManager);

		double dX, dY, dW, dH;
		oSvgReader.GetBounds(dX, dY, dW, dH);

		if (dW < 0) dW = -dW;
		if (dH < 0) dH = -dH;

		double dOneMaxSize = (double)1000.;

		if (dW > dH)
		{
			dH *= (dOneMaxSize / dW);
			dW = dOneMaxSize;
		}
		else
		{
			dW *= (dOneMaxSize / dH);
			dH = dOneMaxSize;
		}

		int nWidth  = static_cast<int>(dW + 0.5);
		int nHeight = static_cast<int>(dH + 0.5);

		double dWidth  = 25.4 * nWidth / 96;
		double dHeight = 25.4 * nHeight / 96;

		BYTE* pBgraData = (BYTE*)malloc(nWidth * nHeight * 4);
		if (!pBgraData)
		{
			double dKoef = 2000.0 / (nWidth > nHeight ? nWidth : nHeight);

			nWidth = (int)(dKoef * nWidth);
			nHeight = (int)(dKoef * nHeight);

			dWidth  = 25.4 * nWidth / 96;
			dHeight = 25.4 * nHeight / 96;

			pBgraData = (BYTE*)malloc(nWidth * nHeight * 4);
		}

		if (!pBgraData)
			return false;

		unsigned int alfa = 0xffffff;
		//дефолтный тон должен быть прозрачным, а не белым
		//memset(pBgraData, 0xff, nWidth * nHeight * 4);
		for (int i = 0; i < nWidth * nHeight; i++)
		{
			((unsigned int*)pBgraData)[i] = alfa;
		}
		CBgraFrame oFrame;
		oFrame.put_Data(pBgraData);
		oFrame.put_Width(nWidth);
		oFrame.put_Height(nHeight);
		oFrame.put_Stride(-4 * nWidth);

		pGrRenderer->CreateFromBgraFrame(&oFrame);
		pGrRenderer->SetSwapRGB(false);
		pGrRenderer->put_Width(dWidth);
		pGrRenderer->put_Height(dHeight);

		oSvgReader.SetWorkingDirectory(m_sSrc);
		oSvgReader.Draw(pGrRenderer, 0, 0, dWidth, dHeight);

		oFrame.SaveFile(m_sDst + L"/word/media/i" + std::to_wstring(m_arrImages.size()) + L".png", 4);
		oFrame.put_Data(NULL);

		RELEASEINTERFACE(pFontManager);
		RELEASEINTERFACE(pGrRenderer);

		if (pBgraData)
			free(pBgraData);

		pFonts->Release();

		return true;
	}
};

CHtmlFile2::CHtmlFile2()
{
	m_internal = new CHtmlFile2_Private();
}

CHtmlFile2::~CHtmlFile2()
{
	RELEASEOBJECT(m_internal);
}

bool CHtmlFile2::IsHtmlFile(const std::wstring& sFile)
{
	// Конвертируем в xhtml
	if(!m_internal->htmlXhtml(sFile))
		return false;
	// Читаем html
	return m_internal->isHtml();
}

bool CHtmlFile2::IsMhtFile(const std::wstring& sFile)
{
	// Конвертируем в xhtml
	if(!m_internal->mhtXhtml(sFile))
		return false;
	// Читаем html
	return m_internal->isHtml();
}

void CHtmlFile2::SetTmpDirectory(const std::wstring& sFolder)
{
	m_internal->m_sTmp = sFolder;
}

HRESULT CHtmlFile2::OpenHtml(const std::wstring& sSrc, const std::wstring& sDst, CHtmlParams* oParams)
{
	if(!m_internal->m_oLightReader.IsValid())
		if(!IsHtmlFile(sSrc))
			return S_FALSE;

	m_internal->m_sSrc = NSSystemPath::GetDirectoryName(sSrc);
	m_internal->m_sDst = sDst;
	m_internal->CreateDocxEmpty(oParams);
	m_internal->readStyle();

	m_internal->m_oStylesCalculator.SetBodyTree(m_internal->m_oTree);

	// Переходим в начало
	if(!m_internal->m_oLightReader.MoveToStart())
		return S_FALSE;

	if(oParams && oParams->m_bNeedPageBreakBefore)
		m_internal->PageBreakBefore();
	m_internal->readSrc();
	m_internal->write();
	return S_OK;
}

HRESULT CHtmlFile2::OpenMht(const std::wstring& sSrc, const std::wstring& sDst, CHtmlParams* oParams)
{
	if(!m_internal->m_oLightReader.IsValid())
		if(!IsMhtFile(sSrc))
			return S_FALSE;

	m_internal->m_sSrc = NSSystemPath::GetDirectoryName(sSrc);
	m_internal->m_sDst = sDst;
	m_internal->CreateDocxEmpty(oParams);
	m_internal->readStyle();

	m_internal->m_oStylesCalculator.SetBodyTree(m_internal->m_oTree);

	// Переходим в начало
	if(!m_internal->m_oLightReader.MoveToStart())
		return S_FALSE;

	if(oParams && oParams->m_bNeedPageBreakBefore)
		m_internal->PageBreakBefore();
	m_internal->readSrc();
	m_internal->write();
	return S_OK;
}

HRESULT CHtmlFile2::OpenBatchHtml(const std::vector<std::wstring>& sSrc, const std::wstring& sDst, CHtmlParams* oParams)
{
	m_internal->m_sDst = sDst;
	m_internal->CreateDocxEmpty(oParams);
	bool bFirst = true;

	for(const std::wstring& sS : sSrc)
	{
#ifdef _DEBUG
		std::wcout << NSFile::GetFileName(sS) << std::endl;
#endif

		m_internal->m_sSrc = NSSystemPath::GetDirectoryName(sS);
		if(!IsHtmlFile(sS))
			continue;
		m_internal->readStyle();

		m_internal->m_oStylesCalculator.SetBodyTree(m_internal->m_oTree);

		// Переходим в начало
		if(m_internal->m_oLightReader.MoveToStart())
		{
			if(oParams && oParams->m_bNeedPageBreakBefore && !bFirst)
				m_internal->PageBreakBefore();
			bFirst = false;
			m_internal->readSrc();
			m_internal->m_oLightReader.Clear();
			m_internal->m_sBase.clear();
		}
		m_internal->m_oStylesCalculator.Clear();
	}

	m_internal->write();
	return S_OK;
}
