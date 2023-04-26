#pragma once
#include "Stream.h"
#include "Encoding.h"
using namespace System::Text;

namespace System
{
	namespace IO
	{
		class BinaryWriter
		{
		public:
			BinaryWriter();
			BinaryWriter(Stream* output);
			BinaryWriter(Stream* output, Encoding* encoding);
			BinaryWriter(Stream* output, Encoding* encoding, bool leaveOpen);
			~BinaryWriter();

		public:
			Stream* GetBaseStream();
			virtual void Write(bool value);
			virtual void Write(_byte value);
			virtual void Write(int8 value);
			void Write(const std::wstring value);
			virtual void Write(std::vector<_byte> buffer);
			virtual void Write(std::vector<_byte> buffer, int index, int count);
			virtual void Write(double value);
			virtual void Write(short value);
			virtual void Write(uint16 value);
			virtual void Write(int value);
			virtual void Write(uint32 value);
			virtual void Write(_int64 value);
			virtual void Write(uint64 value);
			virtual void Write(float value);

		protected:
			void Write7BitEncodedInt(int value);

		protected:
			Stream* OutStream;
			Encoding* _encoding;

		private:
			std::vector<_byte> _buffer;
			const int LargeByteBufferSize = 256;
			bool _leaveOpen;
		};
	}
}