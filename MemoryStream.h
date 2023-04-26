#pragma once
#include "Stream.h"
#include "Exception.h"
#include "Buffer.h"
#include <vector>

namespace System
{
	namespace IO
	{
		class MemoryStream : public Stream
		{
		private:
           std::vector<_byte> _buffer;    // Either allocated internally or externally.
           int _origin;       // For user-provided arrays, start at this origin
           int _position;     // read/write head.

           int _length;       // Number of bytes within the memory stream
           int _capacity;     // length of usable portion of buffer for stream
           // Note that _capacity == _buffer.Length for non-user-provided byte[]'s

           bool _expandable;  // User-provided buffers aren't expandable.
           bool _writable;    // Can user write to this stream?
           bool _exposable;   // Whether the array can be returned to the user.
           bool _isOpen;      // Is this stream open or closed?
           const int MemStreamMaxLength = INT_MAX;

        public:
            MemoryStream();
            MemoryStream(int capacity);
            MemoryStream(std::vector<_byte> buffer);
            MemoryStream(std::vector<_byte> buffer, bool writable);
            virtual ~MemoryStream();

            _int64 Length() override;
            _int64 Seek(_int64 offset, SeekOrigin) override;
            _int64 Position() override;
            void Position(_int64 value) override;
            bool CanRead() override;
            bool CanSeek() override;
            bool CanWrite() override;
            void Flush() override;
            int Read(std::vector<_byte>& buffer, int offset, int count) override;
            int ReadByte();
            void SetLength(_int64 value) override;
            void Write(std::vector<_byte>& buffer, int offset, int count) override;
            void WriteByte(_byte value);
            int Capacity();
            int Capacity(int value);
            std::vector<_byte> ToArray();

        private:
            bool EnsureCapacity(int);
		};
	}
}
