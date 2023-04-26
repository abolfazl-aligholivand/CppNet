#include "Version.h"
#include <string>

namespace System
{
	Version::Version(int major, int minor)
	{
		if (major < 0)
			throw "major_ArgumentOutOfRange_Version";

		if (minor < 0)
			throw "minor_ArgumentOutOfRange_Version";

		this->_Major = major;
		this->_Minor = minor;
	}

	Version::Version(int major, int minor, int build)
	{
		if (major < 0)
			throw "major_ArgumentOutOfRange_Version";

		if (minor < 0)
			throw "minor_ArgumentOutOfRange_Version";

		if (build < 0)
			throw "build_ArgumentOutOfRange_Version";

		this->_Major = major;
		this->_Minor = minor;
		this->_Build = build;
	}

	Version::Version(int major, int minor, int build, int revision)
	{
		if (major < 0)
			throw "major_ArgumentOutOfRange_Version";

		if (minor < 0)
			throw "minor_ArgumentOutOfRange_Version";

		if (build < 0)
			throw "build_ArgumentOutOfRange_Version";
		if (revision < 0)
			throw "revision_ArgumentOutOfRange_Version";

		this->_Major = major;
		this->_Minor = minor;
		this->_Build = build;
		this->_Revision = revision;
	}

	Version::Version(const std::wstring version)
	{
		Version* v = Parse(version);
		_Major = v->getMajor();
		_Minor = v->getMinor();;
		_Build = v->getBuild();
		_Revision = v->getRevision();
	}

	Version::~Version()
	{
	}

	int Version::getMajor()
	{
		return _Major;
	}

	int Version::getMinor()
	{
		return _Minor;
	}

	int Version::getBuild()
	{
		return _Build;
	}

	int Version::getRevision()
	{
		return _Revision;
	}

	Version* Version::Parse(const std::wstring input)
	{
		if (input == L"")
			throw "input";

		//Split the input 
		std::vector<std::wstring> parts;
		std::string str(input.begin(), input.end());
		std::istringstream f(str);
		std::string s;
		while (std::getline(f, s, '.'))
			parts.push_back(std::wstring(s.begin(), s.end()));

		int major = 0, minor = 0, build = -1, revision = -1, n = parts.size();
		if (n > 0)
			major = std::stoi(parts[0]);
		if (n > 1)
			minor = std::stoi(parts[1]);
		if (n > 2)
			build = std::stoi(parts[2]);
		if (n > 3)
			revision = std::stoi(parts[3]);

		if (n == 2)
			return new Version(major, minor);
		else if (n == 3)
			return new Version(major, minor, build);
		else if (n == 4)
			return new Version(major, minor, build, revision);
		else
			throw "Invalid input";
	}

	std::wstring Version::ToString()
	{
		std::wstring version = L"";
		version += std::to_wstring(getMajor());
		version += (L"." + std::to_wstring(getMinor()));
		if (getBuild() > -1)
			version += (L"." + std::to_wstring(getBuild()));
		if (getRevision() > -1)
			version += (L"." + std::to_wstring(getRevision()));

		return version;
	}

	bool Version::Equals(Version* ver)
	{
		if (ver == nullptr)
			return false;

		if ((this->_Major != ver->getMajor()) ||
			(this->_Minor != ver->getMinor()) ||
			(this->_Build != ver->getBuild()) ||
			(this->_Revision != ver->getRevision()))
			return false;

		return true;
	}

	int Version::GetHashCode()
	{
		int accumulator = 0;

		accumulator |= (this->_Major & 0x0000000F) << 28;
		accumulator |= (this->_Minor & 0x000000FF) << 20;
		accumulator |= (this->_Build & 0x000000FF) << 12;
		accumulator |= (this->_Revision & 0x00000FFF);

		return accumulator;
	}

	int Version::CompareTo(Version* value)
	{
		if (value == nullptr)
			return 1;

		if (this->_Major != value->getMajor())
			if (this->_Major > value->getMajor())
				return 1;
			else
				return -1;

		if (this->_Minor != value->getMinor())
			if (this->_Minor > value->getMinor())
				return 1;
			else
				return -1;

		if (this->_Build != value->getBuild())
			if (this->_Build > value->getBuild())
				return 1;
			else
				return -1;

		if (this->_Revision != value->getRevision())
			if (this->_Revision > value->getRevision())
				return 1;
			else
				return -1;

		return 0;
	}

	bool Version::IsEqual(Version* version)
	{
		if (this == nullptr)
			return version == nullptr;
		return this->Equals(version);
	}

	bool Version::IsNotEqual(Version* version)
	{
		return !this->IsEqual(version);
	}
	
	bool Version::IsGreather(Version* version)
	{
		if (this == nullptr)
			throw "this";
		return (this->CompareTo(version) > 0);
	}
	
	bool Version::IsGreaterOrEqual(Version* version)
	{
		if (this == nullptr)
			throw "this";
		return (this->CompareTo(version) >= 0);
	}
	
	bool Version::IsLess(Version* version)
	{
		if (this == nullptr)
			throw "this";
		return (this->CompareTo(version) < 0);
	}
	
	bool Version::IsLessOrEqual(Version* version)
	{
		if (this == nullptr)
			throw "this";
		return (this->CompareTo(version) <= 0);
	}


	bool Version::operator ==(Version* v2)
	{
		if (this == nullptr)
			return v2 == nullptr;
		return this->Equals(v2);
	}

	bool Version::operator !=(Version* v2)
	{
		return !(this == v2);
	}

	bool Version::operator <(Version* v2)
	{
		if (this == nullptr)
			throw "this";
		return (this->CompareTo(v2) < 0);
	}
	
	bool Version::operator <=(Version* v2)
	{
		if (this == nullptr)
			throw "this";
		return (this->CompareTo(v2) <= 0);
	}
	
	bool Version::operator >(Version* v2)
	{
		return (v2 < this);
	}
	
	bool Version::operator >=(Version* v2)
	{
		return (v2 <= this);
	}
}