#pragma once
#include "pch.h"
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h> 
#include <strsafe.h>

namespace System
{
	namespace IO
	{
		class Directory
		{
		public:
			Directory();
			~Directory();

			//Methods
			static void CreateNewDirectory(const std::wstring path);
			static bool Exists(const std::wstring path);
			static std::vector<std::wstring> GetDirectories(const std::wstring directory);
			static std::wstring GetCurrentDirectoryPath();
			static std::wstring GetParent(const std::wstring path);
		};
	}
}

