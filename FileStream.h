#pragma once
#include "Stream.h"
#include "FileAccess.h"
#include "FileMode.h"
#include "FileShare.h"
#include "FileOptions.h"
#include "Buffer.h"
#include "Path.h"
#include "pch.h"
#include <fstream>

namespace System
{
	namespace IO
	{
		class FileStream : public Stream
		{
		public:
			FileStream(const std::wstring path, FileMode mode, FileAccess access, FileShare share);
			FileStream(const std::wstring path, FileMode mode, FileAccess access, FileShare share, int bufferSize);
			~FileStream();

		private:
			int ReadCore(std::vector<_byte>& buffer, int offset, int count);
			int ReadFileNative(std::vector<_byte>& bytes, int offset, int count);
			void Init();
			void WriteStream();
			bool IsCreatedMode();

		public:
			_int64 Length() override;
			_int64 Seek(_int64 offset, SeekOrigin origin) override;
			_int64 Position() override;
			void Position(_int64 vlaue) override;
			bool CanRead() override;
			bool CanSeek() override;
			bool CanWrite() override;
			void Flush() override;
			int Read(std::vector<_byte>& buffer, int offset, int count) override;
			int ReadByte();
			void SetLength(_int64 length) override;
			void Write(std::vector<_byte>&, int, int) override;
			void WriteByte(_byte) override;
			std::wstring GetName();

		private:
			std::ofstream stream;
			const int DefaultBufferSize = 4096;
			const bool _canUseAsync = true;
			std::vector<_byte> _buffer;   // Shared read/write buffer.  Alloc on first use.
			const std::wstring _path;
			std::wstring _fileName; // Fully qualified file name.
			bool _isAsync;    // Whether we opened the handle for overlapped IO
			FileMode _mode;
			bool _canRead;
			bool _canWrite;
			bool _canSeek;
			int _origin;
			int _len;  // Length of internal buffer, if it's allocated.
			long _pos;        // Cache current location in the file.
		};
	}
}