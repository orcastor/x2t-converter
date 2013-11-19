#pragma once
#ifndef OOX_THEME_FILE_INCLUDE_H_
#define OOX_THEME_FILE_INCLUDE_H_

#include "./../File.h"
#include "ThemeElements.h"
#include "ObjectDefaults.h"
#include "ExtraClrSchemeLst.h"


namespace OOX
{
	namespace Theme
	{
		class File : public OOX::File
		{
		public:
			File();
			File(const boost::filesystem::wpath& filename);
			virtual ~File();

		public:
			virtual void read(const boost::filesystem::wpath& filename);
			virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const;

		public:
			virtual const FileType type() const;
			virtual const boost::filesystem::wpath DefaultDirectory() const;
			virtual const boost::filesystem::wpath DefaultFileName() const;

		public:
			const std::string GetMajorFont() const;
			const std::string GetMinorFont() const;

		public:
			nullable_property<std::string>       name;
			nullable_property<ThemeElements>     themeElements;
			nullable_property<ObjectDefaults>    objectDefaults;
			nullable_property<ExtraClrSchemeLst> extraClrSchemeLst;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_FILE_INCLUDE_H_