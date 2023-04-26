#pragma once
#include "pch.h"
#include <vector>>
#include <iostream>

namespace System
{
	namespace IO
	{
		class Path
		{
		public:
			Path();
			~Path();

		public:
			static std::wstring ChangeExtension(std::wstring path, std::wstring extension);
			static std::wstring Combine(std::wstring path1, std::wstring path2);
			static std::wstring GetDirectoryName(std::wstring path);
			static std::wstring GetFileName(std::wstring path);
			static std::wstring GetFileNameWithoutExtension(std::wstring path);
			static std::wstring GetFullPath(std::wstring path);
			static std::wstring GetPathRoot(std::wstring path);
			static bool IsPathRooted(std::wstring path);
			static std::wstring GetExtension(std::wstring path);
			static std::wstring NormalizePath(std::wstring path, bool fullCheck);

		private:
			static std::wstring CombineNoChecks(std::wstring path1, std::wstring path2);
			static std::wstring CleanPath(std::wstring path);
			static std::wstring NewNormalizePathLimitedChecks(std::wstring path, int maxPathLength, bool expandShortPaths);
			static int PathStartSkip(std::wstring path);
			static bool IsDirectorySeparator(char c);
			static bool IsValidDriveChar(char value);
			static int GetRootLength(std::wstring path);
			static bool StartsWithOrdinal(char* source, unsigned long sourceLength, std::wstring value);

		public:
			static char DirectorySeparatorChar;
			static char AltDirectorySeparatorChar;
			static char VolumeSeparatorChar;
			static std::vector<char> InvalidPathChars;
			
		private:
			static std::wstring DirectorySeparatorCharAsString;
			static std::vector<char> InvalidPathcharsWithAdditionalChecks;
			static std::vector<char> InvalidFileNameChars;
			static char PathSeparator;


		};
	}
}
