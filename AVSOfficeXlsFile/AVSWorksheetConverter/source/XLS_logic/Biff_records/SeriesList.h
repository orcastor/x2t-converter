#pragma once

#include "BiffRecord.h"

namespace XLS
{;

class RGISeriesListSpecial : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(RGISeriesListSpecial)
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	std::vector<WORD> series;
};




// Logical representation of SeriesList record in BIFF8
class SeriesList: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(SeriesList)
	BASE_OBJECT_DEFINE_CLASS_NAME(SeriesList)
public:
	SeriesList();
	~SeriesList();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	RGISeriesListSpecial rgiser;
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_COMPLEX(rgiser)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

