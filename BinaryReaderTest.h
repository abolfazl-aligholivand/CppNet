#pragma once
#include "BinaryReader.h"
#include "MemoryStream.h"
#include "FileStream.h"

void BinaryReaderTest()
{
	using namespace System::IO;
	using namespace System::Text;

	std::wstring path = L"/home/aligholi/Desktop/empty.txt";
	FileStream* fileStream = new FileStream(path, FileMode::Open, FileAccess::Read, FileShare::None);
	std::vector<_byte> buffer;
	int n = fileStream->Read(buffer, 0, fileStream->Length());
	MemoryStream* memoryStream = new MemoryStream(buffer);

	BinaryReader* reader = new BinaryReader(memoryStream);
	
	bool readBoolean = reader->ReadBoolean();
	std::cout << "Boolean:  " << readBoolean << std::endl;

	_byte readByte = reader->ReadByte();
	std::cout << "Byte:     " << (int)readByte << std::endl;

	int8 readSByte = reader->ReadSByte();
	std::cout << "SByte:    " << (int)readSByte << std::endl;

	short readInt16 = reader->ReadInt16();
	std::cout << "Int16:    " << readInt16 << std::endl;

	uint16 readUInt16 = reader->ReadUInt16();
	std::cout << "UInt16:   " << readUInt16 << std::endl;

	int readInt32 = reader->ReadInt32();
	std::cout << "Int32:    " << readInt32 << std::endl;

	uint32 readUInt32 = reader->ReadUInt32();
	std::cout << "UInt32:   " << readUInt32 << std::endl;

	_int64 readInt64 = reader->ReadInt64();
	std::cout << "Int64:    " << readInt64 << std::endl;

	_int64 readUInt64 = reader->ReadUInt64();
	std::cout << "UInt64:   " << readUInt64 << std::endl;

	float readSingle = reader->ReadSingle();
	std::cout << "Single:   " << readSingle << std::endl;

	double readDouble = reader->ReadDouble();
	std::cout << "Double:   " << readDouble << std::endl;

	std::vector<_byte> bytes = reader->ReadBytes(10);
	std::cout << "Bytes:    ";
	for (int b = 0; b < bytes.size(); b++)
		std::cout << (int)bytes[b] << " ";
	std::cout  << std::endl;
}