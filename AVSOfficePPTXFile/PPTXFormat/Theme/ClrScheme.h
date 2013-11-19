#pragma once
#ifndef PPTX_THEME_CLRSCHEME_INCLUDE_H_
#define PPTX_THEME_CLRSCHEME_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Logic/UniColor.h"

namespace PPTX
{
	namespace nsTheme
	{
		class ClrScheme : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(ClrScheme)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				name = node.GetAttribute(_T("name"));

				Logic::UniColor lColor;
				Scheme.RemoveAll();

				lColor.GetColorFrom(node.ReadNode(_T("a:dk1")));		Scheme.SetAt(_T("dk1"), lColor);
				lColor.GetColorFrom(node.ReadNode(_T("a:lt1")));		Scheme.SetAt(_T("lt1"), lColor);
				lColor.GetColorFrom(node.ReadNode(_T("a:dk2")));		Scheme.SetAt(_T("dk2"), lColor);
				lColor.GetColorFrom(node.ReadNode(_T("a:lt2")));		Scheme.SetAt(_T("lt2"), lColor);
				lColor.GetColorFrom(node.ReadNode(_T("a:accent1")));	Scheme.SetAt(_T("accent1"), lColor);
				lColor.GetColorFrom(node.ReadNode(_T("a:accent2")));	Scheme.SetAt(_T("accent2"), lColor);
				lColor.GetColorFrom(node.ReadNode(_T("a:accent3")));	Scheme.SetAt(_T("accent3"), lColor);
				lColor.GetColorFrom(node.ReadNode(_T("a:accent4")));	Scheme.SetAt(_T("accent4"), lColor);
				lColor.GetColorFrom(node.ReadNode(_T("a:accent5")));	Scheme.SetAt(_T("accent5"), lColor);
				lColor.GetColorFrom(node.ReadNode(_T("a:accent6")));	Scheme.SetAt(_T("accent6"), lColor);
				lColor.GetColorFrom(node.ReadNode(_T("a:hlink")));		Scheme.SetAt(_T("hlink"), lColor);
				lColor.GetColorFrom(node.ReadNode(_T("a:folHlink")));	Scheme.SetAt(_T("folHlink"), lColor);

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;

				POSITION pos = Scheme.GetStartPosition();
				while (NULL != pos)
				{
					const CAtlMap<CString, Logic::UniColor>::CPair* pPair = Scheme.GetNext(pos);
					oValue.Write2(_T("a:") + pPair->m_key, pPair->m_value.toXML());
				}

				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("name"), name);

				return XmlUtils::CreateNode(_T("a:clrScheme"), oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:clrScheme"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("name"), name);				
				pWriter->EndAttributes();

				CString arr[12] = {_T("dk1"), _T("lt1"), _T("dk2"), _T("lt2"), _T("accent1"), _T("accent2"), _T("accent3"), _T("accent4"), 
					_T("accent5"), _T("accent6"), _T("hlink"), _T("folHlink")};

				for (LONG i = 0; i < 12; ++i)
				{
					const CAtlMap<CString, Logic::UniColor>::CPair* pPair = Scheme.Lookup(arr[i]);
					pWriter->StartNode(_T("a:") + arr[i]);
					pWriter->EndAttributes();
					pPair->m_value.toXmlWriter(pWriter);
					pWriter->EndNode(_T("a:") + arr[i]);
				}

				/*
				POSITION pos = Scheme.GetStartPosition();
				while (NULL != pos)
				{
					const CAtlMap<CString, Logic::UniColor>::CPair* pPair = Scheme.GetNext(pos);
					pWriter->StartNode(_T("a:") + pPair->m_key);
					pWriter->EndAttributes();
					pPair->m_value.toXmlWriter(pWriter);
					pWriter->EndNode(_T("a:") + pPair->m_key);					
				}
				*/

				pWriter->EndNode(_T("a:clrScheme"));
			}

			virtual DWORD GetRGBAFromScheme(const CString& str)const
			{
				const CAtlMap<CString, Logic::UniColor>::CPair* pPair = Scheme.Lookup(str);
				if (NULL == pPair)
					return 0;
				return pPair->m_value.GetRGBA();
			}
			virtual DWORD GetARGBFromScheme(const CString& str)const
			{
				const CAtlMap<CString, Logic::UniColor>::CPair* pPair = Scheme.Lookup(str);
				if (NULL == pPair)
					return 0;
				return pPair->m_value.GetARGB();
			}
			virtual DWORD GetBGRAFromScheme(const CString& str)const
			{
				const CAtlMap<CString, Logic::UniColor>::CPair* pPair = Scheme.Lookup(str);
				if (NULL == pPair)
					return 0;
				return pPair->m_value.GetBGRA();
			}
			virtual DWORD GetABGRFromScheme(const CString& str)const
			{
				const CAtlMap<CString, Logic::UniColor>::CPair* pPair = Scheme.Lookup(str);
				if (NULL == pPair)
					return 0;
				return pPair->m_value.GetABGR();
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, name);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				POSITION pos = Scheme.GetStartPosition();
				while (NULL != pos)
				{
					const CAtlMap<CString, Logic::UniColor>::CPair* pPair = Scheme.GetNext(pos);
					pWriter->WriteRecord1(SchemeClr_GetBYTECode(pPair->m_key), pPair->m_value);
				}
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _e = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attribute

				while (true)
				{
					BYTE _at = pReader->GetUChar();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					if (0 == _at)
						name = pReader->GetString2();
				}

				while (pReader->GetPos() < _e)
				{
					BYTE _rec = pReader->GetUChar();
					
					Logic::UniColor color;
					color.fromPPTY(pReader);

					Scheme.SetAt(SchemeClr_GetStringCode(_rec), color);
				}

				pReader->Seek(_e);
			}

		public:
			CString name;
			CAtlMap<CString, Logic::UniColor> Scheme;

			ClrScheme& operator=(const ClrScheme& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				name = oSrc.name;
				
				Scheme.RemoveAll();
				POSITION pos = oSrc.Scheme.GetStartPosition();
				while (NULL != pos)
				{
					const CAtlMap<CString, Logic::UniColor>::CPair* pPair = oSrc.Scheme.GetNext(pos);
					Scheme.SetAt(pPair->m_key, pPair->m_value);
				}

				return *this;
			}

		protected:
			virtual void FillParentPointersForChilds()
			{
				POSITION pos = Scheme.GetStartPosition();
				while (NULL != pos)
				{
					Scheme.GetNextValue(pos).SetParentPointer(this);
				}
			}
			//Logic::UniColor dk1; //Dark 1 
			//Logic::UniColor lt1; //Light 1 
			//Logic::UniColor dk2; //Dark 2 
			//Logic::UniColor lt2; //Light 2 
			//Logic::UniColor accent1; //Accent 1 
			//Logic::UniColor accent2; //Accent 2 
			//Logic::UniColor accent3; //Accent 3 
			//Logic::UniColor accent4; //Accent 4 
			//Logic::UniColor accent5; //Accent 5 
			//Logic::UniColor accent6; //Accent 6 
			//Logic::UniColor hlink; //Hyperlink 
			//Logic::UniColor folHlink; //Followed Hyperlink 
		};
	} // namespace nsTheme
} // namespace PPTX

#endif // PPTX_THEME_CLRSCHEME_INCLUDE_H