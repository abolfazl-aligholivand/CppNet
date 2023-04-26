#pragma once
#include "Version.h"

void VersionTest()
{
	using namespace System;

	Version* version1 = new Version(1, 2);
	std::wcout << "Version(1, 2): " << version1->ToString() << std::endl;

	Version* version2 = new Version(2, 1, 4);
	std::wcout << "Version(2, 1, 4): " << version2->ToString() << std::endl;

	Version* version3 = new Version(4, 0, 1, 0);
	std::wcout << "Version(4, 0, 1, 0): " << version3->ToString() << std::endl;

	Version* version4 = new Version(version2->ToString());
	std::cout << "Major: " << version4->getMajor() << std::endl;
	std::cout << "Minor: " << version4->getMinor() << std::endl;
	std::cout << "Build: " << version4->getBuild() << std::endl;
	std::cout << "Revision: " << version4->getRevision() << std::endl;
}
