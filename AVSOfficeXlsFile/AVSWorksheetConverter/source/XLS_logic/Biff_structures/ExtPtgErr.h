#pragma once

#include "Ptg.h"
#include "BitMarkedStructs.h"

namespace XLS
{;

class CFRecord;

class ExtPtgErr : public Ptg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(ExtPtgErr)
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent);

private:
	BErr err;
};

} // namespace XLS

