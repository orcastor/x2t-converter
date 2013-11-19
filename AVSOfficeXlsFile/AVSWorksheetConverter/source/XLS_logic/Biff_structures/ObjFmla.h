#pragma once

#include "BiffStructTagged.h"
#include "ObjectParsedFormula.h"
#include "PictFmlaEmbedInfo.h"

namespace XLS
{;

class CFRecord;

class ObjFmla : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(ObjFmla)
public:
	ObjFmla(const bool is_part_of_FtPictFmla);
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	ObjectParsedFormula fmla;
private:
	bool is_part_of_FtPictFmla_;
	bool fmla_found;
	PictFmlaEmbedInfo embedInfo;

};

} // namespace XLS

