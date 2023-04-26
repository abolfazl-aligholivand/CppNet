#include "Stream.h"

namespace System
{
	namespace IO
	{
		Stream::Stream()
		{

		}

		Stream::~Stream()
		{

		}

		int Stream::ReadByte()
		{
			std::vector<_byte> oneByteArray = std::vector<_byte>(1);
			int r = Read(oneByteArray, 0, 1);
			if (r == 0)
				return -1;
			return oneByteArray[0];
		}

		void Stream::WriteByte(_byte value)
		{
			std::vector<_byte> oneByteArray = std::vector<_byte>(1);
			oneByteArray[0] = value;
			Write(oneByteArray, 0, 1);
		}
	}
}
