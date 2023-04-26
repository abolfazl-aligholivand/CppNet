#pragma once
#include "pch.h"
#include "File.h"

void FileTest()
{
	using namespace System::IO;

	std::wstring path = L"E:\\Projects\\FileTest.txt";
	std::wstring path1 = L"E:\\Projects\\FileTest-copy.txt";
	std::wstring path2 = L"E:\\Projects\\FileTest-copy2.txt";
	std::wstring path3 = L"E:\\Projects\\FileTest-copy3.txt";

	std::vector<_byte> bytes = std::vector<_byte>{ 1,2,3,4,5,6,7,8,9 };
	auto stream = File::Create(path);
	stream->Write(bytes, 0, bytes.size());

	auto allBytes = File::ReadAllBytes(path3);
	File::Delete(path2);
	File::Delete(path3);
	File::Copy(path1, path2);
}