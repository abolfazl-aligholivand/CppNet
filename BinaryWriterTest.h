#pragma once
#include "BinaryWriter.h"
#include "MemoryStream.h"

void BinaryWriterTest()
{
	using namespace System::IO;

	std::vector<_byte> buffer;
	for (int i = 0; i < 50; i++)
		buffer.push_back(i * 2);
	MemoryStream* stream = new MemoryStream(buffer);
	BinaryWriter* writer = new BinaryWriter(stream);

	writer->Write(1.250f);
	writer->Write((int)10);
	writer->Write((bool)true);
	std::vector<_byte> bytes1 = std::vector<_byte>{ 1, 3, 5, 7 };
	writer->Write(bytes1);
	std::vector<_byte> bytes2 = std::vector<_byte>{ 9, 11, 13, 15, 17, 19, 21, 23, 25 };
	writer->Write(bytes2, 2, 5);

	auto baseStream = writer->GetBaseStream();
	std::cout << "Stream Length: " << baseStream->Length() << std::endl;
	std::cout << "Stream Position: " << baseStream->Position() << std::endl;
	baseStream->Position(0);
	std::vector<_byte> readBytes;
	baseStream->Read(readBytes, 0, baseStream->Length());
	for (int i = 0; i < readBytes.size(); i++)
		std::cout << (int)readBytes[i] << " ";
	std::cout << std::endl;
}