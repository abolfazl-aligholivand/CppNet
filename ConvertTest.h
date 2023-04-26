#pragma once
#include "Convert.h"
#include "Environment.h"
#include <vector>

bool ArraysAreEqual(std::vector<_byte> a1, std::vector<_byte> a2)
{
	if (a1.size() != a2.size()) return false;
	for (int i = 0; i < a1.size(); i++)
		if (a1[i] != a2[i]) return false;
	return true;
}

void ConvertTest()
{
	using namespace System;
	std::vector<_byte> bytes = std::vector<_byte>{ 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };

	std::cout << "Byte Array:       ";
	for (auto b : bytes)
		std::cout << (int)b << " ";
	std::cout << "\n----------------------------------------------" << std::endl;

	// Convert the array to a base 64 string.
	std::wstring toBase64String = Convert::ToBase64String(bytes);
	std::wcout << "ToBase64String:   " << toBase64String << std::endl;
	std::cout << "----------------------------------------------" << std::endl;

	std::vector<_byte> newBytes = Convert::FromBase64String(toBase64String);
	std::cout << "FromBase64String: ";
	for (auto b : newBytes)
		std::cout << (int)b << " ";
	std::cout << "\n----------------------------------------------" << std::endl;


	std::vector<_byte> byteArray1 = std::vector<_byte>(256);
	std::vector<_byte> byteArray2 = std::vector<_byte>(256);
	std::vector<char> charArray = std::vector <char>(352);
	int charArrayLength;
	std::wstring nl = Environment::NewLine();

	std::wstring ruler1a = L"         1         2         3         4";
	std::wstring ruler2a = L"1234567890123456789012345678901234567890";
	std::wstring ruler3a = L"----+----+----+----+----+----+----+----+";
	std::wstring ruler1b = L"         5         6         7      ";
	std::wstring ruler2b = L"123456789012345678901234567890123456";
	std::wstring ruler3b = L"----+----+----+----+----+----+----+-";
	std::wstring ruler = ruler1a + ruler1b + nl + ruler2a + ruler2b + nl + ruler3a + ruler3b;

	// 1) Initialize and display a Byte array of arbitrary data.
	std::wcout << L"1) Input: A Byte array of arbitrary data." << nl;
	for (int x = 0; x < byteArray1.size(); x++)
	{
		byteArray1[x] = (_byte)x;
		std::cout << std::hex << (int)byteArray1[x] << " ";
		if (((x + 1) % 20) == 0) std::cout << std::endl;
	}
	std::wcout << nl << nl;

	// 2) Convert the input Byte array to a Char array, with newlines inserted.
	charArrayLength = Convert::ToBase64CharArray(byteArray1, 0, byteArray1.size(), charArray, 0, Base64FormattingOptions::InsertLineBreaks);
	std::wcout << L"2) Convert the input Byte array to a Char array with newlines." << std::endl;;
	std::wcout << L"   Output: A Char array (length = "<< charArrayLength << L").";
	std::wcout << L"The elements of the array are:" + nl;
	std::wcout << ruler << std::endl;
	std::string str(charArray.data(), charArray.size());
	std::cout << str << std::endl;
	std::cout << std::endl;

	// 3) Convert the Char array back to a Byte array.
	std::cout << "3) Convert the Char array to an output Byte array." << std::endl;
	byteArray2 = Convert::FromBase64CharArray(charArray, 0, charArrayLength);

	// 4) Are the input and output Byte arrays equivalent?
	bool areEqual = ArraysAreEqual(byteArray1, byteArray2);
	std::cout << "4) The output Byte array is equal to the input Byte array:" << 
		(areEqual == true ? "True" : "False") << std::endl;
}
