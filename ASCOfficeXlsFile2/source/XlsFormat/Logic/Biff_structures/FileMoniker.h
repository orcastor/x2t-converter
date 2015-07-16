#pragma once

#include "BiffStructure.h"

namespace OSHARED
{;

class CFRecord;

class FileMoniker : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FileMoniker)
public:
	FileMoniker();
	FileMoniker(XLS::CFRecord& record);
	XLS::BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	unsigned short cAnti;
	std::string ansiPath;
	unsigned short endServer;
	unsigned short versionNumber;
	unsigned int cbUnicodePathSize;
	unsigned short usKeyValue;
	std::wstring unicodePath;
};

} // namespace OSHARED

