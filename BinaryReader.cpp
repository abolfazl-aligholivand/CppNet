#include "BinaryReader.h"
#include "UTF8Encoding.h"

namespace System
{
	namespace IO
	{
		BinaryReader::BinaryReader(Stream* input) : BinaryReader(input, new UTF8Encoding(), false)
		{
		}

		BinaryReader::BinaryReader(Stream* input, Encoding* encoding) : BinaryReader(input, encoding, false)
		{
		}
		BinaryReader::BinaryReader(Stream* input, Encoding* encoding, bool leaveOpen)
		{
			if (input == nullptr) {
				throw "input";
			}
			if (encoding == nullptr) {
				throw "encoding";
			}
			//if (!input->CanRead())
			//	throw "Argument_StreamNotReadable";

			this->m_stream = input;
			this->m_isMemoryStream = typeid(input).name() == typeid(MemoryStream).name();
			this->m_leaveOpen = leaveOpen;
			_encoding = encoding;
		}

		BinaryReader::~BinaryReader()
		{
			if (m_stream != nullptr) delete m_stream;
			if (_encoding != nullptr) delete _encoding;
			m_buffer.clear();
			m_charBytes.clear();
			m_singleChar.clear();
		}

		Stream* BinaryReader::GetBaseStream()
		{
			return m_stream;
		}

		//int BinaryReader::PeekChar()
		//{
		//	if (m_stream == nullptr) return -1;
		//	long origPos = m_stream->Position();
		//	int ch = Read();
		//	m_stream->Position(origPos);
		//	return ch;
		//}

		bool BinaryReader::ReadBoolean()
		{
			FillBuffer(1);
			return (m_buffer[0] != 0);
		}

		_byte BinaryReader::ReadByte()
		{
			int b = m_stream->ReadByte();
			if (b == -1)
				return -1;
			return (_byte)b;
		}

		int8 BinaryReader::ReadSByte()
		{
			FillBuffer(1);
			return (int8)(m_buffer[0]);
		}

		//char BinaryReader::ReadChar()
		//{
		//	int value = Read();
		//	if (value == -1)
		//		return -1;
		//	return (char)value;
		//}

		short BinaryReader::ReadInt16()
		{
			FillBuffer(2);
			return (short)(m_buffer[0] | m_buffer[1] << 8);
		}

		uint16 BinaryReader::ReadUInt16()
		{
			FillBuffer(2);
			return (uint16)(m_buffer[0] | m_buffer[1] << 8);
		}

		int BinaryReader::ReadInt32()
		{
			FillBuffer(4);
			return (int)(m_buffer[0] | m_buffer[1] << 8 | m_buffer[2] << 16 | m_buffer[3] << 24);
		}

		uint32 BinaryReader::ReadUInt32()
		{
			FillBuffer(4);
			return (unsigned int)(m_buffer[0] | m_buffer[1] << 8 | m_buffer[2] << 16 | m_buffer[3] << 24);
		}

		_int64 BinaryReader::ReadInt64()
		{
			FillBuffer(8);
			unsigned int lo = (unsigned int)(m_buffer[0] | m_buffer[1] << 8 |
				m_buffer[2] << 16 | m_buffer[3] << 24);
			unsigned int hi = (unsigned int)(m_buffer[4] | m_buffer[5] << 8 |
				m_buffer[6] << 16 | m_buffer[7] << 24);
			return (_int64)((_int64)hi) << 32 | lo;
		}

		uint64 BinaryReader::ReadUInt64()
		{
			FillBuffer(8);
			unsigned int lo = (unsigned int)(m_buffer[0] | m_buffer[1] << 8 |
				m_buffer[2] << 16 | m_buffer[3] << 24);
			unsigned int hi = (unsigned int)(m_buffer[4] | m_buffer[5] << 8 |
				m_buffer[6] << 16 | m_buffer[7] << 24);
			return (uint64)((_int64)hi) << 32 | lo;
		}

		float BinaryReader::ReadSingle()
		{
			FillBuffer(4);
			unsigned int tmpBuffer = (unsigned int)(m_buffer[0] | m_buffer[1] << 8 | m_buffer[2] << 16 | m_buffer[3] << 24);
			return *((float*)&tmpBuffer);
		}

		double BinaryReader::ReadDouble()
		{
			FillBuffer(8);
			unsigned int lo = (unsigned int)(m_buffer[0] | m_buffer[1] << 8 |
				m_buffer[2] << 16 | m_buffer[3] << 24);
			unsigned int hi = (unsigned int)(m_buffer[4] | m_buffer[5] << 8 |
				m_buffer[6] << 16 | m_buffer[7] << 24);

			_int64 tmpBuffer = ((_int64)hi) << 32 | lo;
			return *((double*)&tmpBuffer);
		}

		//int BinaryReader::Read()
		//{
		//	if (m_stream == nullptr) return -1;
		//	return InternalReadOneChar();
		//}


		//int BinaryReader::Read(std::vector<char>& buffer, int index, int count)
		//{
		//	if (buffer.empty() || buffer.size() == 0)
		//		throw "buffer_ArgumentNull_Buffer";
		//	if (index < 0)
		//		throw "index_ArgumentOutOfRange_NeedNonNegNum";

		//	if (count < 0)
		//		throw "count_ArgumentOutOfRange_NeedNonNegNum";

		//	if (buffer.size() - index < count)
		//		throw "Argument_InvalidOffLen";

		//	if (m_stream == nullptr)
		//		return -1;

		//	return InternalReadChars(buffer, index, count);
		//}

		int BinaryReader::Read(std::vector<_byte>& buffer, int index, int count)
		{
			if (buffer.empty() || buffer.size() == 0)
				throw "buffer_ArgumentNull_Buffer";
			if (index < 0)
				throw "index_ArgumentOutOfRange_NeedNonNegNum";

			if (count < 0)
				throw "count_ArgumentOutOfRange_NeedNonNegNum";

			if (buffer.size() - index < count)
				throw "Argument_InvalidOffLen";

			if (m_stream == nullptr) return -1;
			return m_stream->Read(buffer, index, count);
		}

		std::vector<_byte> BinaryReader::ReadBytes(int count)
		{
			if (count < 0) "count_ArgumentOutOfRange_NeedNonNegNum";

			std::vector<_byte> result;
			if (m_stream == nullptr) result;

			if (count == 0) {
				return result;
			}

			result = std::vector<_byte>(count);

			int numRead = 0;
			do {
				int n = m_stream->Read(result, numRead, count);
				if (n == 0)
					break;
				numRead += n;
				count -= n;
			} while (count > 0);

			if (numRead != result.size()) {
				// Trim array.  This should happen on EOF & possibly net streams.
				std::vector<_byte> copy = std::vector<_byte>(numRead);
				Buffer::InternalBlockCopy(result, 0, copy, 0, numRead);
				result = copy;
			}

			return result;
		}

		std::wstring BinaryReader::ReadString()
		{
			int currPos = 0;
			int n;
			int stringLength;
			int readLength;
			int charsRead;

			stringLength = Read7BitEncodedInt();
			if (stringLength < 0)
				throw "IO.IO_InvalidStringLen_Len";
			if (stringLength == 0)
				return L"";
			if (m_charBytes.empty())
				m_charBytes = std::vector<_byte>(MaxCharBytesSize);

			Encoding* m_decoder = Encoding::UTF8();

			std::wstring sb = L"";
			do
			{
				readLength = ((stringLength - currPos) > MaxCharBytesSize) ? MaxCharBytesSize : (stringLength - currPos);

				n = m_stream->Read(m_charBytes, 0, readLength);
				if (n == 0) {
					throw "EndOfFile";
				}

				std::vector<_byte> m_charBuffer;
				for (int i = 0; i < n; i++)
					m_charBuffer.push_back(m_charBytes[i]);

				auto s = m_decoder->GetString(m_charBuffer);

				if (currPos == 0 && n == stringLength)
					return s;

				sb += s;
				currPos += n;

			} while (currPos < stringLength);

			return sb;
		}

		//int BinaryReader::InternalReadChars(std::vector<char>& buffer, int index, int count)
		//{
		//	int numBytes = 0;
		//	int charsRemaining = count;

		//	if (m_charBytes.empty() || m_charBytes.size() == 0)
		//	{
		//		m_charBytes = std::vector<_byte>(MaxCharBytesSize);
		//	}

		//	while (charsRemaining > 0) {
		//		int charsRead = 0;
		//		// We really want to know what the minimum number of bytes per char
		//		// is for our encoding.  Otherwise for UnicodeEncoding we'd have to
		//		// do ~1+log(n) reads to read n characters.
		//		numBytes = charsRemaining;

		//		if (m_2BytesPerChar)
		//			numBytes <<= 1;
		//		if (numBytes > MaxCharBytesSize)
		//			numBytes = MaxCharBytesSize;

		//		int position = 0;
		//		std::vector<_byte> byteBuffer;

		//		numBytes = m_stream->Read(m_charBytes, 0, numBytes);
		//		byteBuffer = m_charBytes;

		//		if (numBytes == 0) {
		//			return (count - charsRemaining);
		//		}

		//		if (position < 0 || numBytes < 0 || position + numBytes > byteBuffer.size()) {
		//			throw "byteCount";
		//		}

		//		if (index < 0 || charsRemaining < 0 || index + charsRemaining > buffer.size()) {
		//			throw "charsRemaining";
		//		}


		//		charsRemaining -= charsRead;
		//		index += charsRead;
		//	}
		//}

		//int BinaryReader::InternalReadOneChar()
		//{
		//	// I know having a separate InternalReadOneChar method seems a little 
		//   // redundant, but this makes a scenario like the security parser code
		//   // 20% faster, in addition to the optimizations for UnicodeEncoding I
		//   // put in InternalReadChars.   
		//	int charsRead = 0;
		//	int numBytes = 0;
		//	long posSav = posSav = 0;

		//	if (m_charBytes.empty() || m_charBytes.size() == 0) {
		//		m_charBytes = std::vector<_byte>(MaxCharBytesSize); //
		//	}
		//	if (m_singleChar.empty() || m_singleChar.size() == 0) {
		//		m_singleChar = std::vector<char>(1);
		//	}

		//	while (charsRead == 0) {
		//		// We really want to know what the minimum number of bytes per char
		//		// is for our encoding.  Otherwise for UnicodeEncoding we'd have to
		//		// do ~1+log(n) reads to read n characters.
		//		// Assume 1 byte can be 1 char unless m_2BytesPerChar is true.
		//		numBytes = m_2BytesPerChar ? 2 : 1;

		//		int r = m_stream->ReadByte();
		//		m_charBytes[0] = (_byte)r;
		//		if (r == -1)
		//			numBytes = 0;
		//		if (numBytes == 2) {
		//			r = m_stream->ReadByte();
		//			m_charBytes[1] = (_byte)r;
		//			if (r == -1)
		//				numBytes = 1;
		//		}

		//		if (numBytes == 0) {
		//			// Console.WriteLine("Found no bytes.  We're outta here.");
		//			return -1;
		//		}
		//	}
		//	if (charsRead == 0)
		//		return -1;
		//	return m_singleChar[0];
		//}

		void BinaryReader::FillBuffer(int numBytes)
		{
			if (!m_buffer.empty() && (numBytes < 0 /*|| numBytes > m_buffer.size()*/)) {
				throw "ArgumentOutOfRange_BinaryReaderFillBuffer";
			}
			int bytesRead = 0;
			int n = 0;
			m_buffer = std::vector<_byte>(numBytes);

			if (m_stream == nullptr) return;

			// Need to find a good threshold for calling ReadByte() repeatedly
			// vs. calling Read(byte[], int, int) for both buffered & unbuffered
			// streams.
			if (numBytes == 1) {
				n = m_stream->ReadByte();
				if (n == -1)
					return;
				m_buffer[0] = (_byte)n;
				return;
			}

			do {
				n = m_stream->Read(m_buffer, bytesRead, numBytes - bytesRead);
				if (n == 0) {
					return;
				}
				bytesRead += n;
			} while (bytesRead < numBytes);
		}

		int BinaryReader::Read7BitEncodedInt()
		{
			// Read out an Int32 7 bits at a time.  The high bit
			// of the byte when on means to continue reading more bytes.
			int count = 0;
			int shift = 0;
			_byte b;
			do {
				// Check for a corrupted stream.  Read a max of 5 bytes.
				// In a future version, add a DataFormatException.
				if (shift == 5 * 7)  // 5 bytes max per Int32, shift += 7
					throw "Format_Bad7BitInt32";

				// ReadByte handles end of stream cases for us.
				b = ReadByte();
				count |= (b & 0x7F) << shift;
				shift += 7;
			} while ((b & 0x80) != 0);
			return count;
		}
	}
}