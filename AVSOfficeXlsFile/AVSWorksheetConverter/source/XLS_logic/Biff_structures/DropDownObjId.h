#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class DropDownObjId : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DropDownObjId)
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	ObjId id;
};

typedef boost::shared_ptr<DropDownObjId> DropDownObjIdPtr;

} // namespace XLS

