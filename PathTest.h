#pragma once
#include "pch.h"
#include "Path.h"

void PathTest()
{
	using namespace System::IO;
	std::wstring path1 = L"/home/aligholi/Desktop/de4dot/Source.cpp";
	std::wstring path2 = L"/home/aligholi/Desktop";
	std::wstring path3 = L"empty.txt";

	auto extention = Path::GetExtension(path1);
	std::wcout << L"Extention: " << extention << std::endl; //OK -- .pdf

	auto combine = Path::Combine(path2, path3);
	std::wcout << L"Combine: " << combine << std::endl; //OK -- D:\Pictures\(11).jpg
	
	auto isPathRooted = Path::IsPathRooted(path3);
	std::cout << "IsRooted: " << isPathRooted << std::endl; //OK -- False

	auto fullPath = Path::GetFullPath(path1);
	std::wcout  << L"FullPath: " << fullPath << std::endl; // Not-Ok

	auto fileName = Path::GetFileName(path1);
	std::wcout << L"FileName: " << fileName << std::endl; //OK -- MITRE-SOC.pdf

	auto fileNameWithoutExtension = Path::GetFileNameWithoutExtension(path1);
	std::wcout  << L"FileName: " << fileNameWithoutExtension << std::endl; //OK -- MITRE-SOC

	auto directoryName = Path::GetDirectoryName(path1);
	std::wcout  << L"DirectoryName: " << directoryName << std::endl; //OK -- E:\Learning\Books

	auto pathRoot = Path::GetPathRoot(path2);
	std::wcout  << L"PathRoot: " << pathRoot << std::endl;// OK -- D:
}