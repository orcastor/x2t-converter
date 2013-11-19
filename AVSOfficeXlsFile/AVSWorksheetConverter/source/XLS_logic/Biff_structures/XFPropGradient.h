#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/Boolean.h>

namespace XLS
{;

class CFRecord;

class XFPropGradient : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(XFPropGradient)
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	Boolean<DWORD> type;
	double numDegree;
	double numFillToLeft;
	double numFillToRight;
	double numFillToTop;
	double numFillToBottom;
};

} // namespace XLS
