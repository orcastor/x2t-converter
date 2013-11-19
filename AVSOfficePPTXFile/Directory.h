#pragma once

#include "Settings.h"
#include <windows.h>

namespace FileSystem {
	class Directory {
	public:
        static LPCTSTR GetCurrentDirectory();
		static String GetCurrentDirectoryS();
		
		static bool CreateDirectory(LPCTSTR path);
        static bool CreateDirectory(const String& path);
        static bool CreateDirectories(LPCTSTR path);

		static StringArray GetFilesInDirectory(LPCTSTR path, const bool& andSubdirectories = false);
        static StringArray GetFilesInDirectory(const String& path, const bool& andSubdirectories = false);

        static int GetFilesCount(const CString& path, const bool& recursive = false);
	};
}
