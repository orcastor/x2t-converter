#pragma once

#include "BiffStructure.h"
//#include <XLS_logic/Biff_structures/Phs.h>
//#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class XORObfuscation : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(XORObfuscation)
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	WORD key;
	WORD verificationBytes;
};

} // namespace XLS

