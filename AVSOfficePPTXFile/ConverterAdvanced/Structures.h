#pragma once

#include "../stdafx.h"

#ifndef _AVS_PPT_SHAPE_INCLUDE_
#include "../AVSPresentationEditor/OfficeDrawing/Shapes/BaseShape/PPTShape/Formula.cpp"
#include "../AVSPresentationEditor/OfficeDrawing/Shapes/BaseShape/PPTShape/PPTShape.cpp"
#define _AVS_PPT_SHAPE_INCLUDE_
#endif

#include "../PPTXFormat/PPTX.h"
#include "../../AVSPresentationEditor/OfficeDrawing/Document.h"

#ifndef AVSINLINE
#if defined(_MSC_VER)
#define AVSINLINE __forceinline
#else
#define AVSINLINE inline
#endif
#endif

namespace PPTX2EditorAdvanced
{
	struct TableCellData
	{
		int x;
		int y;
		int width;
		int height;
		int colsCount;
		int rowsCount;
		int colNumber;
		int rowNumber;
		bool firstCol;
		bool lastCol;
		bool firstRow;
		bool lastRow;
		bool band1H;
		bool band1V;
	};
	
	class CPresentationInfo
	{
	public:
		class CThemeInfo
		{
		public:
			CAtlMap<size_t, LONG>			m_mapLayouts;
			NSPresentationEditor::CTheme	m_oTheme;

		public:
			CThemeInfo() : m_mapLayouts(), m_oTheme()
			{
			}
			~CThemeInfo()
			{
				m_mapLayouts.RemoveAll();
			}
			CThemeInfo(const CThemeInfo& oSrc)
			{
				*this = oSrc;
			}
			CThemeInfo& operator=(const CThemeInfo& oSrc)
			{
				m_oTheme = oSrc.m_oTheme;

				m_mapLayouts.RemoveAll();
				POSITION pos = oSrc.m_mapLayouts.GetStartPosition();
				while (NULL != pos)
				{
					const CAtlMap<size_t, LONG>::CPair* pPair = oSrc.m_mapLayouts.GetNext(pos);
					m_mapLayouts.SetAt((unsigned int)pPair->m_key, pPair->m_value);
				}

				return *this;
			}

		public:
			LONG GetIndexLayout(size_t pPointer)
			{
				CAtlMap<size_t, LONG>::CPair* pPair = m_mapLayouts.Lookup((unsigned int)pPointer);
				if (NULL == pPair)
					return -1;
				return pPair->m_value;
			}
			LONG GetIndexLayout(smart_ptr<PPTX::SlideLayout>& pLayout)
			{
				size_t pPointer = (size_t)(pLayout.operator ->());
				return GetIndexLayout(pPointer);
			}
			NSPresentationEditor::CLayout* AddLayout(smart_ptr<PPTX::SlideLayout>& pLayout)
			{
				size_t pPointer = (size_t)(pLayout.operator ->());
				LONG lIndex		= (LONG)m_oTheme.m_arLayouts.GetCount();

				m_oTheme.m_arLayouts.Add();
				m_mapLayouts.SetAt((unsigned int)pPointer, lIndex);

				return &m_oTheme.m_arLayouts[lIndex];
			}
		};
	public:
		CAtlArray<CThemeInfo*>	m_arThemes;
		CAtlMap<size_t, LONG>	m_mapThemes;

		CString					m_strDirectory;

		CPresentationInfo() : m_arThemes(), m_mapThemes()
		{
			m_strDirectory = _T("");
		}
		~CPresentationInfo()
		{
			size_t nCount = m_arThemes.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				CThemeInfo* pInfo = m_arThemes[i];
				RELEASEOBJECT(pInfo);
			}
			m_arThemes.RemoveAll();
		}

		CPresentationInfo(const CPresentationInfo& oSrc)
		{
			*this = oSrc;
		}
		CPresentationInfo& operator=(const CPresentationInfo& oSrc)
		{
			m_arThemes.Copy(oSrc.m_arThemes);

			m_mapThemes.RemoveAll();
			POSITION pos = oSrc.m_mapThemes.GetStartPosition();
			while (NULL != pos)
			{
				const CAtlMap<size_t, LONG>::CPair* pPair = oSrc.m_mapThemes.GetNext(pos);
				m_mapThemes.SetAt((unsigned int)pPair->m_key, pPair->m_value);
			}

			m_strDirectory = oSrc.m_strDirectory;

			return *this;
		}

	public:
		LONG GetIndexTheme(size_t pPointer)
		{
			CAtlMap<size_t, LONG>::CPair* pPair = m_mapThemes.Lookup((unsigned int)pPointer);
			if (NULL == pPair)
				return -1;
			return pPair->m_value;
		}
		LONG GetIndexTheme(smart_ptr<PPTX::SlideMaster>& pMaster)
		{
			size_t pPointer = (size_t)(pMaster.operator ->());
			return GetIndexTheme(pPointer);
		}
		CThemeInfo* AddTheme(smart_ptr<PPTX::SlideMaster>& pMaster)
		{
			size_t pPointer = (size_t)(pMaster.operator ->());
			LONG lIndex		= (LONG)m_arThemes.GetCount();

			CThemeInfo* pTheme = new CThemeInfo();
			
			m_arThemes.Add(pTheme);
			m_mapThemes.SetAt((unsigned int)pPointer, lIndex);

			return pTheme;			
		}
	};
}

namespace PPTX2EditorAdvanced
{
	const double c_dScaleElements = 1.0 / 36000;

	AVSINLINE DWORD GetTLCalcModeFromStr(const CString& str)
	{
		if (str == _T("discrete"))	return 0;
		if (str == _T("lin"))		return 1;
		if (str == _T("fmla"))		return 2;
		return 0;
	}

	AVSINLINE DWORD GetTLNodeTypeFromStr(const CString& str)
	{
		if (str == _T("clickEffect"))		return 1;
		if (str == _T("withEffect"))		return 2;
		if (str == _T("afterEffect"))		return 3;
		if (str == _T("mainSeq"))			return 4;
		if (str == _T("interactiveSeq"))	return 5;
		if (str == _T("clickPar"))			return 6;
		if (str == _T("withGroup"))			return 7;
		if (str == _T("afterGroup"))		return 8;
		if (str == _T("tmRoot"))			return 9;
		return 9;
	}

	AVSINLINE DWORD GetTLPresetClassFromStr(const CString& str)
	{
		if (str == _T("entr"))		return 1;
		if (str == _T("exit"))		return 2;
		if (str == _T("emph"))		return 3;
		if (str == _T("path"))		return 4;
		if (str == _T("verb"))		return 5;
		if (str == _T("mediacall")) return 6;
		return 0;
	}

	AVSINLINE DWORD GetTLRestartFromStr(const CString& str)
	{
		if (str == _T("never"))			return 0;
		if (str == _T("always"))		return 1;
		if (str == _T("whenNotActive")) return 2;
		return 0;
	}

	AVSINLINE DWORD GetTLValueTypeFromStr(const CString& str)
	{
		if (str == _T("str")) return 0;
		if (str == _T("num")) return 1;
		if (str == _T("clr")) return 2;
		return 1;
	}

	AVSINLINE WORD GetTextAnchorFromStr(const CString& str)
	{
		if (str == _T("t"))		return 0;
		if (str == _T("ctr"))	return 1;
		if (str == _T("b"))		return 2;
		return 0;
	}

	AVSINLINE WORD GetTextAlignFromStr(const CString& str)
	{
		if (str == _T("l"))			return 0;
		if (str == _T("ctr"))		return 1;
		if (str == _T("r"))			return 2;
		if (str == _T("just"))		return 3;
		if (str == _T("dist"))		return 4;
		if (str == _T("thaiDist"))	return 5;
		if (str == _T("justLow"))	return 6;
		return 0;
	}

	AVSINLINE WORD GetFontAlignFromStr(const CString& str)
	{
		if (str == _T("auto"))	return 0;
		if (str == _T("base"))	return 0;
		if (str == _T("t"))		return 1;
		if (str == _T("ctr"))	return 2;
		if (str == _T("b"))		return 3;
		return 0;
	}

	AVSINLINE WORD GetPattFillValFromStr(const CString& str)
	{
		WORD result = 4009;
		if (str == _T(""))
			return result;
		switch((CHAR)str[0])
		{
		case 'c':
			if (str == _T("cross"))		{ result += 4; break;} //(Cross)
			break;
		case 'd':
			if (str == _T("dashDnDiag")){ result += 30; break;} //(Dashed Downward Diagonal)
			if (str == _T("dashHorz"))	{ result += 32; break;} //(Dashed Horizontal)
			if (str == _T("dashUpDiag")){ result += 31; break;} //(Dashed Upward DIagonal)
			if (str == _T("dashVert"))	{ result += 33; break;} //(Dashed Vertical)
			if (str == _T("diagBrick"))	{ result += 38; break;} //(Diagonal Brick)
			if (str == _T("diagCross"))	{ result += 5; break;} //(Diagonal Cross)
			if (str == _T("divot"))		{ result += 42; break;} //(Divot)
			if (str == _T("dkDnDiag"))	{ result += 20; break;} //(Dark Downward Diagonal)
			if (str == _T("dkHorz"))	{ result += 29; break;} //(Dark Horizontal)
			if (str == _T("dkUpDiag"))	{ result += 21; break;} //(Dark Upward Diagonal)
			if (str == _T("dkVert"))	{ result += 28; break;} //(Dark Vertical)
			if (str == _T("dnDiag"))	{ result += 3; break;} //(Downward Diagonal)
			if (str == _T("dotDmnd"))	{ result += 44; break;} //(Dotted Diamond)
			if (str == _T("dotGrid"))	{ result += 43; break;} //(Dotted Grid)
			break;
		case 'h':
			if (str == _T("horz"))		{ result += 0; break;} //(Horizontal)
			if (str == _T("horzBrick"))	{ result += 39; break;} //(Horizontal Brick)
			break;
		case 'l':
			if (str == _T("lgCheck"))	{ result += 50; break;} //(Large Checker Board)
			if (str == _T("lgConfetti")){ result += 35; break;} //(Large Confetti)
			if (str == _T("lgGrid"))	{ result += 4; break;} //(Large Grid)
			if (str == _T("ltDnDiag"))	{ result += 18; break;} //(Light Downward Diagonal)
			if (str == _T("ltHorz"))	{ result += 25; break;} //(Light Horizontal)
			if (str == _T("ltUpDiag"))	{ result += 19; break;} //(Light Upward Diagonal)
			if (str == _T("ltVert"))	{ result += 24; break;} //(Light Vertical)
			break;
		case 'n':
			if (str == _T("narHorz"))	{ result += 27; break;} //(Narrow Horizontal)
			if (str == _T("narVert"))	{ result += 26; break;} //(Narrow Vertical)
			break;
		case 'o':
			if (str == _T("openDmnd"))	{ result += 51; break;} //(Open Diamond)
			break;
		case 'p':
			if (str == _T("pct10"))		{ result += 7; break;} //(10%)
			if (str == _T("pct20"))		{ result += 8; break;} //(20%)
			if (str == _T("pct25"))		{ result += 9; break;} //(25%)
			if (str == _T("pct30"))		{ result += 10; break;} //(30%)
			if (str == _T("pct40"))		{ result += 11; break;} //(40%)
			if (str == _T("pct5"))		{ result += 6; break;} //(5%)
			if (str == _T("pct50"))		{ result += 12; break;} //(50%)
			if (str == _T("pct60"))		{ result += 13; break;} //(60%)
			if (str == _T("pct70"))		{ result += 14; break;} //(70%)
			if (str == _T("pct75"))		{ result += 15; break;} //(75%)
			if (str == _T("pct80"))		{ result += 16; break;} //(80%)
			if (str == _T("pct90"))		{ result += 17; break;} //(90%)
			if (str == _T("plaid"))		{ result += 41; break;} //(Plaid)
			break;
		case 's':
			if (str == _T("shingle"))	{ result += 45; break;} //(Shingle)
			if (str == _T("smCheck"))	{ result += 49; break;} //(Small Checker Board)
			if (str == _T("smConfetti")){ result += 34; break;} //(Small Confetti)
			if (str == _T("smGrid"))	{ result += 48; break;} //(Small Grid)
			if (str == _T("solidDmnd"))	{ result += 52; break;} //(Solid Diamond)
			if (str == _T("sphere"))	{ result += 47; break;} //(Sphere)
			break;
		case 't':
			if (str == _T("trellis"))	{ result += 46; break;} //(Trellis)
			break;
		case 'u':
			if (str == _T("upDiag"))	{ result += 2; break;} //(Upward Diagonal)
			break;
		case 'v':
			if (str == _T("vert"))		{ result += 1; break;} //(Vertical)
			break;
		case 'w':
			if (str == _T("wave"))		{ result += 37; break;} //(Wave)
			if (str == _T("wdDnDiag"))	{ result += 22; break;} //(Wide Downward Diagonal)
			if (str == _T("wdUpDiag"))	{ result += 23; break;} //(Wide Upward Diagonal)
			if (str == _T("weave"))		{ result += 40; break;} //(Weave)
			break;
		case 'z':
			if (str == _T("zigZag"))	{ result += 36; break;} //(Zig Zag)
			break;
		}
		return result;
	}

	AVSINLINE OOXMLShapes::ShapeType GetShapeTypeFromStr(const CString& str)//const
	{
		if(str == _T(""))
			return OOXMLShapes::sptNil;
		switch((CHAR)str[0])
		{
		case 'a':
			if (str == _T("accentBorderCallout1")) return OOXMLShapes::sptCAccentBorderCallout1;
			if (str == _T("accentBorderCallout2")) return OOXMLShapes::sptCAccentBorderCallout2;
			if (str == _T("accentBorderCallout3")) return OOXMLShapes::sptCAccentBorderCallout3;
			if (str == _T("accentCallout1")) return OOXMLShapes::sptCAccentCallout1;
			if (str == _T("accentCallout2")) return OOXMLShapes::sptCAccentCallout2;
			if (str == _T("accentCallout3")) return OOXMLShapes::sptCAccentCallout3;
			if (str == _T("actionButtonBackPrevious")) return OOXMLShapes::sptCActionButtonBackPrevious;
			if (str == _T("actionButtonBeginning")) return OOXMLShapes::sptCActionButtonBeginning;
			if (str == _T("actionButtonBlank")) return OOXMLShapes::sptCActionButtonBlank;
			if (str == _T("actionButtonDocument")) return OOXMLShapes::sptCActionButtonDocument;
			if (str == _T("actionButtonEnd")) return OOXMLShapes::sptCActionButtonEnd;
			if (str == _T("actionButtonForwardNext")) return OOXMLShapes::sptCActionButtonForwardNext;
			if (str == _T("actionButtonHelp")) return OOXMLShapes::sptCActionButtonHelp;
			if (str == _T("actionButtonHome")) return OOXMLShapes::sptCActionButtonHome;
			if (str == _T("actionButtonInformation")) return OOXMLShapes::sptCActionButtonInformation;
			if (str == _T("actionButtonMovie")) return OOXMLShapes::sptCActionButtonMovie;
			if (str == _T("actionButtonReturn")) return OOXMLShapes::sptCActionButtonReturn;
			if (str == _T("actionButtonSound")) return OOXMLShapes::sptCActionButtonSound;
			if (str == _T("arc")) return OOXMLShapes::sptCArc;
			break;
		case 'b':
			if (str == _T("bentArrow")) return OOXMLShapes::sptCBentArrow;
			if (str == _T("bentConnector2")) return OOXMLShapes::sptCBentConnector2;
			if (str == _T("bentConnector3")) return OOXMLShapes::sptCBentConnector3;
			if (str == _T("bentConnector4")) return OOXMLShapes::sptCBentConnector4;
			if (str == _T("bentConnector5")) return OOXMLShapes::sptCBentConnector5;
			if (str == _T("bentUpArrow")) return OOXMLShapes::sptCBentUpArrow;
			if (str == _T("bevel")) return OOXMLShapes::sptCBevel;
			if (str == _T("blockArc")) return OOXMLShapes::sptCBlockArc;
			if (str == _T("borderCallout1")) return OOXMLShapes::sptCBorderCallout1;
			if (str == _T("borderCallout2")) return OOXMLShapes::sptCBorderCallout2;
			if (str == _T("borderCallout3")) return OOXMLShapes::sptCBorderCallout3;
			if (str == _T("bracePair")) return OOXMLShapes::sptCBracePair;
			if (str == _T("bracketPair")) return OOXMLShapes::sptCBracketPair;
			break;
		case 'c':
			if (str == _T("callout1")) return OOXMLShapes::sptCCallout1;
			if (str == _T("callout2")) return OOXMLShapes::sptCCallout2;
			if (str == _T("callout3")) return OOXMLShapes::sptCCallout3;
			if (str == _T("can")) return OOXMLShapes::sptCCan;
			if (str == _T("chartPlus")) return OOXMLShapes::sptCChartPlus;
			if (str == _T("chartStar")) return OOXMLShapes::sptCChartStar;
			if (str == _T("chartX")) return OOXMLShapes::sptCChartX;
			if (str == _T("chevron")) return OOXMLShapes::sptCChevron;
			if (str == _T("chord")) return OOXMLShapes::sptCChord;
			if (str == _T("circularArrow")) return OOXMLShapes::sptCCircularArrow;
			if (str == _T("cloud")) return OOXMLShapes::sptCCloud;
			if (str == _T("cloudCallout")) return OOXMLShapes::sptCCloudCallout;
			if (str == _T("corner")) return OOXMLShapes::sptCCorner;
			if (str == _T("cornerTabs")) return OOXMLShapes::sptCCornerTabs;
			if (str == _T("cube")) return OOXMLShapes::sptCCube;
			if (str == _T("curvedConnector2")) return OOXMLShapes::sptCCurvedConnector2;
			if (str == _T("curvedConnector3")) return OOXMLShapes::sptCCurvedConnector3;
			if (str == _T("curvedConnector4")) return OOXMLShapes::sptCCurvedConnector4;
			if (str == _T("curvedConnector5")) return OOXMLShapes::sptCCurvedConnector5;
			if (str == _T("curvedDownArrow")) return OOXMLShapes::sptCCurvedDownArrow;
			if (str == _T("curvedLeftArrow")) return OOXMLShapes::sptCCurvedLeftArrow;
			if (str == _T("curvedRightArrow")) return OOXMLShapes::sptCCurvedRightArrow;
			if (str == _T("curvedUpArrow")) return OOXMLShapes::sptCCurvedUpArrow;
			break;
		case 'd':
			if (str == _T("decagon")) return OOXMLShapes::sptCDecagon;
			if (str == _T("diagStripe")) return OOXMLShapes::sptCDiagStripe;
			if (str == _T("diamond")) return OOXMLShapes::sptCDiamond;
			if (str == _T("dodecagon")) return OOXMLShapes::sptCDodecagon;
			if (str == _T("donut")) return OOXMLShapes::sptCDonut;
			if (str == _T("doubleWave")) return OOXMLShapes::sptCDoubleWave;
			if (str == _T("downArrow")) return OOXMLShapes::sptCDownArrow;
			if (str == _T("downArrowCallout")) return OOXMLShapes::sptCDownArrowCallout;
			break;
		case 'e':
			if (str == _T("ellipse")) return OOXMLShapes::sptCEllipse;
			if (str == _T("ellipseRibbon")) return OOXMLShapes::sptCEllipseRibbon;
			if (str == _T("ellipseRibbon2")) return OOXMLShapes::sptCEllipseRibbon2;
			break;
		case 'f':
			if (str == _T("flowChartAlternateProcess")) return OOXMLShapes::sptCFlowChartAlternateProcess;
			if (str == _T("flowChartCollate")) return OOXMLShapes::sptCFlowChartCollate;
			if (str == _T("flowChartConnector")) return OOXMLShapes::sptCFlowChartConnector;
			if (str == _T("flowChartDecision")) return OOXMLShapes::sptCFlowChartDecision;
			if (str == _T("flowChartDelay")) return OOXMLShapes::sptCFlowChartDelay;
			if (str == _T("flowChartDisplay")) return OOXMLShapes::sptCFlowChartDisplay;
			if (str == _T("flowChartDocument")) return OOXMLShapes::sptCFlowChartDocument;
			if (str == _T("flowChartExtract")) return OOXMLShapes::sptCFlowChartExtract;
			if (str == _T("flowChartInputOutput")) return OOXMLShapes::sptCFlowChartInputOutput;
			if (str == _T("flowChartInternalStorage")) return OOXMLShapes::sptCFlowChartInternalStorage;
			if (str == _T("flowChartMagneticDisk")) return OOXMLShapes::sptCFlowChartMagneticDisk;
			if (str == _T("flowChartMagneticDrum")) return OOXMLShapes::sptCFlowChartMagneticDrum;
			if (str == _T("flowChartMagneticTape")) return OOXMLShapes::sptCFlowChartMagneticTape;
			if (str == _T("flowChartManualInput")) return OOXMLShapes::sptCFlowChartManualInput;
			if (str == _T("flowChartManualOperation")) return OOXMLShapes::sptCFlowChartManualOperation;
			if (str == _T("flowChartMerge")) return OOXMLShapes::sptCFlowChartMerge;
			if (str == _T("flowChartMultidocument")) return OOXMLShapes::sptCFlowChartMultidocument;
			if (str == _T("flowChartOfflineStorage")) return OOXMLShapes::sptCFlowChartOfflineStorage;
			if (str == _T("flowChartOffpageConnector")) return OOXMLShapes::sptCFlowChartOffpageConnector;
			if (str == _T("flowChartOnlineStorage")) return OOXMLShapes::sptCFlowChartOnlineStorage;
			if (str == _T("flowChartOr")) return OOXMLShapes::sptCFlowChartOr;
			if (str == _T("flowChartPredefinedProcess")) return OOXMLShapes::sptCFlowChartPredefinedProcess;
			if (str == _T("flowChartPreparation")) return OOXMLShapes::sptCFlowChartPreparation;
			if (str == _T("flowChartProcess")) return OOXMLShapes::sptCFlowChartProcess;
			if (str == _T("flowChartPunchedCard")) return OOXMLShapes::sptCFlowChartPunchedCard;
			if (str == _T("flowChartPunchedTape")) return OOXMLShapes::sptCFlowChartPunchedTape;
			if (str == _T("flowChartSort")) return OOXMLShapes::sptCFlowChartSort;
			if (str == _T("flowChartSummingJunction")) return OOXMLShapes::sptCFlowChartSummingJunction;
			if (str == _T("flowChartTerminator")) return OOXMLShapes::sptCFlowChartTerminator;
			if (str == _T("foldedCorner")) return OOXMLShapes::sptCFoldedCorner;
			if (str == _T("frame")) return OOXMLShapes::sptCFrame;
			if (str == _T("funnel")) return OOXMLShapes::sptCFunnel;
			break;
		case 'g':
			if (str == _T("gear6")) return OOXMLShapes::sptCGear6;
			if (str == _T("gear9")) return OOXMLShapes::sptCGear9;
			break;
		case 'h':
			if (str == _T("halfFrame")) return OOXMLShapes::sptCHalfFrame;
			if (str == _T("heart")) return OOXMLShapes::sptCHeart;
			if (str == _T("heptagon")) return OOXMLShapes::sptCHeptagon;
			if (str == _T("hexagon")) return OOXMLShapes::sptCHexagon;
			if (str == _T("homePlate")) return OOXMLShapes::sptCHomePlate;
			if (str == _T("horizontalScroll")) return OOXMLShapes::sptCHorizontalScroll;
			break;
		case 'i':
			if (str == _T("irregularSeal1")) return OOXMLShapes::sptCIrregularSeal1;
			if (str == _T("irregularSeal2")) return OOXMLShapes::sptCIrregularSeal2;
			break;
		case 'l':
			if (str == _T("leftArrow")) return OOXMLShapes::sptCLeftArrow;
			if (str == _T("leftArrowCallout")) return OOXMLShapes::sptCLeftArrowCallout;
			if (str == _T("leftBrace")) return OOXMLShapes::sptCLeftBrace;
			if (str == _T("leftBracket")) return OOXMLShapes::sptCLeftBracket;
			if (str == _T("leftCircularArrow")) return OOXMLShapes::sptCLeftCircularArrow;
			if (str == _T("leftRightArrow")) return OOXMLShapes::sptCLeftRightArrow;
			if (str == _T("leftRightArrowCallout")) return OOXMLShapes::sptCLeftRightArrowCallout;
			if (str == _T("leftRightCircularArrow")) return OOXMLShapes::sptCLeftRightCircularArrow;
			if (str == _T("leftRightRibbon")) return OOXMLShapes::sptCLeftRightRibbon;
			if (str == _T("leftRightUpArrow")) return OOXMLShapes::sptCLeftRightUpArrow;
			if (str == _T("leftUpArrow")) return OOXMLShapes::sptCLeftUpArrow;
			if (str == _T("lightningBolt")) return OOXMLShapes::sptCLightningBolt;
			if (str == _T("line")) return OOXMLShapes::sptCLine;
			if (str == _T("lineInv")) return OOXMLShapes::sptCLineInv;
			break;
		case 'm':
			if (str == _T("mathDivide")) return OOXMLShapes::sptCMathDivide;
			if (str == _T("mathEqual")) return OOXMLShapes::sptCMathEqual;
			if (str == _T("mathMinus")) return OOXMLShapes::sptCMathMinus;
			if (str == _T("mathMultiply")) return OOXMLShapes::sptCMathMultiply;
			if (str == _T("mathNotEqual")) return OOXMLShapes::sptCMathNotEqual;
			if (str == _T("mathPlus")) return OOXMLShapes::sptCMathPlus;
			if (str == _T("moon")) return OOXMLShapes::sptCMoon;
			break;
		case 'n':
			if (str == _T("nonIsoscelesTrapezoid")) return OOXMLShapes::sptCNonIsoscelesTrapezoid;
			if (str == _T("noSmoking")) return OOXMLShapes::sptCNoSmoking;
			if (str == _T("notchedRightArrow")) return OOXMLShapes::sptCNotchedRightArrow;
			break;
		case 'o':
			if (str == _T("octagon")) return OOXMLShapes::sptCOctagon;
			break;
		case 'p':
			if (str == _T("parallelogram")) return OOXMLShapes::sptCParallelogram;
			if (str == _T("pentagon")) return OOXMLShapes::sptCPentagon;
			if (str == _T("pie")) return OOXMLShapes::sptCPie;
			if (str == _T("pieWedge")) return OOXMLShapes::sptCPieWedge;
			if (str == _T("plaque")) return OOXMLShapes::sptCPlaque;
			if (str == _T("plaqueTabs")) return OOXMLShapes::sptCPlaqueTabs;
			if (str == _T("plus")) return OOXMLShapes::sptCPlus;
			break;
		case 'q':
			if (str == _T("quadArrow")) return OOXMLShapes::sptCQuadArrow;
			if (str == _T("quadArrowCallout")) return OOXMLShapes::sptCQuadArrowCallout;
			break;
		case 'r':
			if (str == _T("rect")) return OOXMLShapes::sptCRect;
			if (str == _T("ribbon")) return OOXMLShapes::sptCRibbon;
			if (str == _T("ribbon2")) return OOXMLShapes::sptCRibbon2;
			if (str == _T("rightArrow")) return OOXMLShapes::sptCRightArrow;
			if (str == _T("rightArrowCallout")) return OOXMLShapes::sptCRightArrowCallout;
			if (str == _T("rightBrace")) return OOXMLShapes::sptCRightBrace;
			if (str == _T("rightBracket")) return OOXMLShapes::sptCRightBracket;
			if (str == _T("round1Rect")) return OOXMLShapes::sptCRound1Rect;
			if (str == _T("round2DiagRect")) return OOXMLShapes::sptCRound2DiagRect;
			if (str == _T("round2SameRect")) return OOXMLShapes::sptCRound2SameRect;
			if (str == _T("roundRect")) return OOXMLShapes::sptCRoundRect;
			if (str == _T("rtTriangle")) return OOXMLShapes::sptCRtTriangle;
			break;
		case 's':
			if (str == _T("smileyFace")) return OOXMLShapes::sptCSmileyFace;
			if (str == _T("snip1Rect")) return OOXMLShapes::sptCSnip1Rect;
			if (str == _T("snip2DiagRect")) return OOXMLShapes::sptCSnip2DiagRect;
			if (str == _T("snip2SameRect")) return OOXMLShapes::sptCSnip2SameRect;
			if (str == _T("snipRoundRect")) return OOXMLShapes::sptCSnipRoundRect;
			if (str == _T("squareTabs")) return OOXMLShapes::sptCSquareTabs;
			if (str == _T("star10")) return OOXMLShapes::sptCStar10;
			if (str == _T("star12")) return OOXMLShapes::sptCStar12;
			if (str == _T("star16")) return OOXMLShapes::sptCStar16;
			if (str == _T("star24")) return OOXMLShapes::sptCStar24;
			if (str == _T("star32")) return OOXMLShapes::sptCStar32;
			if (str == _T("star4")) return OOXMLShapes::sptCStar4;
			if (str == _T("star5")) return OOXMLShapes::sptCStar5;
			if (str == _T("star6")) return OOXMLShapes::sptCStar6;
			if (str == _T("star7")) return OOXMLShapes::sptCStar7;
			if (str == _T("star8")) return OOXMLShapes::sptCStar8;
			if (str == _T("straightConnector1")) return OOXMLShapes::sptCStraightConnector1;
			if (str == _T("stripedRightArrow")) return OOXMLShapes::sptCStripedRightArrow;
			if (str == _T("sun")) return OOXMLShapes::sptCSun;
			if (str == _T("swooshArrow")) return OOXMLShapes::sptCSwooshArrow;
			break;
		case 't':
			if (str == _T("teardrop")) return OOXMLShapes::sptCTeardrop;
			if (str == _T("trapezoid")) return OOXMLShapes::sptCTrapezoid;
			if (str == _T("triangle")) return OOXMLShapes::sptCTriangle;
			break;
		case 'u':
			if (str == _T("upArrow")) return OOXMLShapes::sptCUpArrow;
			if (str == _T("upArrowCallout")) return OOXMLShapes::sptCUpArrowCallout;
			if (str == _T("upDownArrow")) return OOXMLShapes::sptCUpDownArrow;
			if (str == _T("upDownArrowCallout")) return OOXMLShapes::sptCUpDownArrowCallout;
			if (str == _T("uturnArrow")) return OOXMLShapes::sptCUturnArrow;
			break;
		case 'v': 
			if (str == _T("verticalScroll")) return OOXMLShapes::sptCVerticalScroll;
			break;
		case 'w':
			if (str == _T("wave")) return OOXMLShapes::sptCWave;
			if (str == _T("wedgeEllipseCallout")) return OOXMLShapes::sptCWedgeEllipseCallout;
			if (str == _T("wedgeRectCallout")) return OOXMLShapes::sptCWedgeRectCallout;
			if (str == _T("wedgeRoundRectCallout")) return OOXMLShapes::sptCWedgeRoundRectCallout;
			break;
		}
		return OOXMLShapes::sptNil;
	}
}
