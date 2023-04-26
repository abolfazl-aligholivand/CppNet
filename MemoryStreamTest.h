#pragma once
#include "MemoryStream.h"
#include <iostream>
#include <vector>
using namespace std;

void MemoryStreamTest()
{
	using namespace System::IO;
	std::vector<_byte> firstStringBytes = std::vector<_byte>{ 10, 1, 21, 41, 74, 69, 25, 1, 2, 98 };
	std::vector<_byte> secondStringBytes = std::vector<_byte>{ 4,1,2,7,52,4,6,95,25,12,0,36,14 };
	MemoryStream* stream = new MemoryStream(100);
	stream->Write(firstStringBytes, 0, firstStringBytes.size());

	int count = 0;
	while (count < secondStringBytes.size())
	{
		stream->WriteByte(secondStringBytes[count]);
		count++;
	}
	auto buffer = stream->ToArray();
	for (auto b : buffer)
		cout << (int)b << " ";
	cout << endl;
	stream->Seek(0, SeekOrigin::Begin);
	auto byteArray = std::vector<_byte>(stream->Length());
	count = stream->Read(byteArray, 0, 20);
	while (count < stream->Length())
		byteArray[count++] = stream->ReadByte();
	for (auto b : byteArray)
		cout << (int)b << " ";
	cout << endl;
}