﻿/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#pragma once

#ifndef OOX_LOGIC_TABLE_PROPERTY_INCLUDE_H_
#define OOX_LOGIC_TABLE_PROPERTY_INCLUDE_H_

#include "../WritingElement.h"

#include "../../Common/ComplexTypes.h"

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// JcTable 17.4.29 (Part 1)
		//--------------------------------------------------------------------------------
		class CJcTable : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CJcTable)
			CJcTable()
			{
			}
			virtual ~CJcTable()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_oVal.IsInit() )
				{
					sResult += L"w:val=\"";
					sResult += m_oVal->ToString();
					sResult += L"\" ";
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CJcTable> m_oVal;
		};
		//--------------------------------------------------------------------------------
		// TblLayoutType 17.4.54 (Part 1)
		//--------------------------------------------------------------------------------
		class CTblLayoutType : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTblLayoutType)
			CTblLayoutType()
			{
			}
			virtual ~CTblLayoutType()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:type", m_oType );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_oType.IsInit() )
				{
					sResult += L"w:type=\"";
					sResult += m_oType->ToString();
					sResult += L"\" ";
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:type", m_oType )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CTblLayoutType> m_oType;
		};
		//--------------------------------------------------------------------------------
		// TblLook 17.4.56 (Part 1) + 9.3.11 (Part 4)
		//--------------------------------------------------------------------------------
		class CTblLook : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTblLook)
			CTblLook()
			{
			}
			virtual ~CTblLook()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:firstColumn", m_oFirstColumn );
				XmlMacroReadAttributeBase( oNode, L"w:firstRow",    m_oFirstRow );
				XmlMacroReadAttributeBase( oNode, L"w:lastColumn",  m_oLastColumn );
				XmlMacroReadAttributeBase( oNode, L"w:lastRow",     m_oLastRow );
				XmlMacroReadAttributeBase( oNode, L"w:noHBand",     m_oNoHBand );
				XmlMacroReadAttributeBase( oNode, L"w:noVBand",     m_oNoVBand );
				XmlMacroReadAttributeBase( oNode, L"w:val",         m_oVal );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_oFirstColumn.IsInit() )
				{
					sResult += L"w:firstColumn=\"";
					sResult += m_oFirstColumn->ToString();
					sResult += L"\" ";
				}

				if ( m_oFirstRow.IsInit() )
				{
					sResult += L"w:firstRow=\"";
					sResult += m_oFirstRow->ToString();
					sResult += L"\" ";
				}

				if ( m_oLastColumn.IsInit() )
				{
					sResult += L"w:lastColumn=\"";
					sResult += m_oLastColumn->ToString();
					sResult += L"\" ";
				}

				if ( m_oLastRow.IsInit() )
				{
					sResult += L"w:lastRow=\"";
					sResult += m_oLastRow->ToString();
					sResult += L"\" ";
				}

				if ( m_oNoHBand.IsInit() )
				{
					sResult += L"w:noHBand=\"";
					sResult += m_oNoHBand->ToString();
					sResult += L"\" ";
				}

				if ( m_oNoVBand.IsInit() )
				{
					sResult += L"w:noVBand=\"";
					sResult += m_oNoVBand->ToString();
					sResult += L"\" ";
				}

				ComplexTypes_WriteAttribute( L"w:val=\"", m_oVal );

				return sResult;
			}

			const bool IsFirstRow() const
			{
				if ( m_oFirstRow.IsInit() )
					return ( SimpleTypes::onoffTrue == m_oFirstRow->GetValue() );

				if ( m_oVal.IsInit() )
					return ( 0 != ( m_oVal->GetValue() & 0x0020 ) );

				return false;
			}
			const bool IsLastRow() const
			{
				if ( m_oLastRow.IsInit() )
					return ( SimpleTypes::onoffTrue == m_oLastRow->GetValue() );

				if ( m_oVal.IsInit() )
					return ( 0 != ( m_oVal->GetValue() & 0x0040 ) );

				return false;
			}
			const bool IsFirstColumn() const
			{
				if ( m_oFirstColumn.IsInit() )
					return ( SimpleTypes::onoffTrue == m_oFirstColumn->GetValue() );

				if ( m_oVal.IsInit() )
					return ( 0 != ( m_oVal->GetValue() & 0x0080 ) );

				return false;
			}
			const bool IsLastColumn () const
			{
				if ( m_oLastColumn.IsInit() )
					return ( SimpleTypes::onoffTrue == m_oLastColumn->GetValue() );

				if ( m_oVal.IsInit() )
					return ( 0 != ( m_oVal->GetValue() & 0x0100 ) );

				return false;
			}
			const bool IsNoHBand    () const
			{
				if ( m_oNoHBand.IsInit() )
					return ( SimpleTypes::onoffTrue == m_oNoHBand->GetValue() );

				if ( m_oVal.IsInit() )
					return ( 0 != ( m_oVal->GetValue() & 0x0200 ) );

				return false;
			}
			const bool IsNoVBand    () const
			{
				if ( m_oNoVBand.IsInit() )
					return ( SimpleTypes::onoffTrue == m_oNoVBand->GetValue() );

				if ( m_oVal.IsInit() )
					return ( 0 != ( m_oVal->GetValue() & 0x0400 ) );

				return false;
			}
			const int GetValue() const
			{
				int nRes = 0;
				if(m_oVal.IsInit())
					nRes = m_oVal->GetValue();
				else
				{
					if ( m_oNoVBand.IsInit() &&  SimpleTypes::onoffTrue == m_oNoVBand->GetValue())
						nRes |= 0x0400;
					if ( m_oNoHBand.IsInit() &&  SimpleTypes::onoffTrue == m_oNoHBand->GetValue())
						nRes |= 0x0200;
					if ( m_oLastColumn.IsInit() &&  SimpleTypes::onoffTrue == m_oLastColumn->GetValue())
						nRes |= 0x0100;
					if ( m_oFirstColumn.IsInit() &&  SimpleTypes::onoffTrue == m_oFirstColumn->GetValue())
						nRes |= 0x0080;
					if ( m_oLastRow.IsInit() &&  SimpleTypes::onoffTrue == m_oLastRow->GetValue())
						nRes |= 0x0040;
					if ( m_oFirstRow.IsInit() &&  SimpleTypes::onoffTrue == m_oFirstRow->GetValue())
						nRes |= 0x0020;
				}
				return nRes;
			}
			void SetValue(int nVal)
			{
				m_oVal.Init();
				m_oVal->SetValue(nVal);
				if ( 0 != (nVal & 0x0400))
				{
					m_oNoVBand.Init();
					m_oNoVBand->FromBool(true);
				}
				if ( 0 != (nVal & 0x0200))
				{
					m_oNoHBand.Init();
					m_oNoHBand->FromBool(true);
				}
				if ( 0 != (nVal & 0x0100))
				{
					m_oLastColumn.Init();
					m_oLastColumn->FromBool(true);
				}
				if ( 0 != (nVal & 0x0080))
				{
					m_oFirstColumn.Init();
					m_oFirstColumn->FromBool(true);
				}
				if ( 0 != (nVal & 0x0040))
				{
					m_oLastRow.Init();
					m_oLastRow->FromBool(true);
				}
				if ( 0 != (nVal & 0x0020))
				{
					m_oFirstRow.Init();
					m_oFirstRow->FromBool(true);
				}
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:firstColumn", m_oFirstColumn )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:firstRow",    m_oFirstRow )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:lastColumn",  m_oLastColumn )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:lastRow",     m_oLastRow )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:noHBand",     m_oNoHBand )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:noVBand",     m_oNoVBand )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:val",         m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:

			nullable<SimpleTypes::COnOff            > m_oFirstColumn;
			nullable<SimpleTypes::COnOff            > m_oFirstRow;
			nullable<SimpleTypes::COnOff            > m_oLastColumn;
			nullable<SimpleTypes::COnOff            > m_oLastRow;
			nullable<SimpleTypes::COnOff            > m_oNoHBand;
			nullable<SimpleTypes::COnOff            > m_oNoVBand;
			nullable<SimpleTypes::CShortHexNumber   > m_oVal;
		};
		//--------------------------------------------------------------------------------
		// TblOverlap 17.4.56 (Part 1)
		//--------------------------------------------------------------------------------
		class CTblOverlap : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTblOverlap)
			CTblOverlap()
			{
			}
			virtual ~CTblOverlap()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_oVal.IsInit() )
				{
					sResult += L"w:val=\"";
					sResult += m_oVal->ToString();
					sResult += L"\" ";
				}

				return sResult;
			}

		public:

			nullable<SimpleTypes::CTblOverlap> m_oVal;
		};
		//--------------------------------------------------------------------------------
		// TblPPr 17.4.58 (Part 1)
		//--------------------------------------------------------------------------------
		class CTblPPr : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTblPPr)
			CTblPPr()
			{
			}
			virtual ~CTblPPr()
			{
			}
			void PrepareAfterRead()
			{
				if(false == m_oHorzAnchor.IsInit())
				{
					m_oHorzAnchor.Init();
					m_oHorzAnchor->SetValue(SimpleTypes::hanchorText);
				}
				if(false == m_oVertAnchor.IsInit())
				{
					m_oVertAnchor.Init();
					m_oVertAnchor->SetValue(SimpleTypes::vanchorMargin);
				}
				if(false == m_oTblpXSpec.IsInit())
				{
					if(false == m_oTblpX.IsInit())
					{
						m_oTblpX.Init();
						m_oTblpX->FromTwips(0);
					}
					//Several values of sprmTDxaAbs have special meanings as specified by
					//[ECMA-376] Part 4, Section 2.18.114. These values are specified as
					//follows.
					switch(m_oTblpX->ToTwips())
					{
						case 0:
							m_oTblpX.reset(NULL);
							m_oTblpXSpec.Init();
							m_oTblpXSpec->SetValue(SimpleTypes::xalignLeft);
						break;
						case -4:
							m_oTblpX.reset(NULL);
							m_oTblpXSpec.Init();
							m_oTblpXSpec->SetValue(SimpleTypes::xalignCenter);
						break;
						case -8:
							m_oTblpX.reset(NULL);
							m_oTblpXSpec.Init();
							m_oTblpXSpec->SetValue(SimpleTypes::xalignRight);
						break;
						case -12:
							m_oTblpX.reset(NULL);
							m_oTblpXSpec.Init();
							m_oTblpXSpec->SetValue(SimpleTypes::xalignInside);
						break;
						case -16:
							m_oTblpX.reset(NULL);
							m_oTblpXSpec.Init();
							m_oTblpXSpec->SetValue(SimpleTypes::xalignOutside);
						break;
					}
				}
				if(false == m_oTblpYSpec.IsInit())
				{
					if(false == m_oTblpY.IsInit())
					{
						m_oTblpY.Init();
						m_oTblpY->FromTwips(0);
					}
					//The meanings that are provided correspond to
					//values that are defined in [ECMA-376] Part 4, Section 2.18.115 ST_YAlign
					//(Vertical Alignment Location).
					switch(m_oTblpY->ToTwips())
					{
						case 0:
							m_oVertAnchor.Init();
							m_oVertAnchor->SetValue(SimpleTypes::vanchorText);
						break;
						case -4:
							m_oTblpY.reset(NULL);
							m_oTblpYSpec.Init();
							m_oTblpYSpec->SetValue(SimpleTypes::yalignTop);
						break;
						case -8:
							m_oTblpY.reset(NULL);
							m_oTblpYSpec.Init();
							m_oTblpYSpec->SetValue(SimpleTypes::yalignCenter);
						break;
						case -12:
							m_oTblpY.reset(NULL);
							m_oTblpYSpec.Init();
							m_oTblpYSpec->SetValue(SimpleTypes::yalignBottom);
						break;
						case -16:
							m_oTblpY.reset(NULL);
							m_oTblpYSpec.Init();
							m_oTblpYSpec->SetValue(SimpleTypes::yalignInside);
						break;
						case -20:
							m_oTblpY.reset(NULL);
							m_oTblpYSpec.Init();
							m_oTblpYSpec->SetValue(SimpleTypes::yalignOutside);
						break;
					}
				}
			}
			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:bottomFromText", m_oBottomFromText );
				XmlMacroReadAttributeBase( oNode, L"w:horzAnchor",     m_oHorzAnchor );
				XmlMacroReadAttributeBase( oNode, L"w:leftFromText",   m_oLeftFromText );
				XmlMacroReadAttributeBase( oNode, L"w:rightFromText",  m_oRightFromText );
				XmlMacroReadAttributeBase( oNode, L"w:tblpX",          m_oTblpX );
				XmlMacroReadAttributeBase( oNode, L"w:tblpXSpec",      m_oTblpXSpec );
				XmlMacroReadAttributeBase( oNode, L"w:tblpY",          m_oTblpY );
				XmlMacroReadAttributeBase( oNode, L"w:tblpYSpec",      m_oTblpYSpec );
				XmlMacroReadAttributeBase( oNode, L"w:topFromText",    m_oTopFromText );
				XmlMacroReadAttributeBase( oNode, L"w:vertAnchor",     m_oVertAnchor );
				PrepareAfterRead();
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
				PrepareAfterRead();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				ComplexTypes_WriteAttribute( L"w:bottomFromText=\"", m_oBottomFromText );
				ComplexTypes_WriteAttribute( L"w:horzAnchor=\"",     m_oHorzAnchor );
				ComplexTypes_WriteAttribute( L"w:leftFromText=\"",   m_oLeftFromText );
				ComplexTypes_WriteAttribute( L"w:rightFromText=\"",  m_oRightFromText );
				ComplexTypes_WriteAttribute( L"w:tblpX=\"",          m_oTblpX );
				ComplexTypes_WriteAttribute( L"w:tblpXSpec=\"",      m_oTblpXSpec );
				ComplexTypes_WriteAttribute( L"w:tblpY=\"",          m_oTblpY );
				ComplexTypes_WriteAttribute( L"w:tblpYSpec=\"",      m_oTblpYSpec );
				ComplexTypes_WriteAttribute( L"w:topFromText=\"",    m_oTopFromText );
				ComplexTypes_WriteAttribute( L"w:vertAnchor=\"",     m_oVertAnchor );

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:bottomFromText", m_oBottomFromText )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:horzAnchor",     m_oHorzAnchor )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:leftFromText",   m_oLeftFromText )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:rightFromText",  m_oRightFromText )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:tblpX",          m_oTblpX )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:tblpXSpec",      m_oTblpXSpec )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:tblpY",          m_oTblpY )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:tblpYSpec",      m_oTblpYSpec )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:topFromText",    m_oTopFromText )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:vertAnchor",     m_oVertAnchor )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CTwipsMeasure       > m_oBottomFromText;
			nullable<SimpleTypes::CHAnchor            > m_oHorzAnchor;
			nullable<SimpleTypes::CTwipsMeasure       > m_oLeftFromText;
			nullable<SimpleTypes::CTwipsMeasure       > m_oRightFromText;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oTblpX;
			nullable<SimpleTypes::CXAlign             > m_oTblpXSpec;
			nullable<SimpleTypes::CSignedTwipsMeasure > m_oTblpY;
			nullable<SimpleTypes::CYAlign             > m_oTblpYSpec;
			nullable<SimpleTypes::CTwipsMeasure       > m_oTopFromText;
			nullable<SimpleTypes::CVAnchor            > m_oVertAnchor;

		};
	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// TblBorders 17.4.39 (Part 1) + 9.3.2 (Part 4) + 9.3.6 (Part 4)
		//--------------------------------------------------------------------------------
		class CTblBorders : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTblBorders)
			CTblBorders()
			{
			}
			virtual ~CTblBorders()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				if ( L"w:tblBorders" != oNode.GetName() )
					return;

				XmlUtils::CXmlNode oChild;

				if ( oNode.GetNode( L"w:bottom", oChild ) )  
					m_oBottom = oChild;

				if ( oNode.GetNode( L"w:end", oChild ) )  
					m_oEnd = oChild;

				if ( oNode.GetNode( L"w:insideH", oChild ) )  
					m_oInsideH = oChild;

				if ( oNode.GetNode( L"w:insideV", oChild ) )  
					m_oInsideV = oChild;

				if ( oNode.GetNode( L"w:start", oChild ) )  
					m_oStart = oChild;

				if ( oNode.GetNode( L"w:top", oChild ) )  
					m_oTop = oChild;

				if ( !m_oEnd.IsInit() && oNode.GetNode( L"w:right", oChild ) )
					m_oEnd = oChild;

				if ( !m_oStart.IsInit() && oNode.GetNode( L"w:left", oChild ) )
					m_oStart = oChild;
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( L"w:bottom" == sName )
						m_oBottom = oReader;
					else if ( L"w:end" == sName )
						m_oEnd = oReader;
					else if ( L"w:insideH" == sName )
						m_oInsideH = oReader;
					else if ( L"w:insideV" == sName )
						m_oInsideV = oReader;
					else if ( L"w:start" == sName )
						m_oStart = oReader;
					else if ( L"w:top" == sName )
						m_oTop = oReader;
					else if ( L"w:right" == sName )
						m_oEnd = oReader;
					else if ( L"w:left" == sName )
						m_oStart = oReader;
				}
			}
			virtual std::wstring toXML() const                     
			{
				std::wstring sResult = L"<w:tblBorders>";

				if ( m_oBottom.IsInit() )
				{
					sResult += L"<w:bottom ";
					sResult += m_oBottom->ToString();
					sResult += L"/>";						
				}

				if ( m_oEnd.IsInit() )
				{
					sResult += L"<w:end ";
					sResult += m_oEnd->ToString();
					sResult += L"/>";						
				}

				if ( m_oInsideH.IsInit() )
				{
					sResult += L"<w:insideH ";
					sResult += m_oInsideH->ToString();
					sResult += L"/>";						
				}

				if ( m_oInsideV.IsInit() )
				{
					sResult += L"<w:insideV ";
					sResult += m_oInsideV->ToString();
					sResult += L"/>";						
				}

				if ( m_oStart.IsInit() )
				{
					sResult += L"<w:start ";
					sResult += m_oStart->ToString();
					sResult += L"/>";						
				}

				if ( m_oTop.IsInit() )
				{
					sResult += L"<w:top ";
					sResult += m_oTop->ToString();
					sResult += L"/>";						
				}

				sResult += L"</w:tblBorders>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_tblBorders;
			}
			static const CTblBorders Merge(const CTblBorders& oPrev, const CTblBorders& oCurrent)
			{
				CTblBorders oProperties;
				oProperties.m_oBottom            = Merge( oPrev.m_oBottom,            oCurrent.m_oBottom );
				oProperties.m_oEnd            = Merge( oPrev.m_oEnd,            oCurrent.m_oEnd );
				oProperties.m_oInsideH            = Merge( oPrev.m_oInsideH,            oCurrent.m_oInsideH );
				oProperties.m_oInsideV            = Merge( oPrev.m_oInsideV,            oCurrent.m_oInsideV );
				oProperties.m_oStart            = Merge( oPrev.m_oStart,            oCurrent.m_oStart );
				oProperties.m_oTop            = Merge( oPrev.m_oTop,            oCurrent.m_oTop );

				return oProperties;
			}
			template<typename Type>
			static nullable<Type>     Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
			{
				nullable<Type> oResult;

				if ( oCurrent.IsInit() )
					oResult = oCurrent;
				else if ( oPrev.IsInit() )
					oResult = oPrev;

				return oResult;
			}
		public:

			nullable<ComplexTypes::Word::CBorder > m_oBottom;
			nullable<ComplexTypes::Word::CBorder > m_oEnd;
			nullable<ComplexTypes::Word::CBorder > m_oInsideH;
			nullable<ComplexTypes::Word::CBorder > m_oInsideV;
			nullable<ComplexTypes::Word::CBorder > m_oStart;
			nullable<ComplexTypes::Word::CBorder > m_oTop;

		};
		//--------------------------------------------------------------------------------
		// TblCellMar 17.4.43 (Part 1) + 9.3.4 (Part 4) + 9.3.8 (Part 4)
		//--------------------------------------------------------------------------------
		class CTblCellMar : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTblCellMar)
			CTblCellMar()
			{
			}
			virtual ~CTblCellMar()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				if ( L"w:tblCellMar" != oNode.GetName() )
					return;

				XmlUtils::CXmlNode oChild;

				if ( oNode.GetNode( L"w:bottom", oChild ) )  
					m_oBottom = oChild;

				if ( oNode.GetNode( L"w:end", oChild ) )  
					m_oEnd = oChild;

				if ( oNode.GetNode( L"w:start", oChild ) )  
					m_oStart = oChild;

				if ( oNode.GetNode( L"w:top", oChild ) )  
					m_oTop = oChild;

				if ( !m_oEnd.IsInit() && oNode.GetNode( L"w:right", oChild ) )
					m_oEnd = oChild;

				if ( !m_oStart.IsInit() && oNode.GetNode( L"w:left", oChild ) )
					m_oStart = oChild;
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( L"w:bottom" == sName )  
						m_oBottom = oReader;
					else if ( L"w:end" == sName )  
						m_oEnd = oReader;
					else if ( L"w:start" == sName )  
						m_oStart = oReader;
					else if ( L"w:top" == sName )  
						m_oTop = oReader;
					else if ( L"w:right" == sName )  
						m_oEnd = oReader;
					else if ( L"w:left" == sName )  
						m_oStart = oReader;
				}
			}
			virtual std::wstring      toXML() const                     
			{
				std::wstring sResult = L"<w:tblCellMar>";

				if ( m_oBottom.IsInit() )
				{
					sResult += L"<w:bottom ";
					sResult += m_oBottom->ToString();
					sResult += L"/>";						
				}

				if ( m_oEnd.IsInit() )
				{
					sResult += L"<w:end ";
					sResult += m_oEnd->ToString();
					sResult += L"/>";						
				}

				if ( m_oStart.IsInit() )
				{
					sResult += L"<w:start ";
					sResult += m_oStart->ToString();
					sResult += L"/>";						
				}

				if ( m_oTop.IsInit() )
				{
					sResult += L"<w:top ";
					sResult += m_oTop->ToString();
					sResult += L"/>";						
				}

				sResult += L"</w:tblCellMar>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_tblCellMar;
			}
			static const CTblCellMar Merge(const CTblCellMar& oPrev, const CTblCellMar& oCurrent)
			{
				CTblCellMar oProperties;
				oProperties.m_oBottom            = Merge( oPrev.m_oBottom,            oCurrent.m_oBottom );
				oProperties.m_oEnd            = Merge( oPrev.m_oEnd,            oCurrent.m_oEnd );
				oProperties.m_oStart            = Merge( oPrev.m_oStart,            oCurrent.m_oStart );
				oProperties.m_oTop            = Merge( oPrev.m_oTop,            oCurrent.m_oTop );

				return oProperties;
			}
			template<typename Type>
			static nullable<Type>     Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
			{
				nullable<Type> oResult;

				if ( oCurrent.IsInit() )
					oResult = oCurrent;
				else if ( oPrev.IsInit() )
					oResult = oPrev;

				return oResult;
			}
		public:

			nullable<ComplexTypes::Word::CTblWidth > m_oBottom;
			nullable<ComplexTypes::Word::CTblWidth > m_oEnd;
			nullable<ComplexTypes::Word::CTblWidth > m_oStart;
			nullable<ComplexTypes::Word::CTblWidth > m_oTop;

		};
		//--------------------------------------------------------------------------------
		// TblPrChange 17.13.5.34 (Part 1)
		//--------------------------------------------------------------------------------
		class CTableProperty;
		class CTblPrChange : public WritingElement
		{
		public:
			CTblPrChange(OOX::Document *pMain = NULL);
			CTblPrChange(XmlUtils::CXmlNode &oNode);
			CTblPrChange(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTblPrChange();
			const CTblPrChange& operator = (const XmlUtils::CXmlNode &oNode);
			const CTblPrChange& operator = (const XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			nullable<std::wstring>					m_sAuthor;
			nullable<SimpleTypes::CDateTime>		m_oDate;
			nullable<SimpleTypes::CDecimalNumber>	m_oId;
			nullable<std::wstring>					m_sUserId;

			nullable<CTableProperty>				m_pTblPr;
		};
		//--------------------------------------------------------------------------------
		// CTableProperty
		//--------------------------------------------------------------------------------
		class CTableProperty : public WritingElement
		{
		public:
			CTableProperty(OOX::Document *pMain = NULL)
			{
				m_bTblPrChange = false;
			}
			CTableProperty(XmlUtils::CXmlNode &oNode) : WritingElement(NULL)
			{
				m_bTblPrChange = false;

				fromXML( oNode );
			}
			CTableProperty(XmlUtils::CXmlLiteReader& oReader) : WritingElement(NULL)
			{
				m_bTblPrChange = false;

				fromXML( oReader );
			}
			virtual ~CTableProperty()
			{
			}
			const CTableProperty& operator=(const XmlUtils::CXmlNode &oNode)
			{
				fromXML( (XmlUtils::CXmlNode &)oNode );
				return *this;
			}
			const CTableProperty& operator=(const XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				if ( L"w:tblPr" != oNode.GetName() )
					return;

				XmlUtils::CXmlNode oChild;

				WritingElement_ReadNode( oNode, oChild, L"w:bidiVisual",          m_oBidiVisual );
				WritingElement_ReadNode( oNode, oChild, L"w:jc",                  m_oJc );
				WritingElement_ReadNode( oNode, oChild, L"w:shd",                 m_oShade );
				WritingElement_ReadNode( oNode, oChild, L"w:tblBorders",          m_oTblBorders );
				WritingElement_ReadNode( oNode, oChild, L"w:tblCaption",          m_oTblCaption );
				WritingElement_ReadNode( oNode, oChild, L"w:tblCellMar",          m_oTblCellMar );
				WritingElement_ReadNode( oNode, oChild, L"w:tblCellSpacing",      m_oTblCellSpacing );
				WritingElement_ReadNode( oNode, oChild, L"w:tblDescription",      m_oTblDescription );
				WritingElement_ReadNode( oNode, oChild, L"w:tblInd",              m_oTblInd );
				WritingElement_ReadNode( oNode, oChild, L"w:tblLayout",           m_oTblLayout );
				WritingElement_ReadNode( oNode, oChild, L"w:tblLook",             m_oTblLook );
				WritingElement_ReadNode( oNode, oChild, L"w:tblOverlap",          m_oTblOverlap );
				WritingElement_ReadNode( oNode, oChild, L"w:tblpPr",              m_oTblpPr );

				if ( !m_bTblPrChange )
					WritingElement_ReadNode( oNode, oChild, L"w:tblPrChange", m_oTblPrChange );

				WritingElement_ReadNode( oNode, oChild, L"w:tblStyle",            m_oTblStyle );
				WritingElement_ReadNode( oNode, oChild, L"w:tblStyleColBandSize", m_oTblStyleColBandSize );
				WritingElement_ReadNode( oNode, oChild, L"w:tblStyleRowBandSize", m_oTblStyleRowBandSize );
				WritingElement_ReadNode( oNode, oChild, L"w:tblW",                m_oTblW );
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if      ( L"w:bidiVisual"          == sName ) m_oBidiVisual = oReader;
					else if ( L"w:jc"                  == sName ) m_oJc = oReader;
					else if ( L"w:shd"                 == sName ) m_oShade = oReader;
					else if ( L"w:tblBorders"          == sName ) m_oTblBorders = oReader;
					else if ( L"w:tblCaption"          == sName ) m_oTblCaption = oReader;
					else if ( L"w:tblCellMar"          == sName ) m_oTblCellMar = oReader;
					else if ( L"w:tblCellSpacing"      == sName ) m_oTblCellSpacing = oReader;
					else if ( L"w:tblDescription"      == sName ) m_oTblDescription = oReader;
					else if ( L"w:tblInd"              == sName ) m_oTblInd = oReader;
					else if ( L"w:tblLayout"           == sName ) m_oTblLayout = oReader;
					else if ( L"w:tblLook"             == sName ) m_oTblLook = oReader;
					else if ( L"w:tblOverlap"          == sName ) m_oTblOverlap = oReader;
					else if ( L"w:tblpPr"              == sName ) m_oTblpPr = oReader;
					else if ( !m_bTblPrChange && L"w:tblPrChange" == sName ) m_oTblPrChange = oReader;
					else if ( L"w:tblStyle"            == sName ) m_oTblStyle = oReader;
					else if ( L"w:tblStyleColBandSize" == sName ) m_oTblStyleColBandSize = oReader;
					else if ( L"w:tblStyleRowBandSize" == sName ) m_oTblStyleRowBandSize = oReader;
					else if ( L"w:tblW"                == sName ) m_oTblW = oReader;
					else if ( L"w:tblPr" == sName)
					{//387.docx
						fromXML(oReader);
					}
				}
			}
			virtual std::wstring toXML() const                     
			{
				std::wstring sResult = L"<w:tblPr>";

				WritingElement_WriteNode_1( L"<w:bidiVisual ",          m_oBidiVisual );
				WritingElement_WriteNode_1( L"<w:jc ",                  m_oJc );
				WritingElement_WriteNode_1( L"<w:shd ",                 m_oShade );
				WritingElement_WriteNode_2( m_oTblBorders );
				WritingElement_WriteNode_1( L"<w:tblCaption ",          m_oTblCaption );
				WritingElement_WriteNode_2( m_oTblCellMar );
				WritingElement_WriteNode_1( L"<w:tblCellSpacing ",      m_oTblCellSpacing );
				WritingElement_WriteNode_1( L"<w:tblDescription ",      m_oTblDescription );
				WritingElement_WriteNode_1( L"<w:tblInd ",              m_oTblInd );
				WritingElement_WriteNode_1( L"<w:tblLayout ",           m_oTblLayout );
				WritingElement_WriteNode_1( L"<w:tblLook ",             m_oTblLook );
				WritingElement_WriteNode_1( L"<w:tblOverlap ",          m_oTblOverlap );
				WritingElement_WriteNode_1( L"<w:tblpPr ",              m_oTblpPr );

				if ( !m_bTblPrChange )
					WritingElement_WriteNode_2( m_oTblPrChange );

				WritingElement_WriteNode_1( L"<w:tblStyle ",            m_oTblStyle );
				WritingElement_WriteNode_1( L"<w:tblStyleColBandSize ", m_oTblStyleColBandSize );
				WritingElement_WriteNode_1( L"<w:tblStyleRowBandSize ", m_oTblStyleRowBandSize );
				WritingElement_WriteNode_1( L"<w:tblW ",                m_oTblW );

				sResult += L"</w:tblPr>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_tblPr;
			}
			static const CTableProperty Merge(const CTableProperty& oPrev, const CTableProperty& oCurrent)
			{
				CTableProperty oProperties;
				oProperties.m_bTblPrChange = oPrev.m_bTblPrChange || oCurrent.m_bTblPrChange;
				oProperties.m_oBidiVisual = Merge( oPrev.m_oBidiVisual, oCurrent.m_oBidiVisual );
				oProperties.m_oJc = Merge( oPrev.m_oJc, oCurrent.m_oJc );
				oProperties.m_oShade = Merge( oPrev.m_oShade, oCurrent.m_oShade );

				if ( oCurrent.m_oTblBorders.IsInit() && oPrev.m_oTblBorders.IsInit() )
					oProperties.m_oTblBorders = OOX::Logic::CTblBorders::Merge(oPrev.m_oTblBorders.get(), oCurrent.m_oTblBorders.get());
				else
					oProperties.m_oTblBorders = Merge( oPrev.m_oTblBorders, oCurrent.m_oTblBorders );

				oProperties.m_oTblCaption = Merge( oPrev.m_oTblCaption, oCurrent.m_oTblCaption );

				if ( oCurrent.m_oTblCellMar.IsInit() && oPrev.m_oTblCellMar.IsInit() )
					oProperties.m_oTblCellMar = OOX::Logic::CTblCellMar::Merge(oPrev.m_oTblCellMar.get(), oCurrent.m_oTblCellMar.get());
				else
					oProperties.m_oTblCellMar = Merge( oPrev.m_oTblCellMar,            oCurrent.m_oTblCellMar );

				oProperties.m_oTblCellSpacing = Merge( oPrev.m_oTblCellSpacing,            oCurrent.m_oTblCellSpacing );
				oProperties.m_oTblDescription = Merge( oPrev.m_oTblDescription,            oCurrent.m_oTblDescription );
				oProperties.m_oTblInd = Merge( oPrev.m_oTblInd,            oCurrent.m_oTblInd );
				oProperties.m_oTblLayout = Merge( oPrev.m_oTblLayout,            oCurrent.m_oTblLayout );
				oProperties.m_oTblLook = Merge( oPrev.m_oTblLook,            oCurrent.m_oTblLook );
				oProperties.m_oTblOverlap            = Merge( oPrev.m_oTblOverlap,            oCurrent.m_oTblOverlap );
				oProperties.m_oTblpPr            = Merge( oPrev.m_oTblpPr,            oCurrent.m_oTblpPr );
				oProperties.m_oTblPrChange            = Merge( oPrev.m_oTblPrChange,            oCurrent.m_oTblPrChange );
				oProperties.m_oTblStyle            = Merge( oPrev.m_oTblStyle,            oCurrent.m_oTblStyle );
				oProperties.m_oTblStyleColBandSize            = Merge( oPrev.m_oTblStyleColBandSize,            oCurrent.m_oTblStyleColBandSize );
				oProperties.m_oTblStyleRowBandSize            = Merge( oPrev.m_oTblStyleRowBandSize,            oCurrent.m_oTblStyleRowBandSize );
				oProperties.m_oTblW            = Merge( oPrev.m_oTblW,            oCurrent.m_oTblW );
				return oProperties;
			}
			template<typename Type>
			static nullable<Type> Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
			{
				nullable<Type> oResult;

				if ( oCurrent.IsInit() )
					oResult = oCurrent;
				else if ( oPrev.IsInit() )
					oResult = oPrev;

				return oResult;
			}
		public:

			bool                                                           m_bTblPrChange;

			nullable<ComplexTypes::Word::COnOff2 > m_oBidiVisual;
			nullable<ComplexTypes::Word::CJcTable                        > m_oJc;
			nullable<ComplexTypes::Word::CShading                        > m_oShade;
			nullable<OOX::Logic::CTblBorders                             > m_oTblBorders;
			nullable<ComplexTypes::Word::String                        > m_oTblCaption;
			nullable<OOX::Logic::CTblCellMar                             > m_oTblCellMar;
			nullable<ComplexTypes::Word::CTblWidth                       > m_oTblCellSpacing;
			nullable<ComplexTypes::Word::String                        > m_oTblDescription;
			nullable<ComplexTypes::Word::CTblWidth                       > m_oTblInd;
			nullable<ComplexTypes::Word::CTblLayoutType                  > m_oTblLayout;
			nullable<ComplexTypes::Word::CTblLook                        > m_oTblLook;
			nullable<ComplexTypes::Word::CTblOverlap                     > m_oTblOverlap;
			nullable<ComplexTypes::Word::CTblPPr                         > m_oTblpPr;
			nullable<OOX::Logic::CTblPrChange                            > m_oTblPrChange;
			nullable<ComplexTypes::Word::String                        > m_oTblStyle;
			nullable<ComplexTypes::Word::CDecimalNumber                  > m_oTblStyleColBandSize;
			nullable<ComplexTypes::Word::CDecimalNumber                  > m_oTblStyleRowBandSize;
			nullable<ComplexTypes::Word::CTblWidth                       > m_oTblW;

		};
	} // namespace Logic
} // namespace OOX

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// Height 17.4.81 (Part 1)
		//--------------------------------------------------------------------------------
		class CHeight : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CHeight)
			CHeight()
			{
			}
			virtual ~CHeight()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:hRule", m_oHRule );
				XmlMacroReadAttributeBase( oNode, L"w:val",   m_oVal );
				XmlMacroReadAttributeBase(oNode, L"w:h-rule", m_oHRule);
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				ComplexTypes_WriteAttribute( L"w:hRule=\"", m_oHRule );
				ComplexTypes_WriteAttribute( L"w:val=\"",   m_oVal );

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if ( oReader, L"w:hRule", m_oHRule )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_Read_else_if(oReader, L"w:h-rule", m_oHRule)
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CHeightRule >		m_oHRule;
			nullable<SimpleTypes::CTwipsMeasure >	m_oVal;
		};

	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// TrPrChange 17.13.5.37 (Part 1)
		//--------------------------------------------------------------------------------
		class CTableRowProperties;
		class CTrPrChange : public WritingElement
		{
		public:
			CTrPrChange(OOX::Document *pMain = NULL);
			CTrPrChange(XmlUtils::CXmlNode &oNode);
			CTrPrChange(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTrPrChange();
			const CTrPrChange& operator = (const XmlUtils::CXmlNode &oNode);
			const CTrPrChange& operator = (const XmlUtils::CXmlLiteReader& oReader);
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			// Attributes
			nullable<std::wstring>					m_sAuthor;
			nullable<SimpleTypes::CDateTime>		m_oDate;
			nullable<SimpleTypes::CDecimalNumber>	m_oId;
			nullable<std::wstring>					m_sUserId;

			// Childs
			nullable<CTableRowProperties>           m_pTrPr;
		};
		//--------------------------------------------------------------------------------
		// CTableRowProperties
		//--------------------------------------------------------------------------------
		class CTableRowProperties : public WritingElement
		{
		public:
			CTableRowProperties(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
				m_bTrPrChange = false;
			}
			CTableRowProperties(XmlUtils::CXmlNode &oNode) : WritingElement(NULL)
			{
				m_bTrPrChange = false;

				fromXML( (XmlUtils::CXmlNode &)oNode );
			}
			CTableRowProperties(XmlUtils::CXmlLiteReader& oReader) : WritingElement(NULL)
			{
				m_bTrPrChange = false;

				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			}
			virtual ~CTableRowProperties()
			{
			}

			const CTableRowProperties& operator =(const XmlUtils::CXmlNode &oNode)
			{
				fromXML( (XmlUtils::CXmlNode &)oNode );
				return *this;
			}
			const CTableRowProperties& operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				if ( L"w:trPr" != oNode.GetName() )
					return;

				XmlUtils::CXmlNode oChild;

				WritingElement_ReadNode( oNode, oChild, L"w:cantSplit",      m_oCantSplit );
				WritingElement_ReadNode( oNode, oChild, L"w:cnfStyle",       m_oCnfStyle );
				WritingElement_ReadNode( oNode, oChild, L"w:del",            m_oDel );
				WritingElement_ReadNode( oNode, oChild, L"w:divId",          m_oDivId );
				WritingElement_ReadNode( oNode, oChild, L"w:gridAfter",      m_oGridAfter );
				WritingElement_ReadNode( oNode, oChild, L"w:gridBefore",     m_oGridBefore );
				WritingElement_ReadNode( oNode, oChild, L"w:hidden",         m_oHidden );
				WritingElement_ReadNode( oNode, oChild, L"w:ins",            m_oIns );
				WritingElement_ReadNode( oNode, oChild, L"w:jc",             m_oJc );
				WritingElement_ReadNode( oNode, oChild, L"w:tblCellSpacing", m_oTblCellSpacing );
				WritingElement_ReadNode( oNode, oChild, L"w:tblHeader",      m_oTblHeader );
				WritingElement_ReadNode( oNode, oChild, L"w:trHeight",       m_oTblHeight );

				if ( !m_bTrPrChange )
					WritingElement_ReadNode( oNode, oChild, L"w:trPrChange", m_oTrPrChange );

				WritingElement_ReadNode( oNode, oChild, L"w:wAfter",         m_oWAfter );
				WritingElement_ReadNode( oNode, oChild, L"w:wBefore",        m_oWBefore );
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if      ( L"w:cantSplit"      == sName ) m_oCantSplit = oReader;
					else if ( L"w:cnfStyle"       == sName ) m_oCnfStyle = oReader;
					else if ( L"w:del"            == sName ) m_oDel = oReader;
					else if ( L"w:divId"          == sName ) m_oDivId = oReader;
					else if ( L"w:gridAfter"      == sName ) m_oGridAfter = oReader;
					else if ( L"w:gridBefore"     == sName ) m_oGridBefore = oReader;
					else if ( L"w:hidden"         == sName ) m_oHidden = oReader;
					else if ( L"w:ins"            == sName ) m_oIns = oReader;
					else if ( L"w:jc"             == sName ) m_oJc = oReader;
					else if ( L"w:tblCellSpacing" == sName ) m_oTblCellSpacing = oReader;
					else if ( L"w:tblHeader"      == sName ) m_oTblHeader = oReader;
					else if ( L"w:trHeight"       == sName ) m_oTblHeight = oReader;
					else if ( !m_bTrPrChange && L"w:trPrChange" == sName ) m_oTrPrChange = oReader;
					else if ( L"w:wAfter"         == sName ) m_oWAfter = oReader;
					else if ( L"w:wBefore"        == sName ) m_oWBefore = oReader;
				}
			}
			virtual std::wstring      toXML() const
			{
				std::wstring sResult = L"<w:trPr>";

				WritingElement_WriteNode_1( L"<w:cantSplit ",      m_oCantSplit );
				WritingElement_WriteNode_1( L"<w:cnfStyle ",       m_oCnfStyle );
				WritingElement_WriteNode_1( L"<w:del ",            m_oDel );
				WritingElement_WriteNode_1( L"<w:divId ",          m_oDivId );
				WritingElement_WriteNode_1( L"<w:gridAfter ",      m_oGridAfter );
				WritingElement_WriteNode_1( L"<w:gridBefore ",     m_oGridBefore );
				WritingElement_WriteNode_1( L"<w:hidden ",         m_oHidden );
				WritingElement_WriteNode_1( L"<w:ins ",            m_oIns );
				WritingElement_WriteNode_1( L"<w:jc ",             m_oJc );
				WritingElement_WriteNode_1( L"<w:tblCellSpacing ", m_oTblCellSpacing );
				WritingElement_WriteNode_1( L"<w:tblHeader ",      m_oTblHeader );
				WritingElement_WriteNode_1( L"<w:trHeight ",       m_oTblHeight );

				if ( !m_bTrPrChange )
					WritingElement_WriteNode_2( m_oTrPrChange );

				WritingElement_WriteNode_1( L"<w:wAfter ",         m_oWAfter );
				WritingElement_WriteNode_1( L"<w:wBefore ",        m_oWBefore );

				sResult += L"</w:trPr>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_trPr;
			}
			static const CTableRowProperties Merge(const CTableRowProperties& oPrev, const CTableRowProperties& oCurrent)
			{
				CTableRowProperties oProperties;

				oProperties.m_bTrPrChange            = oPrev.m_bTrPrChange || oCurrent.m_bTrPrChange;
				oProperties.m_oCantSplit            = Merge( oPrev.m_oCantSplit,            oCurrent.m_oCantSplit );
				oProperties.m_oCnfStyle            = Merge( oPrev.m_oCnfStyle,            oCurrent.m_oCnfStyle );
				oProperties.m_oDel            = Merge( oPrev.m_oDel,            oCurrent.m_oDel );
				oProperties.m_oDivId            = Merge( oPrev.m_oDivId,            oCurrent.m_oDivId );
				oProperties.m_oGridAfter            = Merge( oPrev.m_oGridAfter,            oCurrent.m_oGridAfter );
				oProperties.m_oGridBefore            = Merge( oPrev.m_oGridBefore,            oCurrent.m_oGridBefore );
				oProperties.m_oHidden            = Merge( oPrev.m_oHidden,            oCurrent.m_oHidden );
				oProperties.m_oIns            = Merge( oPrev.m_oIns,            oCurrent.m_oIns );
				oProperties.m_oJc            = Merge( oPrev.m_oJc,            oCurrent.m_oJc );
				oProperties.m_oTblCellSpacing            = Merge( oPrev.m_oTblCellSpacing,            oCurrent.m_oTblCellSpacing );
				oProperties.m_oTblHeader            = Merge( oPrev.m_oTblHeader,            oCurrent.m_oTblHeader );
				oProperties.m_oTblHeight            = Merge( oPrev.m_oTblHeight,            oCurrent.m_oTblHeight );
				oProperties.m_oTrPrChange            = Merge( oPrev.m_oTrPrChange,            oCurrent.m_oTrPrChange );
				oProperties.m_oWAfter            = Merge( oPrev.m_oWAfter,            oCurrent.m_oWAfter );
				oProperties.m_oWBefore            = Merge( oPrev.m_oWBefore,            oCurrent.m_oWBefore );

				return oProperties;
			}
			template<typename Type>
			static nullable<Type> Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
			{
				nullable<Type> oResult;

				if ( oCurrent.IsInit() )
					oResult = oCurrent;
				else if ( oPrev.IsInit() )
					oResult = oPrev;

				return oResult;
			}
		public:

			bool                                                           m_bTrPrChange;

			nullable<ComplexTypes::Word::COnOff2 > m_oCantSplit;
			nullable<ComplexTypes::Word::CCnf                            > m_oCnfStyle;
			nullable<ComplexTypes::Word::CTrackChange                    > m_oDel;
			nullable<ComplexTypes::Word::CDecimalNumber                  > m_oDivId;
			nullable<ComplexTypes::Word::CDecimalNumber                  > m_oGridAfter;
			nullable<ComplexTypes::Word::CDecimalNumber                  > m_oGridBefore;
			nullable<ComplexTypes::Word::COnOff2 > m_oHidden;
			nullable<ComplexTypes::Word::CTrackChange                    > m_oIns;
			nullable<ComplexTypes::Word::CJcTable                        > m_oJc;
			nullable<ComplexTypes::Word::CTblWidth                       > m_oTblCellSpacing;
			nullable<ComplexTypes::Word::COnOff2 > m_oTblHeader;
			nullable<ComplexTypes::Word::CHeight                         > m_oTblHeight;
			nullable<OOX::Logic::CTrPrChange                             > m_oTrPrChange;
			nullable<ComplexTypes::Word::CTblWidth                       > m_oWAfter;
			nullable<ComplexTypes::Word::CTblWidth                       > m_oWBefore;
		};

	} // namespace Logic
} // namespace OOX

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// CellMergeTrackChange 17.13.5.3 (Part 1)
		//--------------------------------------------------------------------------------
		class CCellMergeTrackChange : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CCellMergeTrackChange)
			CCellMergeTrackChange()
			{
			}
			virtual ~CCellMergeTrackChange()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:author",     m_sAuthor );
				XmlMacroReadAttributeBase( oNode, L"w:date",       m_oDate );
				XmlMacroReadAttributeBase( oNode, L"w:id",         m_oId );
				XmlMacroReadAttributeBase( oNode, L"w:vMerge",     m_oVMerge );
				XmlMacroReadAttributeBase( oNode, L"w:vMergeOrig", m_oVMergeOrig );
				XmlMacroReadAttributeBase( oNode, L"oouserid",     m_sUserId );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				if ( m_sAuthor.IsInit() )
				{
					sResult += L"w:author=\"";
                    sResult += m_sAuthor.get2();
					sResult += L"\" ";
				}

				ComplexTypes_WriteAttribute( L"w:date=\"",       m_oDate );
				ComplexTypes_WriteAttribute( L"w:id=\"",         m_oId );
				ComplexTypes_WriteAttribute( L"w:vMerge=\"",     m_oVMerge );
				ComplexTypes_WriteAttribute( L"w:vMergeOrig=\"", m_oVMergeOrig );

				if ( m_sUserId.IsInit() )
				{
					sResult += L"oouserid=\"";
                    sResult += m_sUserId.get2();
					sResult += L"\" ";
				}

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:author",     m_sAuthor )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:date",       m_oDate )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",         m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:vMerge",     m_oVMerge )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:vMergeOrig", m_oVMergeOrig )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"oouserid",     m_sUserId )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<std::wstring                     > m_sAuthor;
			nullable<SimpleTypes::CDateTime           > m_oDate;
			nullable<SimpleTypes::CDecimalNumber      > m_oId;
			nullable<SimpleTypes::CAnnotationVMerge   > m_oVMerge;
			nullable<SimpleTypes::CAnnotationVMerge   > m_oVMergeOrig;
			nullable<std::wstring                     > m_sUserId;

		};
		//--------------------------------------------------------------------------------
		// HMerge 17.4.22 (Part 1)
		//--------------------------------------------------------------------------------
		class CHMerge : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CHMerge)
			CHMerge()
			{
			}
			virtual ~CHMerge()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				ComplexTypes_WriteAttribute( L"w:val=\"", m_oVal );

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CMerge> m_oVal;

		};

		//--------------------------------------------------------------------------------
		// VMerge 17.4.85 (Part 1)
		//--------------------------------------------------------------------------------
		class CVMerge : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CVMerge)
			CVMerge()
			{
			}
			virtual ~CVMerge()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"w:val", m_oVal );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual std::wstring ToString() const
			{
				std::wstring sResult;

				ComplexTypes_WriteAttribute( L"w:val=\"", m_oVal );

				return sResult;
			}
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"w:val", m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			nullable<SimpleTypes::CMerge> m_oVal;

		};

	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// Headers 17.13.1.19 (Part 1)
		//--------------------------------------------------------------------------------
		class CHeaders : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CHeaders)

			CHeaders(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CHeaders()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrHeaders.size(); nIndex++ )
				{
					if ( m_arrHeaders[nIndex] ) delete m_arrHeaders[nIndex];
					m_arrHeaders[nIndex] = NULL;
				}
				m_arrHeaders.clear();
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				if ( L"w:headers" != oNode.GetName() )
					return;

				XmlUtils::CXmlNodes oHeaders;

				if ( oNode.GetNodes( L"w:header", oHeaders ) )
				{
					XmlUtils::CXmlNode oHeader;
					for ( int nIndex = 0; nIndex < oHeaders.GetCount(); nIndex++ )
					{
						if ( oHeaders.GetAt( nIndex, oHeader ) )
						{
							ComplexTypes::Word::String *oHead = new ComplexTypes::Word::String(oHeader);
							if (oHead) m_arrHeaders.push_back( oHead );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if ( L"w:header" == sName )
					{
						ComplexTypes::Word::String *oHead = new ComplexTypes::Word::String(oReader);
						if (oHead) m_arrHeaders.push_back( oHead );
					}
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = L"<w:headers>";

				for (unsigned int nIndex = 0; nIndex < m_arrHeaders.size(); nIndex++ )
				{		
					sResult += L"<w:header ";
					if (m_arrHeaders[nIndex])
						sResult += m_arrHeaders[nIndex]->ToString();
					sResult += L"/>";
				}

				sResult += L"</w:headers>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_headers;
			}
			std::vector<ComplexTypes::Word::String *> m_arrHeaders;
		};

		//--------------------------------------------------------------------------------
		// TcBorders 17.4.67 (Part 1) + 9.3.1 (Part 4) + 9.3.5 (Part 4)
		//--------------------------------------------------------------------------------
		class CTcBorders : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTcBorders)
			CTcBorders()
			{
			}
			virtual ~CTcBorders()
			{
			}

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				if ( L"w:tcBorders" != oNode.GetName() )
					return;

				XmlUtils::CXmlNode oChild;

				WritingElement_ReadNode( oNode, oChild, L"w:bottom",  m_oBottom );
				WritingElement_ReadNode( oNode, oChild, L"w:end",     m_oEnd );
				WritingElement_ReadNode( oNode, oChild, L"w:insideH", m_oInsideH );
				WritingElement_ReadNode( oNode, oChild, L"w:insideV", m_oInsideV );
				WritingElement_ReadNode( oNode, oChild, L"w:start",   m_oStart );
				WritingElement_ReadNode( oNode, oChild, L"w:tl2br",   m_oTL2BR );
				WritingElement_ReadNode( oNode, oChild, L"w:top",     m_oTop );
				WritingElement_ReadNode( oNode, oChild, L"w:tr2bl",   m_oTR2BL );

				if ( !m_oEnd.IsInit() )
					WritingElement_ReadNode( oNode, oChild, L"w:right", m_oEnd );

				if ( !m_oStart.IsInit() )
					WritingElement_ReadNode( oNode, oChild, L"w:left",  m_oStart );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if      ( L"w:bottom"  == sName ) m_oBottom = oReader;
					else if ( L"w:end"     == sName ) m_oEnd = oReader;
					else if ( L"w:insideH" == sName ) m_oInsideH = oReader;
					else if ( L"w:insideV" == sName ) m_oInsideV = oReader;
					else if ( L"w:start"   == sName ) m_oStart = oReader;
					else if ( L"w:tl2br"   == sName ) m_oTL2BR = oReader;
					else if ( L"w:top"     == sName ) m_oTop = oReader;
					else if ( L"w:tr2bl"   == sName ) m_oTR2BL = oReader;
					else if ( !m_oEnd.IsInit()   && L"w:right" == sName ) m_oEnd = oReader;
					else if ( !m_oStart.IsInit() && L"w:left"  == sName ) m_oStart = oReader;
				}
			}
			virtual std::wstring      toXML() const
			{
				std::wstring sResult = L"<w:tcBorders>";

				WritingElement_WriteNode_1( L"<w:bottom ",  m_oBottom );
				WritingElement_WriteNode_1( L"<w:end ",     m_oEnd );
				WritingElement_WriteNode_1( L"<w:insideH ", m_oInsideH );
				WritingElement_WriteNode_1( L"<w:insideV ", m_oInsideV );
				WritingElement_WriteNode_1( L"<w:start ",   m_oStart );
				WritingElement_WriteNode_1( L"<w:tl2br ",   m_oTL2BR );
				WritingElement_WriteNode_1( L"<w:top ",     m_oTop );
				WritingElement_WriteNode_1( L"<w:tr2bl ",   m_oTR2BL );

				sResult += L"</w:tcBorders>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_tcBorders;
			}
			static const CTcBorders Merge(const CTcBorders& oPrev, const CTcBorders& oCurrent)
			{
				CTcBorders oProperties;
				oProperties.m_oBottom            = Merge( oPrev.m_oBottom,            oCurrent.m_oBottom );
				oProperties.m_oEnd            = Merge( oPrev.m_oEnd,            oCurrent.m_oEnd );
				oProperties.m_oInsideH            = Merge( oPrev.m_oInsideH,            oCurrent.m_oInsideH );
				oProperties.m_oInsideV            = Merge( oPrev.m_oInsideV,            oCurrent.m_oInsideV );
				oProperties.m_oStart            = Merge( oPrev.m_oStart,            oCurrent.m_oStart );
				oProperties.m_oTL2BR            = Merge( oPrev.m_oTL2BR,            oCurrent.m_oTL2BR );
				oProperties.m_oTop            = Merge( oPrev.m_oTop,            oCurrent.m_oTop );
				oProperties.m_oTR2BL            = Merge( oPrev.m_oTR2BL,            oCurrent.m_oTR2BL );

				return oProperties;
			}
			template<typename Type>
			static nullable<Type>     Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
			{
				nullable<Type> oResult;

				if ( oCurrent.IsInit() )
					oResult = oCurrent;
				else if ( oPrev.IsInit() )
					oResult = oPrev;

				return oResult;
			}
		public:

			nullable<ComplexTypes::Word::CBorder > m_oBottom;
			nullable<ComplexTypes::Word::CBorder > m_oEnd;
			nullable<ComplexTypes::Word::CBorder > m_oInsideH;
			nullable<ComplexTypes::Word::CBorder > m_oInsideV;
			nullable<ComplexTypes::Word::CBorder > m_oStart;
			nullable<ComplexTypes::Word::CBorder > m_oTL2BR;
			nullable<ComplexTypes::Word::CBorder > m_oTop;
			nullable<ComplexTypes::Word::CBorder > m_oTR2BL;
		};

		//--------------------------------------------------------------------------------
		// TcMar 17.4.69 (Part 1) + 9.3.3 (Part 4) + 9.3.7 (Part 4)
		//--------------------------------------------------------------------------------
		class CTcMar : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTcMar)
			CTcMar()
			{
			}
			virtual ~CTcMar()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				if ( L"w:tcMar" != oNode.GetName() )
					return;

				XmlUtils::CXmlNode oChild;

				WritingElement_ReadNode( oNode, oChild, L"w:bottom",  m_oBottom );
				WritingElement_ReadNode( oNode, oChild, L"w:end",     m_oEnd );
				WritingElement_ReadNode( oNode, oChild, L"w:start",   m_oStart );
				WritingElement_ReadNode( oNode, oChild, L"w:top",     m_oTop );

				if ( !m_oEnd.IsInit() )
					WritingElement_ReadNode( oNode, oChild, L"w:right", m_oEnd );

				if ( !m_oStart.IsInit() )
					WritingElement_ReadNode( oNode, oChild, L"w:left",  m_oStart );
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if      ( L"w:bottom"  == sName ) m_oBottom = oReader;
					else if ( L"w:end"     == sName ) m_oEnd = oReader;
					else if ( L"w:start"   == sName ) m_oStart = oReader;
					else if ( L"w:top"     == sName ) m_oTop = oReader;
					else if ( !m_oEnd.IsInit()   && L"w:right" == sName ) m_oEnd = oReader;
					else if ( !m_oStart.IsInit() && L"w:left"  == sName ) m_oStart = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = L"<w:tcMar>";

				WritingElement_WriteNode_1( L"<w:bottom ", m_oBottom );
				WritingElement_WriteNode_1( L"<w:end ",    m_oEnd );
				WritingElement_WriteNode_1( L"<w:start ",  m_oStart );
				WritingElement_WriteNode_1( L"<w:top ",    m_oTop );

				sResult += L"</w:tcMar>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_tcMar;
			}
			static const CTcMar Merge(const CTcMar& oPrev, const CTcMar& oCurrent)
			{
				CTcMar oProperties;
				oProperties.m_oBottom	= Merge( oPrev.m_oBottom,	oCurrent.m_oBottom );
				oProperties.m_oEnd		= Merge( oPrev.m_oEnd,		oCurrent.m_oEnd );
				oProperties.m_oStart	= Merge( oPrev.m_oStart,	oCurrent.m_oStart );
				oProperties.m_oTop		= Merge( oPrev.m_oTop,		oCurrent.m_oTop );

				return oProperties;
			}
			template<typename Type>
			static nullable<Type>     Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
			{
				nullable<Type> oResult;

				if ( oCurrent.IsInit() )
					oResult = oCurrent;
				else if ( oPrev.IsInit() )
					oResult = oPrev;

				return oResult;
			}

			nullable<ComplexTypes::Word::CTblWidth > m_oBottom;
			nullable<ComplexTypes::Word::CTblWidth > m_oEnd;
			nullable<ComplexTypes::Word::CTblWidth > m_oStart;
			nullable<ComplexTypes::Word::CTblWidth > m_oTop;
		};

		//--------------------------------------------------------------------------------
		// TblPrChange 17.13.5.34 (Part 1)
		//--------------------------------------------------------------------------------
		class CTableCellProperties;
		class CTcPrChange : public WritingElement
		{
		public:
			CTcPrChange(OOX::Document *pMain = NULL);
			CTcPrChange(XmlUtils::CXmlNode &oNode);
			CTcPrChange(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTcPrChange();
			const CTcPrChange& operator = (const XmlUtils::CXmlNode &oNode);
			const CTcPrChange& operator = (const XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			nullable<std::wstring>					m_sAuthor;
			nullable<SimpleTypes::CDateTime>		m_oDate;
			nullable<SimpleTypes::CDecimalNumber>	m_oId;
			nullable<std::wstring>					m_sUserId;

			nullable<CTableCellProperties>			m_pTcPr;
		};
		//--------------------------------------------------------------------------------
		// CTableCellProperties
		//--------------------------------------------------------------------------------
		class CTableCellProperties : public WritingElement
		{
		public: 
			CTableCellProperties(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
				m_bTcPrChange = false;
			}
			CTableCellProperties(XmlUtils::CXmlNode &oNode) : WritingElement(NULL)
			{
				m_bTcPrChange = false;

				fromXML( oNode );
			}
			CTableCellProperties(XmlUtils::CXmlLiteReader& oReader) : WritingElement(NULL)
			{
				m_bTcPrChange = false;

				fromXML( oReader );
			}
			virtual ~CTableCellProperties()
			{
			}

			const CTableCellProperties& operator =(const XmlUtils::CXmlNode &oNode)
			{
				fromXML( (XmlUtils::CXmlNode &)oNode );
				return *this;
			}
			const CTableCellProperties& operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				if ( L"w:tcPr" != oNode.GetName() )
					return;

				XmlUtils::CXmlNode oChild;

				WritingElement_ReadNode( oNode, oChild, L"w:cellDel",        m_oCellDel );
				WritingElement_ReadNode( oNode, oChild, L"w:cellIns",        m_oCellIns );
				WritingElement_ReadNode( oNode, oChild, L"w:cellMerge",      m_oCellMerge );
				WritingElement_ReadNode( oNode, oChild, L"w:cnfStyle",       m_oCnfStyle );
				WritingElement_ReadNode( oNode, oChild, L"w:gridSpan",       m_oGridSpan );
				WritingElement_ReadNode( oNode, oChild, L"w:headers",        m_oHeaders );
				WritingElement_ReadNode( oNode, oChild, L"w:hideMark",       m_oHideMark );

				// В списке аттрибутов написано "hMerge", а в примере "hmerge"
				WritingElement_ReadNode( oNode, oChild, L"w:hmerge",         m_oHMerge );
				if ( !m_oHMerge.IsInit() )
					WritingElement_ReadNode( oNode, oChild, L"w:hMerge",     m_oHMerge );

				WritingElement_ReadNode( oNode, oChild, L"w:noWrap",         m_oNoWrap );
				WritingElement_ReadNode( oNode, oChild, L"w:shd",            m_oShd );
				WritingElement_ReadNode( oNode, oChild, L"w:tcBorders",      m_oTcBorders );
				WritingElement_ReadNode( oNode, oChild, L"w:tcFitText",      m_oTcFitText );
				WritingElement_ReadNode( oNode, oChild, L"w:tcMar",          m_oTcMar );

				if ( !m_bTcPrChange )
					WritingElement_ReadNode( oNode, oChild, L"w:tcPrChange", m_oTcPrChange );

				WritingElement_ReadNode( oNode, oChild, L"w:tcW",            m_oTcW );
				WritingElement_ReadNode( oNode, oChild, L"w:textDirection",  m_oTextDirection );
				WritingElement_ReadNode( oNode, oChild, L"w:vAlign",         m_oVAlign );

				// В списке аттрибутов написано "vMerge", а в примере "vmerge"
				WritingElement_ReadNode( oNode, oChild, L"w:vmerge",         m_oVMerge );
				if ( !m_oVMerge.IsInit() )
					WritingElement_ReadNode( oNode, oChild, L"w:vMerge",     m_oVMerge );
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if      ( L"w:cellDel"        == sName ) m_oCellDel = oReader;
					else if ( L"w:cellIns"        == sName ) m_oCellIns = oReader;
					else if ( L"w:cellMerge"      == sName ) m_oCellMerge = oReader;
					else if ( L"w:cnfStyle"       == sName ) m_oCnfStyle = oReader;
					else if ( L"w:gridSpan"       == sName ) m_oGridSpan = oReader;
					else if ( L"w:headers"        == sName ) m_oHeaders = oReader;
					else if ( L"w:hideMark"       == sName ) m_oHideMark = oReader;
					else if ( L"w:noWrap"         == sName ) m_oNoWrap = oReader;
					else if ( L"w:shd"            == sName ) m_oShd = oReader;
					else if ( L"w:tcBorders"      == sName ) m_oTcBorders = oReader;
					else if ( L"w:tcFitText"      == sName ) m_oTcFitText = oReader;
					else if ( L"w:tcMar"          == sName ) m_oTcMar = oReader;
					else if ( !m_bTcPrChange && L"w:tcPrChange" == sName ) m_oTcPrChange = oReader;
					else if ( L"w:tcW"            == sName ) m_oTcW = oReader;
					else if ( L"w:vAlign"         == sName ) m_oVAlign = oReader;
					
					else if ( L"w:textDirection" == sName || L"w:textFlow" == sName) m_oTextDirection = oReader;
					else if ( L"w:hMerge" == sName || L"w:hmerge" == sName) m_oHMerge = oReader;
					else if ( L"w:vMerge" == sName || L"w:vmerge" == sName) m_oVMerge = oReader;
				}
			}
			virtual std::wstring toXML() const             
			{
				std::wstring sResult = L"<w:tcPr>";

				WritingElement_WriteNode_1(L"<w:cnfStyle ", m_oCnfStyle );
				WritingElement_WriteNode_1(L"<w:tcW ", m_oTcW);
				WritingElement_WriteNode_1(L"<w:gridSpan ", m_oGridSpan);
				WritingElement_WriteNode_1(L"<w:hMerge ", m_oHMerge);
				WritingElement_WriteNode_1(L"<w:vMerge ", m_oVMerge);
				
				WritingElement_WriteNode_2(m_oTcBorders);
				
				WritingElement_WriteNode_1(L"<w:shd ", m_oShd);
				WritingElement_WriteNode_1(L"<w:noWrap ", m_oNoWrap);
				
				WritingElement_WriteNode_2(m_oTcMar);
				
				WritingElement_WriteNode_1(L"<w:textDirection ", m_oTextDirection);
				WritingElement_WriteNode_1(L"<w:tcFitText ", m_oTcFitText);

				WritingElement_WriteNode_1(L"<w:vAlign ", m_oVAlign);
				WritingElement_WriteNode_1(L"<w:hideMark ", m_oHideMark);

				WritingElement_WriteNode_2(m_oHeaders);

				WritingElement_WriteNode_1(L"<w:cellIns ", m_oCellIns );
				WritingElement_WriteNode_1(L"<w:cellDel ", m_oCellDel );
				WritingElement_WriteNode_1(L"<w:cellMerge ", m_oCellMerge);

				if ( !m_bTcPrChange )
					WritingElement_WriteNode_2( m_oTcPrChange );

				sResult += L"</w:tcPr>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_tcPr;
			}
			static const CTableCellProperties Merge(const CTableCellProperties& oPrev, const CTableCellProperties& oCurrent)
			{
				CTableCellProperties oProperties;
				oProperties.m_bTcPrChange            = oPrev.m_bTcPrChange || oCurrent.m_bTcPrChange;
				oProperties.m_oCellDel            = Merge( oPrev.m_oCellDel,            oCurrent.m_oCellDel );
				oProperties.m_oCellIns            = Merge( oPrev.m_oCellIns,            oCurrent.m_oCellIns );
				oProperties.m_oCellMerge            = Merge( oPrev.m_oCellMerge,            oCurrent.m_oCellMerge );
				oProperties.m_oCnfStyle            = Merge( oPrev.m_oCnfStyle,            oCurrent.m_oCnfStyle );
				oProperties.m_oGridSpan            = Merge( oPrev.m_oGridSpan,            oCurrent.m_oGridSpan );
				oProperties.m_oHeaders            = Merge( oPrev.m_oHeaders,            oCurrent.m_oHeaders );
				oProperties.m_oHideMark            = Merge( oPrev.m_oHideMark,            oCurrent.m_oHideMark );
				oProperties.m_oHMerge            = Merge( oPrev.m_oHMerge,            oCurrent.m_oHMerge );
				oProperties.m_oNoWrap            = Merge( oPrev.m_oNoWrap,            oCurrent.m_oNoWrap );
				oProperties.m_oShd            = Merge( oPrev.m_oShd,            oCurrent.m_oShd );

				if ( oCurrent.m_oTcBorders.IsInit() && oPrev.m_oTcBorders.IsInit() )
					oProperties.m_oTcBorders = OOX::Logic::CTcBorders::Merge(oPrev.m_oTcBorders.get(), oCurrent.m_oTcBorders.get());
				else
					oProperties.m_oTcBorders            = Merge( oPrev.m_oTcBorders,            oCurrent.m_oTcBorders );

				oProperties.m_oTcFitText            = Merge( oPrev.m_oTcFitText,            oCurrent.m_oTcFitText );

				if ( oCurrent.m_oTcMar.IsInit() && oPrev.m_oTcMar.IsInit() )
					oProperties.m_oTcMar = OOX::Logic::CTcMar::Merge(oPrev.m_oTcMar.get(), oCurrent.m_oTcMar.get());
				else
					oProperties.m_oTcMar            = Merge( oPrev.m_oTcMar,            oCurrent.m_oTcMar );

				oProperties.m_oTcPrChange            = Merge( oPrev.m_oTcPrChange,            oCurrent.m_oTcPrChange );
				oProperties.m_oTcW            = Merge( oPrev.m_oTcW,            oCurrent.m_oTcW );
				oProperties.m_oTextDirection            = Merge( oPrev.m_oTextDirection,            oCurrent.m_oTextDirection );
				oProperties.m_oVAlign            = Merge( oPrev.m_oVAlign,            oCurrent.m_oVAlign );
				oProperties.m_oVMerge            = Merge( oPrev.m_oVMerge,            oCurrent.m_oVMerge );

				return oProperties;
			}
			template<typename Type>
			static nullable<Type>     Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
			{
				nullable<Type> oResult;

				if ( oCurrent.IsInit() )
					oResult = oCurrent;
				else if ( oPrev.IsInit() )
					oResult = oPrev;

				return oResult;
			}
		public:

			bool m_bTcPrChange;

			nullable<ComplexTypes::Word::CTrackChange                    > m_oCellDel;
			nullable<ComplexTypes::Word::CTrackChange                    > m_oCellIns;
			nullable<ComplexTypes::Word::CCellMergeTrackChange           > m_oCellMerge;
			nullable<ComplexTypes::Word::CCnf                            > m_oCnfStyle;
			nullable<ComplexTypes::Word::CDecimalNumber                  > m_oGridSpan;
			nullable<OOX::Logic::CHeaders                                > m_oHeaders;
			nullable<ComplexTypes::Word::COnOff2 > m_oHideMark;
			nullable<ComplexTypes::Word::CHMerge                         > m_oHMerge;
			nullable<ComplexTypes::Word::COnOff2 > m_oNoWrap;
			nullable<ComplexTypes::Word::CShading                        > m_oShd;
			nullable<OOX::Logic::CTcBorders                              > m_oTcBorders;
			nullable<ComplexTypes::Word::COnOff2 > m_oTcFitText;
			nullable<OOX::Logic::CTcMar                                  > m_oTcMar;
			nullable<OOX::Logic::CTcPrChange                             > m_oTcPrChange;
			nullable<ComplexTypes::Word::CTblWidth                       > m_oTcW;
			nullable<ComplexTypes::Word::CTextDirection                  > m_oTextDirection;
			nullable<ComplexTypes::Word::CVerticalJc                     > m_oVAlign;
			nullable<ComplexTypes::Word::CVMerge                         > m_oVMerge;

		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABLE_PROPERTY_INCLUDE_H_
