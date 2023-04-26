#pragma once
#include "pch.h"
#include "SeekOrigin.h"

namespace System
{
	namespace IO
	{
		class Stream 
		{
		protected:
			Stream();

		public:
            virtual ~Stream();
            virtual _int64 Length() = 0;
            virtual _int64 Seek(_int64, SeekOrigin) = 0;
            virtual _int64 Position() = 0;
            virtual void Position(_int64) = 0;
            virtual bool CanRead() = 0;
            virtual bool CanSeek() = 0;
            virtual bool CanWrite() = 0;
            virtual void Flush() = 0;
            virtual int Read(std::vector<_byte> &buffer, int offset, int count) = 0;
            virtual int ReadByte();
            virtual void SetLength(_int64) = 0;
            virtual void Write(std::vector<_byte>&, int, int) = 0;
            virtual void WriteByte(_byte);
		};
	}
}
