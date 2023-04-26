#pragma once
#include "pch.h"
#include "PlatformID.h"
#include "Version.h"

namespace System
{
	class OperatingSystem
	{
	public:
		OperatingSystem();
		OperatingSystem(PlatformID platform, Version* version);
		~OperatingSystem();

	private:
		OperatingSystem(PlatformID platform, Version* version, std::wstring servicePack);

	public:

		//Methods
		PlatformID Platform();
		std::wstring ServicePack();
		Version* GetVersion();
		std::wstring ToString();
		std::wstring VersionString();

	private:
		Version* _version;
		PlatformID _platform;
		std::wstring _servicePack;
		std::wstring _versionString;

	};

}