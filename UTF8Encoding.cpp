#include "UTF8Encoding.h"
#include <codecvt>
#include <clocale>
#include <cstdlib>
#include <iostream>
#include <string>

namespace System
{
	namespace Text
	{
		UTF8Encoding::UTF8Encoding() : Encoding()
		{
		}
		UTF8Encoding::~UTF8Encoding()
		{

		}


		int UTF8Encoding::GetBytes(const std::wstring& s, int charIndex, int charCount, std::vector<_byte>& bytes, int byteIndex)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t, 0x10ffff, std::little_endian>> cvt;
			std::string str = cvt.to_bytes(s);
			auto len = charIndex + charCount;
			for (int i = charIndex; i < len; i++)
				bytes.push_back(str[i]);
			return charCount;
		}

		std::vector<_byte> UTF8Encoding::GetBytes(const std::wstring& s)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t, 0x10ffff, std::little_endian>> cvt;
			std::string str = cvt.to_bytes(s);
			std::vector<_byte> bytes;
			for (int i = 0; i < str.size(); i++)
				bytes.push_back(str[i]);
			return bytes;
		}

		std::wstring UTF8Encoding::GetString(std::vector<_byte>& bytes)
		{
			return GetString(bytes, 0, bytes.size());
		}

		std::wstring UTF8Encoding::GetString(std::vector<_byte>& bytes, int index, int count)
		{
			if (bytes.empty() || bytes.size() == 0)
				throw "source is empty";
			if (index + count > bytes.size())
				throw "Out of range";

			std::vector<_byte> newBytes;
			for (int i = index; i < index + count; i++)
				newBytes.push_back(bytes[i]);
			//bytes = newBytes;
			//newBytes.clear();

			//int i = index;
			//while (i < bytes.size())
			//{
			//	if (bytes[i] < 128)
			//	{
			//		newBytes.push_back(bytes[i]);
			//		i++;
			//	}
			//	else
			//	{
			//		if (i + 2 < bytes.size())
			//		{
			//			if (bytes[i + 1] > 127 && bytes[i + 2] > 127)
			//			{
			//				newBytes.push_back(239);
			//				newBytes.push_back(191);
			//				newBytes.push_back(189);
			//				i += 3;
			//			}
			//			else if (bytes[i + 1] > 127)
			//			{
			//				newBytes.push_back(239);
			//				newBytes.push_back(191);
			//				newBytes.push_back(189);
			//				i += 2;
			//			}
			//			else
			//			{
			//				newBytes.push_back(239);
			//				newBytes.push_back(191);
			//				newBytes.push_back(189);
			//				i++;
			//			}
			//		}
			//		else
			//		{
			//			if (i + 1 > bytes.size() && bytes[i + 1] > 127)
			//			{
			//				newBytes.push_back(239);
			//				newBytes.push_back(191);
			//				newBytes.push_back(189);
			//				i += 2;
			//			}
			//			else
			//			{
			//				newBytes.push_back(239);
			//				newBytes.push_back(191);
			//				newBytes.push_back(189);
			//				i++;
			//			}
			//		}
			//	}
			//}

			unsigned char* p = NULL;
			p = &newBytes[0];
			char* chars = (char*)(p + index);
			static std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t > converter;
			auto str = converter.from_bytes(chars, chars + newBytes.size());
			return str;
		}

		void UTF8Encoding::Convert(std::vector<_byte> bytes, int byteIndex, int byteCount, std::vector<char>& chars, int& byteUsed, int& charsUsed)
		{
			if (bytes.empty() || bytes.size() == 0)
				throw "source is empty";
			if (byteIndex + byteCount > bytes.size())
				throw "Out og range";

			int firstByte = byteIndex, lastByte = byteIndex + byteCount;
			charsUsed = 0, byteUsed = 0;
			std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv_utf8_utf32;
			for (int i = firstByte; i < lastByte; i++)
			{
				std::u32string unicode_codepoints = conv_utf8_utf32.from_bytes(bytes[i]);
				chars.push_back(*unicode_codepoints.c_str());
				byteUsed++;
				charsUsed++;
			}
		}
	}
}
