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

#include <string>

#include "CodePageOle.h"
#include "../../XlsFile/Format/Binary/CFStream.h"
#include "../../XlsFile/Format/Auxiliary/HelpFunc.h"
#include "../../Common/Utils/simple_xml_writer.h"

#include "../../../UnicodeConverter/UnicodeConverter.h"

// #include <boost/date_time.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace OLEPS
{
PropertyCodePage::PropertyCodePage(unsigned int prop_type, const unsigned short value_type) : Property(prop_type, value_type), code_page(0)
{
}
bool PropertyCodePage::Read (XLS::CFStreamPtr stream)
{
	if (value_type == Property::VT_I2)
	{
		*stream >> code_page;
	}
	return true;
}
std::wstring PropertyCodePage::toString()
{
	return std::to_wstring(code_page);
}
//-------------------------------------------------------------------
PropertyString::PropertyString(unsigned int prop_type, const unsigned short value_type, unsigned short code_page_) : Property(prop_type, value_type)
{
	code_page = code_page_;
}
bool PropertyString::Read (XLS::CFStreamPtr stream)
{
	if (value_type != Property::VT_LPSTR) return false;

	_INT32 size;
	*stream >> size;

	if (size > 0)
	{
		char *s = new char[size];
		stream->read(s, size);

		for (_INT32 i = size - 1; i >= 0; i--)
		{
			if (s[i] == 0) size--;
			else break;
		}

		value = size > 0 ? STR::toStdWString(std::string(s, size), code_page) : L"";
		delete[]s;
	}

	return true;
}
std::wstring PropertyString::toString()
{
	return value;
}
bool PropertyString::IsEmpty()
{
	return value.empty();
}
//-------------------------------------------------------------------
PropertyWString::PropertyWString(unsigned int prop_type, const unsigned short value_type) : Property(prop_type, value_type)
{
}
bool PropertyWString::Read(XLS::CFStreamPtr stream)
{
	if (value_type != Property::VT_LPWSTR) return false;

	_INT32 size;
	*stream >> size;

	if (size > 0)
	{
		char *s = new char[size];
		stream->read(s, size);

		if (sizeof(wchar_t) == 4)
		{
			value = convertUtf16ToWString((UTF16*)s, size / 2);
		}
		else
		{
			value = std::wstring((wchar_t*)s, size / 2);
		}		

		delete[]s;
	}
	return true;
}
std::wstring PropertyWString::toString()
{
	return value;
}
//-------------------------------------------------------------------
PropertyDTM::PropertyDTM(unsigned int prop_type, const unsigned short value_type) : Property(prop_type, value_type)
{
}
bool PropertyDTM::Read (XLS::CFStreamPtr stream)
{
	dwLowDateTime = 0, dwHighDateTime = 0;
	if (value_type == Property::VT_FILETIME)
	{
		*stream >> dwLowDateTime >> dwHighDateTime;
	}
	return true;
}
bool PropertyDTM::IsEmpty()
{
	std::wstring value = toString();
	return value.empty();
}
std::wstring PropertyDTM::toString()
{
    _UINT64 temp = ((_UINT64)dwHighDateTime << 32) + dwLowDateTime;

    std::time_t epochStartTime = -11644473600LL; // Seconds from 1601 to 1970
    std::time_t timeInSeconds = (temp / 10000000) - epochStartTime;

    std::tm *tmStruct = std::gmtime(&timeInSeconds);

    int Year = tmStruct->tm_year + 1900;
    int Month = tmStruct->tm_mon + 1;
    int Day = tmStruct->tm_mday;
    int Hour = tmStruct->tm_hour;
    int Min = tmStruct->tm_min;

    std::wostringstream wss;
    wss << std::setfill(L'0') << std::setw(4) << Year << L"-"
        << std::setw(2) << Month << L"-"
        << std::setw(2) << Day << L"T"
        << std::setw(2) << Hour << L":"
        << std::setw(2) << Min << L":00Z";

    return wss.str();
}
//-------------------------------------------------------------------
PropertyInt::PropertyInt(unsigned int prop_type, const unsigned short value_type) : Property(prop_type, value_type)
{
}
bool PropertyInt::Read (XLS::CFStreamPtr stream)
{
	value = 0;
	if (value_type == Property::VT_I4)
	{
		*stream >> value;
	}
	return true;
}
std::wstring PropertyInt::toString()
{
	return std::to_wstring(value);
}
//-------------------------------------------------------------------
PropertyBool::PropertyBool(unsigned int prop_type, const unsigned short value_type) : Property(prop_type, value_type)
{
}
bool PropertyBool::Read (XLS::CFStreamPtr stream)
{
	value = false;
	if (value_type == Property::VT_BOOL)
	{
		_UINT32 v;
		*stream >> v;

		if (v != 0)
			value = true;
	}
	return true;
}
std::wstring PropertyBool::toString()
{
	return value ? L"1" : L"0";
}
//-------------------------------------------------------------------
PropertyVecString::PropertyVecString(unsigned int prop_type, const unsigned short value_type, unsigned short code_page_) : Property(prop_type, value_type)
{
	code_page = code_page_;
}
bool PropertyVecString::Read(XLS::CFStreamPtr stream)
{
	_INT32 count;
	*stream >> count;

	for (size_t i = 0; i < count; ++i)
	{
		PropertyString str(0, 0x001E, code_page);
		str.Read(stream);

		values.push_back(str.value);
	}
	return true;
}
std::wstring PropertyVecString::toString()
{
	std::wstringstream output;
	CP_XML_WRITER(output)
	{
		CP_XML_NODE(L"vt:vector")
		{
			CP_XML_ATTR(L"size", values.size());
			CP_XML_ATTR(L"baseType", L"lpstr");

			for (size_t i = 0; i < values.size(); ++i)
			{
				CP_XML_NODE(L"vt:lpstr")
				{
					CP_XML_CONTENT(values[i]);
				}
			}
		}
	}
	return output.str();
}//-------------------------------------------------------------------
PropertyVecHeadingPair::PropertyVecHeadingPair(unsigned int prop_type, const unsigned short value_type, unsigned short code_page_) : Property(prop_type, value_type)
{
	code_page = code_page_;
}
bool PropertyVecHeadingPair::Read(XLS::CFStreamPtr stream)
{
	_INT32 count;
	*stream >> count;

	for (size_t i = 0; i < count / 2; ++i)
	{
		HeadingPair pair;
		
		_INT16 type, padding;
		*stream >> type >> padding;
		
		if (type == 0x001E)
		{
			PropertyString str(0, type, code_page);
			str.Read(stream);

			pair.headingString = str.value;
		}
		else if (type == 0x001F)
		{
			PropertyWString str(0, type);
			str.Read(stream);

			pair.headingString = str.value;
		}

		*stream >> type >> padding >> pair.headerParts;

		values.push_back(pair);
	}
	return true;
}
std::wstring PropertyVecHeadingPair::toString()
{
	std::wstringstream output;
	CP_XML_WRITER(output)
	{
		CP_XML_NODE(L"vt:vector")
		{
			CP_XML_ATTR(L"size", values.size() * 2);
			CP_XML_ATTR(L"baseType", L"variant");

			for (size_t i = 0; i < values.size(); ++i)
			{
				CP_XML_NODE(L"vt:variant")
				{
					CP_XML_NODE(L"vt:lpstr")
					{
						CP_XML_CONTENT(values[i].headingString);
					}
				}
				CP_XML_NODE(L"vt:variant")
				{
					CP_XML_NODE(L"vt:i4")
					{
						CP_XML_STREAM() << values[i].headerParts;
					}
				}
			}
		}
	}
	return output.str();
}
//-------------------------------------------------------------------
PropertyDigSig::PropertyDigSig(unsigned int prop_type, const unsigned short value_type) : Property(prop_type, value_type) {}
bool PropertyDigSig::Read(XLS::CFStreamPtr stream)
{
	_INT32 cb;
	unsigned long pos_start = stream->getStreamPointer();
	*stream >> cb >> data.serializedPointer;
	
	data.sigInfo.Read(stream);

	stream->seekFromBegin(pos_start + cb);
	return true;
}
bool PropertyDigSig::SignatureInfo::Read(XLS::CFStreamPtr stream)
{
	*stream >> cbSignature >> signatureOffset >> cbSigningCertStore >> certStoreOffset >> certStoreOffset >>
		cbProjectName >> projectNameOffset >> fTimestamp >> cbTimestampUrl >> timestampUrlOffset;

	pbSignatureBuffer = boost::shared_array<char>(new char[cbSigningCertStore]);
	stream->read(pbSignatureBuffer.get(), cbSigningCertStore);
	
	pbSigningCertStoreBuffer.Read(stream);

	//rgchProjectNameBuffer; //null-terminate
	//rgchTimestampBuffer;
	return true;
}
bool PropertyDigSig::SerializedPropertyEntry::Read(XLS::CFStreamPtr stream)
{
	return true;
}
bool PropertyDigSig::SerializedCertificateEntry::Read(XLS::CFStreamPtr stream)
{
	return true;
}
bool PropertyDigSig::CertStoreCertificateGroup::Read(XLS::CFStreamPtr stream)
{
	return true;
}
bool PropertyDigSig::VBASigSerializedCertStore::Read(XLS::CFStreamPtr stream)
{
	return true;
}
std::wstring PropertyDigSig::toString()
{ 
	return L""; 
}

} // namespace OLEPS
