#pragma once
#ifndef ODT_LIMIT_RUN_THROUGH_INCLUDE_H_
#define ODT_LIMIT_RUN_THROUGH_INCLUDE_H_

#include "setter.h"
#include <string>


namespace Odt
{
	namespace Limit
	{
		class RunThrough : public setter::from<std::string>
		{
		public:
			RunThrough();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace Odt

#endif // ODT_LIMIT_RUN_THROUGH_INCLUDE_H_