#pragma once
#ifndef OOX_NUMFMTS_FILE_INCLUDE_H_
#define OOX_NUMFMTS_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "rPr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CNumFmt : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CNumFmt)
			CNumFmt()
			{
			}
			virtual ~CNumFmt()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
			{
				toXML2(writer, CString(_T("numFmt")));
			}
			void toXML2(CStringWriter& writer, CString& sHeader) const
			{
				writer.WriteStringC(_T("<") + sHeader);
				if(m_oNumFmtId.IsInit())
				{
					CString sVal;sVal.Format(_T(" numFmtId=\"%d\""), m_oNumFmtId->GetValue());
					writer.WriteStringC(sVal);
				}
				if(m_oFormatCode.IsInit())
				{
					CString sVal;sVal.Format(_T(" formatCode=\"%s\""), XmlUtils::EncodeXmlString(m_oFormatCode.get()));
					writer.WriteStringC(sVal);
				}
				if(m_oSourceLinked.IsInit())
				{
					CString sVal;
					sVal.Format(_T(" sourceLinked=\"%s\""), m_oSourceLinked->ToString2(SimpleTypes::onofftostring1));
					writer.WriteStringC(sVal);
				}
				writer.WriteStringC(_T("/>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_NumFmt;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// ������ ��������
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("formatCode"),      m_oFormatCode )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("numFmtId"),      m_oNumFmtId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("sourceLinked"),      m_oSourceLinked )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<CString >								m_oFormatCode;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oNumFmtId;
			nullable<SimpleTypes::COnOff<>>				m_oSourceLinked;
		};
		class CNumFmts : public WritingElementWithChilds<CNumFmt>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CNumFmts)
			CNumFmts()
			{
			}
			virtual ~CNumFmts()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
			{
				if(m_arrItems.GetSize() > 0 )
				{
					writer.WriteStringC(_T("<numFmts"));
					if(m_oCount.IsInit())
					{
						CString sVal;sVal.Format(_T(" count=\"%d\""), m_oCount->GetValue());
						writer.WriteStringC(sVal);
					}
					writer.WriteStringC(_T(">"));
					for(int i = 0, length = m_arrItems.GetSize(); i < length; ++i)
						m_arrItems[i]->toXML(writer);
					writer.WriteStringC(_T("</numFmts>"));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("numFmt") == sName )
						m_arrItems.Add( new CNumFmt( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_NumFmts;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// ������ ��������
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCount;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_NUMFMTS_FILE_INCLUDE_H_