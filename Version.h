#pragma once
#include <iostream>
#include <sstream>
#include <vector>

namespace System
{
	class Version
	{
	public:
		Version(int major, int minor);
		Version(int major, int minor, int build);
		Version(int major, int minor, int build, int revision);
		Version(const std::wstring version);
		~Version();

	public:
		int getMajor();
		int getMinor();
		int getBuild();
		int getRevision();
		Version* Parse(const std::wstring input);
		std::wstring ToString();
		bool Equals(Version* ver);
		int GetHashCode();
		int CompareTo(Version* value);
		bool IsEqual(Version* version);
		bool IsNotEqual(Version* version);
		bool IsGreather(Version* version);
		bool IsGreaterOrEqual(Version* version);
		bool IsLess(Version* version);
		bool IsLessOrEqual(Version* version);

		bool operator ==(Version* v2);
		bool operator !=(Version* v2);
		bool operator <(Version* v2);
		bool operator <=(Version* v2);
		bool operator >(Version* v2);
		bool operator >=(Version* v2);

	private:
		int _Major;
		int _Minor;
		int _Build = -1;
		int _Revision = -1;
	};
}