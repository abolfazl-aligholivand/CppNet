#include "Environment.h"
#include "Directory.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif // _WIN32


namespace System
{
	Environment::Environment()
	{
	}

	Environment::~Environment()
	{
	}

	std::wstring Environment::CurrentDirectory()
	{
		return IO::Directory::GetCurrentDirectoryPath();
	}

	std::wstring Environment::MachineName()
	{
#ifdef _WIN32
#define INFO_BUFFER_SIZE 32767
		TCHAR  infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;

		// Get the name of the computer.
		if (!GetComputerName(infoBuf, &bufCharCount))
			return L"";
		return infoBuf;
#else
		char* hostname;
		int result;
		result = gethostname(hostname, 32767);
		if (result)
		{
			perror("gethostname");
			return 0;
		}
		std::string str = hostname;
		return std::wstring(str.begin(), str.end());
#endif // _WIN32
	}

//	std::wstring Environment::UserName()
//	{
//#ifdef _WIN32
//#define INFO_BUFFER_SIZE 32767
//		TCHAR  infoBuf[INFO_BUFFER_SIZE];
//		DWORD  bufCharCount = INFO_BUFFER_SIZE;
//
//		// Get the user name.
//		if (!GetUserName(infoBuf, &bufCharCount))
//			return L"";
//		return infoBuf;
//#else
//		char* username;
//		int result;
//		result = getlogin_r(username, 32767);
//		if (result)
//		{
//			perror("username");
//			return 0;
//		}
//		std::string str = username;
//		return std::wstring(str.begin(), str.end());
//#endif // _WIN32
//	}

	std::wstring Environment::NewLine()
	{
		return L"\r\n";
	}

	std::wstring Environment::SystemDirectory()
	{
		auto WINDIR = GetEnvironmentVariables(L"WINDIR");
		return IO::Path::Combine(WINDIR, L"system32");
	}

	std::wstring Environment::GetEnvironmentVariables(std::wstring variable)
	{
		std::string path(variable.begin(), variable.end());
		const char* env_p = std::getenv(path.c_str());
		std::wstring VariableName = L"";
		if (env_p != "")
		{
			std::string str = env_p;
			VariableName = std::wstring(str.begin(), str.end());
		}
		return VariableName;
	}

	OperatingSystem* Environment::OSVersion()
	{
		auto version = GetOSVersion();
		auto platform = Platform();
		return new OperatingSystem(platform, version);
	}

	bool Environment::s_CheckedOSWin8OrAbove = false;
	bool Environment::s_IsWindows8OrAbove = false;

	bool Environment::IsWindows8OrAbove()
	{
		if (!s_CheckedOSWin8OrAbove)
		{
			OperatingSystem* OS = OSVersion();
			s_IsWindows8OrAbove = (OS->Platform() == PlatformID::Win32NT &&
				((OS->GetVersion()->getMajor() == 6 && OS->GetVersion()->getMinor() >= 2) || (OS->GetVersion()->getMajor() > 6)));
			s_CheckedOSWin8OrAbove = true;
		}
		return s_IsWindows8OrAbove;
	}

	bool Environment::Is64BitProcess()
	{
#if WIN32
		return false;
#else
		return true;
#endif
	}

	PlatformID Environment::Platform()
	{
#ifdef __linux__ 
		return PlatformID::Unix;
#elif _WIN32
		return PlatformID::Win32NT;
#elif __APPLE__
		return PlatformID::MacOSX;
#endif
	}

	Version* Environment::GetOSVersion()
	{
#ifdef _WIN32
		DWORD dwVersion = 0;
		DWORD dwMajorVersion = 0;
		DWORD dwMinorVersion = 0;
		DWORD dwBuild = 0;
		DWORD dwRevision = 0;

		dwVersion = GetVersion();

		// Get the Windows version.

		dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
		dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

		// Get the build number.

		if (dwVersion < 0x80000000)
			dwBuild = (DWORD)(HIWORD(dwVersion));

		return new Version(dwMajorVersion, dwMinorVersion, dwBuild, dwRevision);
#else // for Non Win32 OS
		return new Version(10, 10, 0, 0); // Not Emplement
#endif // _WIN32
	}
}