#pragma once
#ifndef OOX_FROMTO_FILE_INCLUDE_H_
#define OOX_FROMTO_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CFromTo : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CFromTo)
			CFromTo()
			{
			}
			virtual ~CFromTo()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
			{
			}
			virtual void toXML2(CStringWriter& writer, CString sName) const
			{
				CString sStart;sStart.Format(_T("<%s>"), sName);
				writer.WriteStringC(sStart);
				if(m_oCol.IsInit())
				{
					CString sVal;sVal.Format(_T("<xdr:col>%d</xdr:col>"), m_oCol->GetValue());
					writer.WriteStringC(sVal);
				}
				if(m_oColOff.IsInit())
				{
					CString sVal;sVal.Format(_T("<xdr:colOff>%I64d</xdr:colOff>"), m_oColOff->ToEmu());
					writer.WriteStringC(sVal);
				}
				if(m_oRow.IsInit())
				{
					CString sVal;sVal.Format(_T("<xdr:row>%d</xdr:row>"), m_oRow->GetValue());
					writer.WriteStringC(sVal);
				}
				if(m_oRowOff.IsInit())
				{
					CString sVal;sVal.Format(_T("<xdr:rowOff>%I64d</xdr:rowOff>"), m_oRowOff->ToEmu());
					writer.WriteStringC(sVal);
				}
				CString sEnd;sEnd.Format(_T("</%s>"), sName);
				writer.WriteStringC(sEnd);
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

					if ( _T("xdr:col") == sName )
						m_oCol = oReader.GetText2();
					else if ( _T("xdr:colOff") == sName )
						m_oColOff = oReader.GetText2();
					else if ( _T("xdr:row") == sName )
						m_oRow = oReader.GetText2();
					else if ( _T("xdr:rowOff") == sName )
						m_oRowOff = oReader.GetText2();
				}
			}

			virtual EElementType getType () const
			{
				return et_FromTo;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCol;
			nullable<SimpleTypes::CEmu>			m_oColOff;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRow;
			nullable<SimpleTypes::CEmu>			m_oRowOff;
		};
		class CExt : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CExt)
			CExt()
			{
			}
			virtual ~CExt()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(CStringWriter& writer) const
			{
				writer.WriteStringC(_T("<xdr:ext"));
				if(m_oCx.IsInit())
				{
					CString sVal;sVal.Format(_T(" cx=\"%I64d\""), m_oCx->ToEmu());
					writer.WriteStringC(sVal);
				}
				if(m_oCy.IsInit())
				{
					CString sVal;sVal.Format(_T(" cy=\"%I64d\""), m_oCy->ToEmu());
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
				return et_Ext;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// ������ ��������
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("cx"),      m_oCx )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("cy"),      m_oCy )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CEmu>			m_oCx;
			nullable<SimpleTypes::CEmu>			m_oCy;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_FROMTO_FILE_INCLUDE_H_