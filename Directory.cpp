#include "Directory.h"
#include "Path.h"
#include "File.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <dirent.h>
#endif

namespace System
{
	namespace IO
	{
		Directory::Directory()
		{
		}

		Directory::~Directory()
		{
		}

		void Directory::CreateNewDirectory(const std::wstring path)
		{
			if (path == L"")
				throw "Path is empty";

			if (File::Exists(path))
				throw "Path is directory";
			if (!Exists(path))
			{
				std::string p(path.begin(), path.end());
#ifdef _WIN32
				::_mkdir(p.c_str());
#else
#if _POSIX_C_SOURCE
				::mkdir(p.c_str());
#endif
#endif
			}
		}

		bool Directory::Exists(const std::wstring path)
		{
			std::string p(path.begin(), path.end());
			struct stat info;
			if (stat(p.c_str(), &info) != 0)
				return 0;
			else if (info.st_mode & S_IFDIR)
				return 1;
			else
				return 0;
		}

		std::vector<std::wstring> Directory::GetDirectories(const std::wstring directory)
		{
			std::vector<std::wstring> directories;
#ifdef _WIN32
			WIN32_FIND_DATA ffd;
			LARGE_INTEGER filesize;
			TCHAR szDir[MAX_PATH];
			size_t length_of_arg;
			HANDLE hFind = INVALID_HANDLE_VALUE;
			DWORD dwError = 0;

			// If the directory is not specified as a command-line argument,
			// Check that the input path plus 3 is not longer than MAX_PATH.
			// Three characters are for the "\*" plus NULL appended below.
			StringCchLength(directory.c_str(), MAX_PATH, &length_of_arg);

			if (length_of_arg > (MAX_PATH - 3))
			{
				_tprintf(TEXT("\nDirectory path is too long.\n"));
				return directories;
			}

			// Prepare string for use with FindFile functions.  First, copy the
			// string to a buffer, then append '\*' to the directory name.

			StringCchCopy(szDir, MAX_PATH, directory.c_str());
			StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

			// Find the first file in the directory.
			hFind = FindFirstFile(szDir, &ffd);

			if (INVALID_HANDLE_VALUE == hFind)
			{
				return directories;
			}

			// List all the files in the directory with some info about them.
			do
			{
				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (ffd.cFileName[0] == L'.')
						continue;
					if (Exists(Path::Combine(directory, ffd.cFileName)))
						directories.push_back(Path::Combine(directory, ffd.cFileName));
				}
				/* This secion is for files that are in directory */
				//else
				//{
				//    filesize.LowPart = ffd.nFileSizeLow;
				//    filesize.HighPart = ffd.nFileSizeHigh;
				//    if (ffd.cFileName[0] == L'.')
				//        continue;
				//    _tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
				//}
			} while (FindNextFile(hFind, &ffd) != 0);
			FindClose(hFind);
#else
			struct dirent* de;  // Pointer for directory entry 

			// opendir() returns a pointer of DIR type.  
			DIR* dr = opendir(".");

			if (dr == NULL)  // opendir returns NULL if couldn't open directory 
			{
				printf("Could not open current directory");
				return 0;
			}

			while ((de = readdir(dr)) != NULL)
			{
				if (de->d_name[0] == '.')
					continue;
				std::string dir_name(de->d_name);
				auto dir = Path::Combine(directory, std::wstring(dir_name.begin(), dir_name.end()));
				if (Exists(dir))
					directories.push_back(dir);
			}
			closedir(dr);
#endif
			return directories;
		}

		std::wstring Directory::GetCurrentDirectoryPath()
		{
			char buff[FILENAME_MAX];
#ifdef _WIN32
			_getcwd(buff, FILENAME_MAX);
#else
			getcwd(buff, FILENAME_MAX);
#endif
			std::string currentDirectory(buff);
			return std::wstring(currentDirectory.begin(), currentDirectory.end());
		}

		std::wstring Directory::GetParent(const std::wstring path)
		{
			if (path == L"")
				throw "path";

			if (path.size() == 0)
				throw "path";

			std::wstring fullPath = Path::GetFullPath(path);

			std::wstring s = Path::GetDirectoryName(fullPath);
			if (s == L"")
				return L"";
			return s;
		}
	}
}
