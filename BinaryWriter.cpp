#include "BinaryWriter.h"
#include "UTF8Encoding.h"

namespace System
{
	namespace IO
	{
		BinaryWriter::BinaryWriter()
		{
			OutStream = nullptr;
			_buffer = std::vector<_byte>(16);
			_encoding = new UTF8Encoding();
		}

		BinaryWriter::BinaryWriter(Stream* output) : BinaryWriter(output, new UTF8Encoding(), false)
		{

		}
	
		BinaryWriter::BinaryWriter(Stream* output, Encoding* encoding) : BinaryWriter(output, encoding, false)
		{
		}
		
		BinaryWriter::BinaryWriter(Stream* output, Encoding* encoding, bool leaveOpen)
		{
			if (output == nullptr)
				throw "output";
			if (encoding == nullptr)
				throw "encoding";
			if (!output->CanWrite())
				throw "Argument_StreamNotWritable";

			OutStream = output;
			_buffer = std::vector<_byte>(16);
			_encoding = encoding;
			_leaveOpen = leaveOpen;
		}

		BinaryWriter::~BinaryWriter()
		{
			if (OutStream != nullptr) delete OutStream;
			if (_encoding != nullptr) delete _encoding;
			_buffer.clear();
		}

		Stream* BinaryWriter::GetBaseStream()
		{
			return OutStream;
		}

		void BinaryWriter::Write(bool value)
		{
			_buffer = std::vector<_byte>(1);
			_buffer[0] = (_byte)(value ? 1 : 0);
			OutStream->Write(_buffer, 0, 1);
		}

		void BinaryWriter::Write(_byte value)
		{
			OutStream->WriteByte(value);
		}

		void BinaryWriter::Write(const std::wstring value)
		{
			std::string str(value.begin(), value.end());
			for (int i = 0; i < str.size(); i++)
				OutStream->WriteByte((_byte)str[i]);
		}
		
		void BinaryWriter::Write(int8 value)
		{
			OutStream->WriteByte((_byte)value);
		}
		
		void BinaryWriter::Write(std::vector<_byte> buffer)
		{
			if (buffer.empty() || buffer.size() == 0)
				throw "buffer";
			OutStream->Write(buffer, 0, buffer.size());
		}
		
		void BinaryWriter::Write(std::vector<_byte> buffer, int index, int count)
		{
			OutStream->Write(buffer, index, count);
		}
		
		void BinaryWriter::Write(double value)
		{
			uint64 TmpValue = *(uint64*)&value;
			_buffer = std::vector<_byte>(8);
			_buffer[0] = (_byte)TmpValue;
			_buffer[1] = (_byte)(TmpValue >> 8);
			_buffer[2] = (_byte)(TmpValue >> 16);
			_buffer[3] = (_byte)(TmpValue >> 24);
			_buffer[4] = (_byte)(TmpValue >> 32);
			_buffer[5] = (_byte)(TmpValue >> 40);
			_buffer[6] = (_byte)(TmpValue >> 48);
			_buffer[7] = (_byte)(TmpValue >> 56);
			OutStream->Write(_buffer, 0, 8);
		}
		
		void BinaryWriter::Write(short value)
		{
			_buffer = std::vector<_byte>(2);
			_buffer[0] = (_byte)value;
			_buffer[1] = (_byte)(value >> 8);
			OutStream->Write(_buffer, 0, 2);
		}
		
		void BinaryWriter::Write(uint16 value)
		{
			_buffer = std::vector<_byte>(2);
			_buffer[0] = (_byte)value;
			_buffer[1] = (_byte)(value >> 8);
			OutStream->Write(_buffer, 0, 2);
		}
		
		void BinaryWriter::Write(int value)
		{
			_buffer = std::vector<_byte>(4);
			_buffer[0] = (_byte)value;
			_buffer[1] = (_byte)(value >> 8);
			_buffer[2] = (_byte)(value >> 16);
			_buffer[3] = (_byte)(value >> 24);
			OutStream->Write(_buffer, 0, 4);
		}
		
		void BinaryWriter::Write(uint32 value)
		{
			_buffer = std::vector<_byte>(4);
			_buffer[0] = (_byte)value;
			_buffer[1] = (_byte)(value >> 8);
			_buffer[2] = (_byte)(value >> 16);
			_buffer[3] = (_byte)(value >> 24);
			OutStream->Write(_buffer, 0, 4);
		}
		
		void BinaryWriter::Write(_int64 value)
		{
			_buffer = std::vector<_byte>(8);
			_buffer[0] = (_byte)value;
			_buffer[1] = (_byte)(value >> 8);
			_buffer[2] = (_byte)(value >> 16);
			_buffer[3] = (_byte)(value >> 24);
			_buffer[4] = (_byte)(value >> 32);
			_buffer[5] = (_byte)(value >> 40);
			_buffer[6] = (_byte)(value >> 48);
			_buffer[7] = (_byte)(value >> 56);
			OutStream->Write(_buffer, 0, 8);
		}
		
		void BinaryWriter::Write(uint64 value)
		{
			_buffer = std::vector<_byte>(8);
			_buffer[0] = (_byte)value;
			_buffer[1] = (_byte)(value >> 8);
			_buffer[2] = (_byte)(value >> 16);
			_buffer[3] = (_byte)(value >> 24);
			_buffer[4] = (_byte)(value >> 32);
			_buffer[5] = (_byte)(value >> 40);
			_buffer[6] = (_byte)(value >> 48);
			_buffer[7] = (_byte)(value >> 56);
			OutStream->Write(_buffer, 0, 8);
		}
		
		void BinaryWriter::Write(float value)
		{
			uint32 TmpValue = *(uint32*)&value;
			_buffer = std::vector<_byte>(4);
			_buffer[0] = (_byte)TmpValue;
			_buffer[1] = (_byte)(TmpValue >> 8);
			_buffer[2] = (_byte)(TmpValue >> 16);
			_buffer[3] = (_byte)(TmpValue >> 24);
			OutStream->Write(_buffer, 0, 4);
		}

		void BinaryWriter::Write7BitEncodedInt(int value)
		{
			// Write out an int 7 bits at a time.  The high bit of the byte,
		   // when on, tells reader to continue reading more bytes.
			uint32 v = (uint32)value;   // support negative numbers
			while (v >= 0x80) {
				Write((_byte)(v | 0x80));
				v >>= 7;
			}
			Write((_byte)v);
		}
	}
}