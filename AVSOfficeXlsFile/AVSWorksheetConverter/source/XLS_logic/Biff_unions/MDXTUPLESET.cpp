#include "stdafx.h"
#include "MDXTUPLESET.h"
#include <XLS_logic/Biff_records/MDXTuple.h>
#include <XLS_logic/Biff_records/MDXSet.h>
#include <XLS_logic/Biff_records/ContinueFrt12.h>

namespace XLS
{;


MDXTUPLESET::MDXTUPLESET()
{
}


MDXTUPLESET::~MDXTUPLESET()
{
}


BaseObjectPtr MDXTUPLESET::clone()
{
	return BaseObjectPtr(new MDXTUPLESET(*this));
}


// MDXTUPLESET = (MDXTuple / MDXSet) *ContinueFrt12
const bool MDXTUPLESET::loadContent(BinProcessor& proc)
{
	if(!proc.optional<MDXTuple>())
	{
		if(!proc.optional<MDXSet>())
		{
			return false;
		}
	}
	proc.repeated<ContinueFrt12>(0, 0);
	return true;
}

} // namespace XLS

