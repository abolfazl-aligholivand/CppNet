#pragma once
#include "Encoding.h"
#include "ASCIIEncoding.h"
#include "UTF16Encoding.h"
#include "UTF8Encoding.h"
#include <iostream>
#include <string>
#include <locale>
// #include <codecvt>
#include <vector>
using namespace std;

void EncodingTest()
{
	//UTF8Encoding();
	using namespace System::Text;
	//wstring unicodeString = L"This string contains the unicode character Pi (\u03a0)";
	wstring unicodeString = L"అధ్యక్షుడు హియర్";
	auto ascii = Encoding::ASCII();
	auto unicode = Encoding::Unicode();
	auto utf8 = Encoding::UTF8();

	auto unicodeBytes = unicode->GetBytes(unicodeString);
	auto utf8Bytes = utf8->GetBytes(unicodeString);
	auto asciiBytes = ascii->GetBytes(unicodeString);

	for (auto b : unicodeBytes)
		cout << (int)b << " ";
	cout << endl;
	auto utf16_str = unicode->GetString(unicodeBytes);
	std::wcout << utf16_str << endl;
	for (auto b : utf8Bytes)
		cout << (int)b << " ";
	cout << endl;
	auto utf8_str = utf8->GetString(utf8Bytes);
	std::wcout << utf8_str << endl;

	std::vector<unsigned char> bytes = std::vector<unsigned char> { 25, 14, 35 };
	UTF8Encoding* decoder = new UTF8Encoding();
	std::vector<char> chars;
	int byteUsed = 0, charUsed = 0;
	decoder->Convert(bytes, 0, 2, chars, byteUsed, charUsed);
	int result = 0;
}