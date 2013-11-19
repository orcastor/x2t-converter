#pragma once
#ifndef PPTX_IFILE_CONTAINER_INCLUDE_H_
#define PPTX_IFILE_CONTAINER_INCLUDE_H_

#include "DocxFormat/IFileContainer.h"
#include "FileMap.h"
#include "PPTXEvent.h"

namespace PPTX
{
	class FileContainer : public OOX::IFileContainer
	{
	public:
		FileContainer()
		{
			m_lPercent = 0;
			m_bCancelled = false;

		}
	protected:
		void read(const OOX::CPath& filename);
		void read(const OOX::Rels::File& rels, const OOX::CPath& path);

		void read(const OOX::CPath& filename, FileMap& map, IPPTXEvent* Event);
		void read(const OOX::Rels::File& rels, const OOX::CPath& path, FileMap& map, IPPTXEvent* Event);
		void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::ContentTypes::File& content) const;
		void write(OOX::Rels::File& rels, const OOX::CPath& current, const OOX::CPath& directory, OOX::ContentTypes::File& content) const;

		void WrittenSetFalse();

		long m_lPercent;
		bool m_bCancelled;
	};

	class CCommonRels : public PPTX::FileContainer
	{
	public:
		CCommonRels() : PPTX::FileContainer()
		{
		}
	
		void _read(const OOX::CPath& filename);
		void _read(const OOX::Rels::File& rels, const OOX::CPath& path);
	};
} // namespace PPTX

#endif // PPTX_IFILE_CONTAINER_INCLUDE_H_