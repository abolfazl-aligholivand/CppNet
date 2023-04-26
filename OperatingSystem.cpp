#include "OperatingSystem.h"

namespace System
{
	OperatingSystem::OperatingSystem()
	{
		_version = nullptr;
		_platform = PlatformID::Win32NT;
	}

	OperatingSystem::~OperatingSystem()
	{
	}

	OperatingSystem::OperatingSystem(PlatformID platform, Version* version) : OperatingSystem(platform, version, L"")
	{
	}

	OperatingSystem::OperatingSystem(PlatformID platform, Version* version, std::wstring servicePack)
	{
		if (platform < PlatformID::Win32S || platform > PlatformID::MacOSX)
			throw "Arg_EnumIllegalVal_platform";
		if (version == nullptr)
			throw "version";

		_platform = platform;
		_version = version;
		_servicePack = servicePack;
	}

	PlatformID OperatingSystem::Platform()
	{
		return _platform;
	}

	std::wstring OperatingSystem::ServicePack()
	{
		if (_servicePack == L"")
			return L"";
		return _servicePack;
	}
	
	Version* OperatingSystem::GetVersion()
	{
		return _version;
	}
	
	std::wstring OperatingSystem::ToString()
	{
		return VersionString();
	}
	
	std::wstring OperatingSystem::VersionString()
	{
		if (_versionString != L"")
			return _versionString;

		std::wstring os = L"";
		switch (_platform)
		{

		case PlatformID::Win32NT:
			os = L"Microsoft Windows NT ";
			break;
		case PlatformID::Win32Windows:
			if ((_version->getMajor() > 4) ||
				((_version->getMajor() == 4) && (_version->getMinor() > 0)))
				os = L"Microsoft Windows 98 ";
			else
				os = L"Microsoft Windows 95 ";
			break;
		case PlatformID::Win32S:
			os = L"Microsoft Win32S ";
			break;
		case PlatformID::WinCE:
			os = L"Microsoft Windows CE ";
			break;
		case PlatformID::MacOSX:
			os = L"Mac OS X ";
			break;
		default:
			os = L"<unknown> ";
			break;
		}

		if (_servicePack == L"")
			_versionString = os + _version->ToString();
		else
			_versionString = os + _version->ToString() + L" " + _servicePack;

		return _versionString;
	}
}