#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of SORTDATA12 union of records 
class SORTDATA12: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SORTDATA12)
public:
	SORTDATA12();
	~SORTDATA12();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSORTDATA12;
	
	int serialize(std::wostream & stream);

	BaseObjectPtr		m_SortData;
};

} // namespace XLS

