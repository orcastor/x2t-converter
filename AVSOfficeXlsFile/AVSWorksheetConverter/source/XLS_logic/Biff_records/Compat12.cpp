#include "stdafx.h"
#include "Compat12.h"
#include <XLS_logic/Biff_structures/FrtHeader.h>

namespace XLS
{;

Compat12::Compat12()
{
}


Compat12::~Compat12()
{
}


BaseObjectPtr Compat12::clone()
{
	return BaseObjectPtr(new Compat12(*this));
}


void Compat12::writeFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_Compat12);
	record << frtHeader;
	record << fNoCompatChk;
}


void Compat12::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_Compat12);
	record >> frtHeader;
	record >> fNoCompatChk;
}

} // namespace XLS

