#pragma once
#include "Stream.h"
#include "MemoryStream.h"
#include "Encoding.h"
using namespace System::Text;

namespace System
{
	namespace IO
	{
		class BinaryReader
		{
		public:
			BinaryReader(Stream* input);
			BinaryReader(Stream* input, Encoding* encoding);
			BinaryReader(Stream* input, Encoding* encoding, bool leaveOpen);
			~BinaryReader();

			//Methods
		public:
			virtual Stream* GetBaseStream();
			//virtual int PeekChar();
			virtual bool ReadBoolean();
			virtual _byte ReadByte();
			virtual int8 ReadSByte();
			//virtual char ReadChar();
			virtual short ReadInt16();
			virtual uint16 ReadUInt16();
			virtual int ReadInt32();
			virtual uint32 ReadUInt32();
			virtual _int64 ReadInt64();
			virtual uint64 ReadUInt64();
			virtual float ReadSingle();
			virtual double ReadDouble();
			virtual std::wstring ReadString();
			//virtual int Read();
			//virtual int Read(std::vector<char>& buffer, int index, int count);
			virtual int Read(std::vector<_byte>& buffer, int index, int count);
			virtual std::vector<_byte> ReadBytes(int count);
		private:
			//int InternalReadChars(std::vector<char>& buffer, int index, int count);
			//int InternalReadOneChar();

		protected:
			virtual void FillBuffer(int numBytes);
			int Read7BitEncodedInt();

		private:
			const int MaxCharBytesSize = 128;
			Stream* m_stream;
			Encoding* _encoding;
			std::vector<_byte> m_buffer;
			std::vector<_byte> m_charBytes;
			std::vector<char> m_singleChar;
			//bool m_2BytesPerChar;
			bool m_isMemoryStream;
			bool m_leaveOpen;
		};
	}
}
