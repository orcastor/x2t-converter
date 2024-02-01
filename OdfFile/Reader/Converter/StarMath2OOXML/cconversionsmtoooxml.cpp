﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "cconversionsmtoooxml.h"
#include "../../../../DesktopEditor/common/File.h"
#include <fstream>
namespace StarMath {
	CConversionSMtoOOXML::CConversionSMtoOOXML(): m_pXmlWrite(nullptr)
	{
	}
	//check XMLWrite(if not nullptr == delete)
	void CConversionSMtoOOXML::StartConversion(std::vector<StarMath::CElement*> arPars)
	{
		m_pXmlWrite = new XmlUtils::CXmlWriter;
		m_pXmlWrite->WriteNodeBegin(L"m:oMathPara",false);
		m_pXmlWrite->WriteNodeBegin(L"m:oMath",false);
		for(CElement* oTempElement:arPars)
		{
			if(oTempElement != nullptr)
				oTempElement->ConversionToOOXML(m_pXmlWrite);
		}
		EndConversion();
	}
	void CConversionSMtoOOXML::StandartProperties(XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute)
	{
		if(pAttribute == nullptr)
		{
			//тут должны быть базовые свойства шрифта, задаваемые выше

			pXmlWrite->WriteNodeBegin(L"w:rPr",false);
			pXmlWrite->WriteNodeBegin(L"w:rFonts",true);
			pXmlWrite->WriteAttribute(L"w:hAnsi",L"Cambria Math");
			pXmlWrite->WriteAttribute(L"w:ascii",L"Cambria Math");
			pXmlWrite->WriteNodeEnd(L"w",true,true);
			pXmlWrite->WriteNodeBegin(L"w:sz",true);
			pXmlWrite->WriteAttribute(L"w:val",L"40");
			pXmlWrite->WriteNodeEnd(L"w",true,true);
			pXmlWrite->WriteNodeBegin(L"w:szCs",true);
			pXmlWrite->WriteAttribute(L"w:val",L"40");
			pXmlWrite->WriteNodeEnd(L"w",true,true);
			pXmlWrite->WriteNodeEnd(L"w:rPr",false,false);
		}
		else
		{
			pXmlWrite->WriteNodeBegin(L"w:rPr",false);
			pXmlWrite->WriteNodeBegin(L"w:rFonts",true);
			pXmlWrite->WriteAttribute(L"w:hAnsi",L"Cambria Math");
			pXmlWrite->WriteAttribute(L"w:ascii",L"Cambria Math");
			pXmlWrite->WriteNodeEnd(L"w",true,true);
			if(pAttribute->GetSize() == 0)
			{
				//тут должны быть базовые свойства шрифта, задаваемые выше - далее везде где не задано - аналогично
				pXmlWrite->WriteNodeBegin(L"w:sz",true);
				pXmlWrite->WriteAttribute(L"w:val",L"40");
				pXmlWrite->WriteNodeEnd(L"w",true,true);
				pXmlWrite->WriteNodeBegin(L"w:szCs",true);
				pXmlWrite->WriteAttribute(L"w:val",L"40");
				pXmlWrite->WriteNodeEnd(L"w",true,true);
			}
			else if(pAttribute->GetSize() != 0)
			{
				pXmlWrite->WriteNodeBegin(L"w:sz",true);
				pXmlWrite->WriteAttribute(L"w:val",std::to_wstring(pAttribute->GetSize()));
				pXmlWrite->WriteNodeEnd(L"w",true,true);
				pXmlWrite->WriteNodeBegin(L"w:szCs",true);
				pXmlWrite->WriteAttribute(L"w:val",std::to_wstring(pAttribute->GetSize()));
				pXmlWrite->WriteNodeEnd(L"w",true,true);
			}
			if(!pAttribute->EmptyColor())
			{
				pXmlWrite->WriteNodeBegin(L"w:color",true);
				pXmlWrite->WriteAttribute(L"w:val",pAttribute->GetColor());
				pXmlWrite->WriteNodeEnd(L"w",true,true);
			}
			if(pAttribute->GetBold() && pAttribute->GetItal())
			{
				WriteStyNode(pXmlWrite,L"bi");
			}
			else if(pAttribute->GetBold())
			{
				pXmlWrite->WriteNodeBegin(L"m:sty", true);
				pXmlWrite->WriteAttribute(L"m:val",L"b");
				pXmlWrite->WriteNodeEnd(L"w",true,true);
				pXmlWrite->WriteNodeBegin(L"w:b",true);
				pXmlWrite->WriteNodeEnd(L"w",true,true);
				pXmlWrite->WriteNodeBegin(L"w:bCs",true);
				pXmlWrite->WriteNodeEnd(L"w",true,true);
			}
			else if(pAttribute->GetItal())
			{
				pXmlWrite->WriteNodeBegin(L"w:i",true);
				pXmlWrite->WriteNodeEnd(L"w",true,true);
			}
			if(pAttribute->GetStrike())
			{
				pXmlWrite->WriteNodeBegin(L"w:strike",true);
				pXmlWrite->WriteNodeEnd(L"w",true,true);
			}
			pXmlWrite->WriteNodeEnd(L"w:rPr",false,false);
		}
	}
	void CConversionSMtoOOXML::PropertiesMFPR(bool bType, XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute)
	{
		pXmlWrite->WriteNodeBegin(L"m:fPr",false);
		if(bType)
		{
			pXmlWrite->WriteNodeBegin(L"m:type",true);
			pXmlWrite->WriteAttribute(L"m:val",L"lin");
			pXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		WriteCtrlPrNode(pXmlWrite,pAttribute);
		pXmlWrite->WriteNodeEnd(L"m:fPr",false,false);
	}
	void CConversionSMtoOOXML::PropertiesNaryPr(const TypeElement& enTypeOp,bool bEmptySub,bool bEmptySup,XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute)
	{
		pXmlWrite->WriteNodeBegin(L"m:naryPr",false);
		switch(enTypeOp)
		{
		case TypeElement::sum:
		{
		WriteChrNode(L"\u2211",pXmlWrite);
		break;
		}
		case TypeElement::prod:
		WriteChrNode(L"\u220F",pXmlWrite);
		break;
		case TypeElement::coprod:
		WriteChrNode(L"\u2210",pXmlWrite);
		break;
		case TypeElement::iint:
		WriteChrNode(L"\u222C",pXmlWrite);
		break;
		case TypeElement::iiint:
		WriteChrNode(L"\u222D",pXmlWrite);
		break;
		case TypeElement::lint:
		WriteChrNode(L"\u222E",pXmlWrite);
		break;
		case TypeElement::llint:
		WriteChrNode(L"\u222F",pXmlWrite);
		break;
		case TypeElement::lllint:
		WriteChrNode(L"\u2230",pXmlWrite);
		break;
		default:
		break;
		}
		WriteLimLocNode(L"undOvr",pXmlWrite);
		if(bEmptySub)
		{
			pXmlWrite->WriteNodeBegin(L"m:subHide",true);
			pXmlWrite->WriteAttribute(L"m:val",L"1");
			pXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		if(bEmptySup)
		{
			pXmlWrite->WriteNodeBegin(L"m:supHide",true);
			pXmlWrite->WriteAttribute(L"m:val",L"1");
			pXmlWrite->WriteNodeEnd(L"w",true,true);
		}
		WriteCtrlPrNode(pXmlWrite,pAttribute);
		pXmlWrite->WriteNodeEnd(L"m:naryPr",false,false);
	}
	void CConversionSMtoOOXML::BlockRecording(const std::wstring &wsNameBlock, CElement *pValueBlock,XmlUtils::CXmlWriter* pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(wsNameBlock,false);
		if(pValueBlock != nullptr)
			pValueBlock->ConversionToOOXML(pXmlWrite);
		pXmlWrite->WriteNodeEnd(wsNameBlock,false,false);
	}
	std::wstring CConversionSMtoOOXML::GetOOXML()
	{
		return m_pXmlWrite->GetXmlString();
	}
	void CConversionSMtoOOXML::EndConversion()
	{
		m_pXmlWrite->WriteNodeEnd(L"m:oMath",false,false);
		m_pXmlWrite->WriteNodeEnd(L"m:oMathPara",false,false);
	}
	void CConversionSMtoOOXML::PropertiesFuncPr(XmlUtils::CXmlWriter* pXmlWrite,CAttribute* pAttribute)
	{
		pXmlWrite->WriteNodeBegin(L"m:funcPr", false);
		pXmlWrite->WriteNodeBegin(L"m:ctrlPr", false);
		StandartProperties(pXmlWrite,pAttribute);
		pXmlWrite->WriteNodeEnd(L"m:ctrlPr", false,false);
		pXmlWrite->WriteNodeEnd(L"m:funcPr",false,false);
	}

	void CConversionSMtoOOXML::PropertiesDPr(XmlUtils::CXmlWriter *pXmlWrite, const TypeElement &enTypeBracket,CAttribute* pAttribute)
	{
		pXmlWrite->WriteNodeBegin(L"m:dPr",false);
		switch(enTypeBracket)
		{
			case TypeElement::langle:
				BracketTypeNotation(L"\u2329",L"\u232A",pXmlWrite);
				break;
			case TypeElement::square:
				BracketTypeNotation(L"\u005B",L"\u005D",pXmlWrite);
				break;
			case TypeElement::ldbracket:
				BracketTypeNotation(L"\u27E6",L"\u27E7",pXmlWrite);
				break;
			case TypeElement::lbrace:
				BracketTypeNotation(L"\u007B",L"\u007D",pXmlWrite);
				break;
			case TypeElement::lceil:
				BracketTypeNotation(L"\u23A1",L"\u23A4",pXmlWrite);
				break;
			case TypeElement::lfloor:
				BracketTypeNotation(L"\u23A3",L"\u23A6",pXmlWrite);
				break;
			case TypeElement::lline:
				BracketTypeNotation(L"\u23AA",L"\u23AA",pXmlWrite);
				break;
			case TypeElement::ldline:
				BracketTypeNotation(L"\u2016",L"\u2016",pXmlWrite);
				break;
		}
		pXmlWrite->WriteNodeBegin(L"m:ctrlPr");
		StandartProperties(pXmlWrite,pAttribute);
		pXmlWrite->WriteNodeEnd(L"m:ctrlPr",false,false);
		pXmlWrite->WriteNodeEnd(L"m:dPr",false,false);
	}
	void CConversionSMtoOOXML::BracketTypeNotation(const std::wstring &wsOpenBracket, const std::wstring &wsCloseBracket, XmlUtils::CXmlWriter *pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(L"m:begChr", true);
		pXmlWrite->WriteAttribute(L"m:val",wsOpenBracket);
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeBegin(L"m:endChr", true);
		pXmlWrite->WriteAttribute(L"m:val", wsCloseBracket);
		pXmlWrite->WriteNodeEnd(L"w",true,true);
	}
	void CConversionSMtoOOXML::PropertiesMPr(XmlUtils::CXmlWriter *pXmlWrite, const TypeElement &enTypeMatrix,CAttribute* pAttribute)
	{
		pXmlWrite->WriteNodeBegin(L"m:mPr",false);
		pXmlWrite->WriteNodeBegin(L"m:mcs",false);
		pXmlWrite->WriteNodeBegin(L"m:mc",false);
		pXmlWrite->WriteNodeBegin(L"m:mcPr",false);
		pXmlWrite->WriteNodeBegin(L"m:count",true);
		switch(enTypeMatrix)
		{
			case TypeElement::matrix:
				pXmlWrite->WriteAttribute(L"m:val",L"2");
				break;
			default:
				pXmlWrite->WriteAttribute(L"m:val",L"1");
				break;
		}
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeBegin(L"m:mcJc",true);
		pXmlWrite->WriteAttribute(L"m:val",L"center");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeEnd(L"m:mcPr",false,false);
		pXmlWrite->WriteNodeEnd(L"m:mc",false,false);
		pXmlWrite->WriteNodeEnd(L"m:mcs",false,false);
		pXmlWrite->WriteNodeBegin(L"m:ctrlPr",false);
		StandartProperties(pXmlWrite,pAttribute);
		pXmlWrite->WriteNodeEnd(L"m:ctrlPr",false,false);
		pXmlWrite->WriteNodeEnd(L"m:mPr",false,false);
	}
	void CConversionSMtoOOXML::NodeGrade(XmlUtils::CXmlWriter *pXmlWrite,CElement* pValueGrade,CAttribute* pAttribute)
	{
		pXmlWrite->WriteNodeBegin(L"m:d",false);
		pXmlWrite->WriteNodeBegin(L"m:dPr",false);
		pXmlWrite->WriteNodeBegin(L"m:begChr",true);
		pXmlWrite->WriteAttribute(L"m:val",L"");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeBegin(L"m:endChr",true);
		pXmlWrite->WriteAttribute(L"m:val",L"\u23AA");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeBegin(L"m:ctrlPr",false);
		StandartProperties(pXmlWrite,pAttribute);
		pXmlWrite->WriteNodeEnd(L"m:ctrlPr",false);
		pXmlWrite->WriteNodeEnd(L"m:dPr",false,false);
		pXmlWrite->WriteNodeBegin(L"m:e",false);
		pValueGrade->ConversionToOOXML(pXmlWrite);
		pXmlWrite->WriteNodeEnd(L"m:e",false,false);
		pXmlWrite->WriteNodeEnd(L"m:d",false,false);
	}
	void CConversionSMtoOOXML::WriteCtrlPrNode(XmlUtils::CXmlWriter *pXmlWrite, CAttribute *pAttribute)
	{
		pXmlWrite->WriteNodeBegin(L"m:ctrlPr",false);
		StandartProperties(pXmlWrite,pAttribute);
		pXmlWrite->WriteNodeEnd(L"m:ctrlPr",false,false);
	}
	void CConversionSMtoOOXML::WriteChrNode(const std::wstring &wsTypeOp,XmlUtils::CXmlWriter* pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(L"m:chr",true);
		pXmlWrite->WriteAttribute(L"m:val",wsTypeOp);
		pXmlWrite->WriteNodeEnd(L"w",true,true);
	}
	void CConversionSMtoOOXML::WriteLimLocNode(const std::wstring &wsTypeLimLock,XmlUtils::CXmlWriter* pXmlWrite)
	{
		pXmlWrite->WriteNodeBegin(L"m:limLoc",true);
		pXmlWrite->WriteAttribute(L"m:val",wsTypeLimLock);
		pXmlWrite->WriteNodeEnd(L"w",true,true);
	}
	void CConversionSMtoOOXML::WriteRPrFName(const TypeElement &enTypeOp, XmlUtils::CXmlWriter *pXmlWrite,CAttribute* pAttribute,const std::wstring& wsNameOp)
	{
		pXmlWrite->WriteNodeBegin(L"m:r",false);
		pXmlWrite->WriteNodeBegin(L"m:rPr",false);
		pXmlWrite->WriteNodeBegin(L"m:sty",true);
		pXmlWrite->WriteAttribute(L"m:val",L"p");
		pXmlWrite->WriteNodeEnd(L"w",true,true);
		pXmlWrite->WriteNodeEnd(L"m:rPr",false,false);
		CConversionSMtoOOXML::StandartProperties(pXmlWrite,pAttribute);
		pXmlWrite->WriteNodeBegin(L"m:t",false);
		switch(enTypeOp)
		{
		case TypeElement::lim:
		pXmlWrite->WriteString(L"lim");
		break;
		case TypeElement::liminf:
		pXmlWrite->WriteString(L"lim inf");
		break;
		case TypeElement::limsup:
		pXmlWrite->WriteString(L"lim sup");
		break;
		case TypeElement::oper:
		pXmlWrite->WriteString(wsNameOp);
		break;
		default:
		break;
		}
		pXmlWrite->WriteNodeEnd(L"m:t",false,false);
		pXmlWrite->WriteNodeEnd(L"m:r",false,false);
	}
	void CConversionSMtoOOXML::WriteStyNode(XmlUtils::CXmlWriter *pXmlWrite, const std::wstring &wsAttributeNode)
	{
		pXmlWrite->WriteNodeBegin(L"m:sty", true);
		pXmlWrite->WriteAttribute(L"m:val",wsAttributeNode);
		pXmlWrite->WriteNodeEnd(L"w",true,true);
	}
	void CConversionSMtoOOXML::WritePreserveBlock(XmlUtils::CXmlWriter *pXmlWrite,CAttribute* pAttribute)
	{
		pXmlWrite->WriteNodeBegin(L"m:r",false);
		StandartProperties(pXmlWrite,pAttribute);
		pXmlWrite->WriteNodeBegin(L"m:t",true);
		pXmlWrite->WriteAttribute(L"xml:space",L"preserve");
		pXmlWrite->WriteNodeEnd(L"w",true,false);
		pXmlWrite->WriteNodeEnd(L"m:t",false,false);
		pXmlWrite->WriteNodeEnd(L"m:r",false,false);
	}
}

