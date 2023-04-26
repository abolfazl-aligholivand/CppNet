#include "MemoryStream.h"

namespace System
{
	namespace IO
	{
		MemoryStream::MemoryStream() : MemoryStream(0)
		{
		}

		MemoryStream::MemoryStream(int capacity)
		{
			this->_buffer = std::vector<_byte>(capacity);
			this->_capacity = capacity;
			this->_expandable = true;
			this->_writable = true;
			this->_exposable = true;
			this->_origin = 0;      // Must be 0 for byte[]'s created by MemoryStream
			this->_isOpen = true;
		}

		MemoryStream::MemoryStream(std::vector<_byte> buffer) : MemoryStream(buffer, true)
		{
		}

		MemoryStream::MemoryStream(std::vector<_byte> buffer, bool writable)
		{
			if (buffer.empty() || buffer.size() == 0)
				throw new ArgumentNullException(L"ArgumentNull_Buffer");

			this->_buffer = buffer;
			this->_length = this->_capacity = buffer.size();
			this->_expandable = true;
			this->_writable = true;
			this->_exposable = true;
			this->_origin = 0;      // Must be 0 for _byte[]'s created by MemoryStream
			this->_isOpen = true;
		}

		MemoryStream::~MemoryStream()
		{

		}

		_int64 MemoryStream::Length()
		{
			if (!_isOpen)
				return -1;
			return (_int64)(_length - _origin);
		}
		_int64 MemoryStream::Seek(_int64 offset, SeekOrigin loc)
		{
			if (offset > MemStreamMaxLength)
				throw "Offset out of range";

			switch (loc)
			{
			case System::IO::Begin:
			{
				int tempPoition = _origin + (int)offset;
				if (offset < 0 || tempPoition < _origin)
					throw "IO.IO_SeekBeforeBegin";
				this->_position = tempPoition;
				break;
			}
			case System::IO::Current:
			{
				int tempPoition = _position + (int)offset;
				if (offset < 0 || tempPoition < _origin)
					throw "IO.IO_SeekBeforeBegin";
				this->_position = tempPoition;
				break;
			}
			case System::IO::End:
			{
				int tempPoition = _length + (int)offset;
				if (offset < 0 || tempPoition < _origin)
					throw "IO.IO_SeekBeforeBegin";
				this->_position = tempPoition;
				break;
			}
			default:
				throw "Argument_InvalidSeekOrigin";
			}
			return this->_position;
		}

		_int64 MemoryStream::Position()
		{
			if (!_isOpen) return -1;
			return _position - _origin;
		}
		void MemoryStream::Position(_int64 value)
		{
			if (value < 0)
				throw "ArgumentOutOfRange_NeedNonNegNum";

			if (!_isOpen) return;

			if (value > MemStreamMaxLength)
				throw "ArgumentOutOfRange_StreamLength";
			_position = _origin + (int)value;
		}

		bool MemoryStream::CanRead()
		{
			return _isOpen;
		}

		bool MemoryStream::CanSeek()
		{
			return _isOpen;
		}

		bool MemoryStream::CanWrite()
		{
			return _writable;
		}
		void MemoryStream::Flush()
		{

		}

		int MemoryStream::Read(std::vector<_byte>& buffer, int offset, int count)
		{
			if (_position >= _length || count == 0)
				return 0;

			if (_position > _length - count)
				count = _length - _position;

			if (buffer.size() == 0 || buffer.empty())
				buffer = std::vector<_byte>(offset + count);

			Buffer::BlockCopy(this->_buffer, _position, buffer, offset, count);

			_position += count;
			return count;
		}

		int MemoryStream::ReadByte()
		{
			if (_position >= _length)
				return -1;
			return _buffer[_position++];
		}

		void MemoryStream::SetLength(_int64 value)
		{
			int newLength = _origin + (int)value;
			_length = newLength;

			if (_position > newLength) _position = newLength;

			bool allocatedNewArray = EnsureCapacity(newLength);
			if (!allocatedNewArray && newLength > _length)
			{
				int len = newLength - _length, index = _length;
				for (int i = index; i < index + len; i++)
					_buffer[i] = 0;
			}
			_length = newLength;
			if (_position > newLength) _position = newLength;
		}

		void MemoryStream::Write(std::vector<_byte>& buffer, int offset, int count)
		{
			if (buffer.empty() || buffer.size()==0)
				throw new ArgumentNullException(L"ArgumentNull_Buffer");
			if (offset < 0)
				throw "ArgumentOutOfRange_NeedNonNegNum";
			if (count < 0)
				throw "ArgumentOutOfRange_NeedNonNegNum";
			if (buffer.size() - offset < count)
				throw "Argument_InvalidOffLen";

			if (!_isOpen) return;

			int i = _position + count;
			// Check for overflow
			if (i < 0)
				throw "IO.IO_StreamTooLong";

			if (i > _length) {
				bool mustZero = _position > _length;
				if (i > _capacity) {
					bool allocatedNewArray = EnsureCapacity(i);
					if (allocatedNewArray)
						mustZero = false;
				}
				if (mustZero)
				{
					int len = i - _length, index = _length;
					for (int i = index; i < index + len; i++)
						_buffer[i] = 0;
				}
				_length = i;
			}
			if ((count <= 8) && (buffer != _buffer))
			{
				int byteCount = count;
				while (--byteCount >= 0)
					_buffer[_position + byteCount] = buffer[offset + byteCount];
			}
			else
				Buffer::InternalBlockCopy(buffer, offset, _buffer, _position, count);
			_position = i;
		}

		void MemoryStream::WriteByte(_byte value)
		{
			if (!_isOpen) return;

			if (_position >= _length) {
				int newLength = _position + 1;
				bool mustZero = _position > _length;
				if (newLength >= _capacity) {
					bool allocatedNewArray = EnsureCapacity(newLength);
					if (allocatedNewArray)
						mustZero = false;
				}
				if (mustZero)
				{
					int len = _position - _length, index = _length;
					for (int i = index; i < index + len; i++)
						_buffer[i] = 0;
				}
				_length = newLength;
			}
			_buffer[_position++] = value;
		}

		std::vector<_byte> MemoryStream::ToArray()
		{
			auto len = _length - _origin;
			std::vector<_byte> copy = std::vector<_byte>(len);
			Buffer::InternalBlockCopy(_buffer, _origin, copy, 0, len);
			return copy;
		}

		int MemoryStream::Capacity()
		{
			return _capacity - _origin;
		}

		int MemoryStream::Capacity(int value)
		{
			if (!_isOpen) return -1;
			if (!_expandable && (value != Capacity())) return -1;

			// MemoryStream has this invariant: _origin > 0 => !expandable (see ctors)
			if (_expandable && value != _capacity) {
				if (value > 0) {
					std::vector<_byte> newBuffer = std::vector<_byte>(value);
					if (_length > 0)
						Buffer::InternalBlockCopy(_buffer, 0, newBuffer, 0, _length);
					_buffer = newBuffer;
				}
				else {
					_buffer = std::vector<_byte>();
				}
				_capacity = value;
			}
		}

		bool  MemoryStream::EnsureCapacity(int value)
		{
			if (value < 0)
				throw "IO.IO_StreamTooLong";

			if (value > _capacity) {
				int newCapacity = value;
				if (newCapacity < 256)
					newCapacity = 256;
				// We are ok with this overflowing since the next statement will deal
				// with the cases where _capacity*2 overflows.
				if (newCapacity < _capacity * 2)
					newCapacity = _capacity * 2;
				// We want to expand the array up to Array.MaxArrayLengthOneDimensional
				// And we want to give the user the value that they asked for
				if ((unsigned int)(_capacity * 2) > 0x7FFFFFC7)
					newCapacity = value > 0x7FFFFFC7 ? value : 0x7FFFFFC7;

				Capacity(newCapacity);
				return true;
			}
			return false;
		}
	}
}