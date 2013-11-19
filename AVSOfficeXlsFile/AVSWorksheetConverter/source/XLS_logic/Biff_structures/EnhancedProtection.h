#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class EnhancedProtection : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(EnhancedProtection)
public:
	EnhancedProtection();
	EnhancedProtection(CFRecord& record);
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	bool iprotObjects;
	bool iprotScenarios;
	bool iprotFormatCells;
	bool iprotFormatColumns;
	bool iprotFormatRows;
	bool iprotInsertColumns;
	bool iprotInsertRows;
	bool iprotInsertHyperlinks;
	bool iprotDeleteColumns;
	bool iprotDeleteRows;
	bool iprotSelLockedCells;
	bool iprotSort;
	bool iprotAutoFilter;
	bool iprotPivotTables;
	bool iprotSelUnlockedCells;
};

} // namespace XLS

