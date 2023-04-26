#include "UTF16Encoding.h"
#include <codecvt>
#include <clocale>
#include <cstdlib>
#include <iostream>
#include <string>

namespace System
{
	namespace Text
	{
		UTF16Encoding::UTF16Encoding() : Encoding()
		{
		}
		UTF16Encoding::~UTF16Encoding()
		{

		}

		int UTF16Encoding::GetBytes(const std::wstring& s, int charIndex, int charCount, std::vector<_byte>& bytes, int byteIndex)
		{
			std::wstring_convert<std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>> cvt1;
			std::string str = cvt1.to_bytes(s);

			for (int i = 0; i < str.size(); i++)
				bytes.push_back((int)str[i]);
			return charCount;
		}

		std::vector<_byte> UTF16Encoding::GetBytes(const std::wstring& s)
		{
			std::vector<_byte> bytes;
			GetBytes(s, 0, s.size(), bytes, 0);
			return bytes;
		}

		std::wstring UTF16Encoding::GetString(std::vector<_byte>& bytes)
		{
			return GetString(bytes, 0, bytes.size());
		}
		std::wstring UTF16Encoding::GetString(std::vector<_byte>& bytes, int index, int count)
		{
			unsigned char* p = &bytes[0];
			char* chars = (char*)(p + index);
			using convert_type = std::codecvt_utf16<typename std::wstring::value_type>;
			std::wstring_convert<convert_type, typename std::wstring::value_type> converter;
			auto u16String = converter.from_bytes(chars, chars + count);

			char* buffer = (char*)u16String.c_str();
			int buflen = u16String.size() * 2;
			std::wstring_convert<std::codecvt_utf16<wchar_t>, wchar_t> conv;
			std::string str(buffer, buffer+buflen);
			std::wstring originalString = conv.from_bytes(str);// std::string(buffer, buflen));
			return originalString;
		}
	}
}
