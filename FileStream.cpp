#include "FileStream.h"
#include "File.h"

namespace System
{
	namespace IO
	{
		FileStream::FileStream(const std::wstring path, FileMode mode, FileAccess access, FileShare share)
			: FileStream(path, mode, access, share, DefaultBufferSize)
		{
		}

		FileStream::FileStream(const std::wstring path, FileMode mode, FileAccess access, FileShare share, int bufferSize) :
			_path(path),
			_fileName(Path::GetFileName(path)),
			_canSeek(true),
			_pos(0),
			_len(bufferSize),
			_isAsync(false),
			_origin(0),
			_canRead(false),
			_canWrite(false),
			_mode(mode)
		{
			if (path == L"")
				throw "Argument Null";
			switch (access)
			{
			case FileAccess::Read:
				_canRead = true;
				_canWrite = false;
				break;
			case FileAccess::Write:
				_canRead == false;
				_canWrite = true;
				break;
			case FileAccess::ReadWrite:
				_canRead = true;
				_canWrite = true;
				break;
			default:
				break;
			}
			Init();
		}

		FileStream::~FileStream()
		{
			stream.close();
		}

		void FileStream::Init()
		{
			if (_path == L"")
				throw "Path is empty";

			if (IsCreatedMode())
			{
				std::ofstream open(_path.c_str());
				open << "";
				open.close();
			}
			if (CanRead())
			{
				std::ifstream ifs(_path, std::ios::binary | std::ios::in);
				if (!File::Exists(_path))
					throw "File not exist";

				ifs.seekg(0, ifs.end);
				int length = ifs.tellg();
				ifs.seekg(0, ifs.beg);
				char* buffer = new char[length];
				_buffer = std::vector<_byte>(0);
				ifs.read(buffer, length);
				for (int i = 0; i < length; i++)
					_buffer.push_back((_byte)buffer[i]);
				_len = length;
				ifs.close();
				delete[] buffer;
			}
			if (CanWrite())
			{
				stream.open(_path.c_str(), std::ofstream::out | std::ofstream::binary);
			}
		}

		_int64 FileStream::Length()
		{
			return _len;
		}

		bool FileStream::IsCreatedMode()
		{
			if (_mode == FileMode::Create || _mode == FileMode::CreateNew || _mode == FileMode::OpenOrCreate)
				return true;
			else
				return false;
		}

		_int64 FileStream::Seek(_int64 offset, SeekOrigin origin)
		{
			switch (origin)
			{
			case System::IO::Begin:
			{
				int tempPoition = _origin + (int)offset;
				if (offset < 0 || tempPoition < _origin)
					throw "IO.IO_SeekBeforeBegin";
				this->_pos = tempPoition;
				break;
			}
			case System::IO::Current:
			{
				if (offset < _origin)
					throw "IO.IO_SeekBeforeBegin";
				this->_pos = offset;
				break;
			}
			case System::IO::End:
			{
				int tempPoition = _len + (int)offset;
				if (offset < 0 || tempPoition < origin)
					throw "IO.IO_SeekBeforeBegin";
				this->_pos = tempPoition;
				break;
			}
			default:
				throw "Argument_InvalidSeekOrigin";
			}
			return _pos;
		}

		_int64 FileStream::Position()
		{
			return _pos;
		}

		void FileStream::Position(_int64 value)
		{
			Seek(value, SeekOrigin::Current);
		}

		bool FileStream::CanRead()
		{
			return _canRead;
		}

		bool FileStream::CanSeek()
		{
			if (_pos >= Length())
				_canSeek = false;
			return _canSeek;
		}

		bool FileStream::CanWrite()
		{
			return _canWrite;
		}

		void FileStream::Flush()
		{
			throw "Not Implementation";
		}

		int FileStream::Read(std::vector<_byte>& buffer, int offset, int count)
		{
			if (!CanRead()) return -1;
			int readedBytes = 0;
			if (CanSeek())
			{
				if (buffer.empty() || buffer.size() == 0)
					buffer = std::vector<_byte>(count);
				readedBytes = ReadCore(buffer, offset, count);
				return readedBytes;
			}

			return readedBytes;
		}

		int FileStream::ReadByte()
		{
			if (!CanRead()) return -1;
			if (!CanSeek())
				if (Position() >= Length())
					return 0;
			auto buffer = std::vector<_byte>(1);
			int _readLen = ReadCore(buffer, 0, 1);
			int result = buffer[0];
			return result;
		}

		int FileStream::ReadCore(std::vector<_byte>& buffer, int offset, int count)
		{
			if (buffer.empty() || buffer.size() == 0)
				throw "ArgumentNull_Buffer";
			if (offset < 0)
				throw "ArgumentOutOfRange_NeedNonNegNum";
			if (count < 0)
				throw "ArgumentOutOfRange_NeedNonNegNum";
			if (buffer.size() - offset < count)
				throw "Argument_InvalidOffLen";

			int r = ReadFileNative(buffer, offset, count);
			if (r == -1)
			{
				return -1;
			}
			return r;
		}

		int FileStream::ReadFileNative(std::vector<_byte>& bytes, int offset, int count)
		{
			if (bytes.size() == 0) {
				return 0;
			}

			int r = 0;
			int numBytesRead = 0;
			int i = 0, len = Length();

			if (_pos + count <= len)
				while (i < count)
				{
					bytes[offset] = _buffer[_pos];
					numBytesRead++;
					i++;
					_pos++;
					offset++;
				}
			else
				while (_pos < len)
				{
					bytes[offset] = _buffer[_pos];
					numBytesRead++;
					i++;
					_pos++;
					offset++;
				}

			return numBytesRead;
		}

		void FileStream::SetLength(_int64 length)
		{
			if (length < 0)
				throw "ArgumentOutOfRange_NeedNonNegNum";
			if (!CanWrite()) return;

			long origPos = _pos;
			if (_pos != length)
				Seek(length, SeekOrigin::Begin);
			if (origPos != length)
			{
				if (origPos < length)
					Seek(origPos, SeekOrigin::Begin);
				else
					Seek(0, SeekOrigin::End);
			}
			_len = length;
		}

		void FileStream::Write(std::vector<_byte>& bytes, int offset, int count)
		{
			if (bytes.empty() || bytes.size() == 0)
				throw "ArgumentNull_Buffer";
			if (offset < 0)
				throw "ArgumentOutOfRange_NeedNonNegNum";
			if (count < 0)
				throw "ArgumentOutOfRange_NeedNonNegNum";
			if (bytes.size() - offset < count)
				throw "Argument_InvalidOffLen";

			if (CanWrite())
			{
				if (_pos == Length())
				{
					long newLen = Length() + count;
					SetLength(newLen);
					_buffer.resize(newLen);
					Buffer::InternalBlockCopy(bytes, offset, _buffer, _pos, count);
					_pos += count;
					WriteStream();
				}
				else if (_pos + count > Length())
				{
					long newLen = _pos + count;
					SetLength(newLen);
					_buffer.resize(newLen);
					Buffer::InternalBlockCopy(bytes, offset, _buffer, _pos, count);
					_pos += count;
					WriteStream();
				}
				else if (_pos + count < Length())
				{
					Buffer::InternalBlockCopy(bytes, offset, _buffer, _pos, count);
					_pos += count;
					WriteStream();
				}
			}
		}

		void FileStream::WriteStream()
		{
			stream.seekp(0);
			stream.write(reinterpret_cast<char*>(_buffer.data()), Length());
		}

		void FileStream::WriteByte(_byte value)
		{
			std::vector<_byte> bytes{ value };
			Write(bytes, 0, 1);
		}

		std::wstring FileStream::GetName()
		{
			if (_fileName == L"")
				throw "IO_UnknownFileName";

			return _fileName;
		}
	}
}