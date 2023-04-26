#pragma once
#include "pch.h"
#include "OperatingSystem.h"
#include "PlatformID.h"
#include "Path.h"

namespace System
{
	enum EnvironmentVariableTarget {
		Process = 0,
		User = 1,
		Machine = 2,
	};

	class Environment
	{
	public:
		Environment();
		~Environment();

		//Methods
		static std::wstring CurrentDirectory();
		static std::wstring MachineName();
		static std::wstring NewLine();
		//static std::wstring UserName();
		static std::wstring SystemDirectory();
		static std::wstring GetEnvironmentVariables(std::wstring variable);
		static OperatingSystem* OSVersion();
		static PlatformID Platform();
		static bool IsWindows8OrAbove();
		static bool Is64BitProcess();

	private:
		static Version* GetOSVersion();
	private:
		const int MaxEnvVariableValueLength = 32767;
		const int MaxSystemEnvVariableLength = 1024;
		const int MaxUserEnvVariableLength = 255;
		static bool s_CheckedOSWin8OrAbove;
		static bool s_IsWindows8OrAbove;

	};
}
