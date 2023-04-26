#include "ASCIIEncoding.h"
#include <codecvt>
#include <clocale>
#include <cstdlib>
#include <iostream>
#include <string>

namespace System
{
	namespace Text
	{
		ASCIIEncoding::ASCIIEncoding() : Encoding()
		{
		}
		ASCIIEncoding::~ASCIIEncoding()
		{

		}

		int ASCIIEncoding::GetBytes(const std::wstring& s, int charIndex, int charCount, std::vector<_byte>& bytes, int byteIndex)
		{
			if (s.empty())
				throw "String is null";
			if (charIndex > s.size())
				throw "Out of range";
			if (charIndex + charCount > s.size())
				throw "Out of range";
			std::string str(s.begin(), s.end());
			int count = 0;
			for (int i = charIndex; i < charIndex + charCount; i++)
			{
				bytes.push_back((int)str[i]);
				count++;
			}
			return count;
		}

		std::vector<_byte> ASCIIEncoding::GetBytes(const std::wstring& s)
		{
			std::vector<_byte> bytes;
			GetBytes(s, 0, s.size(), bytes, 0);
			return bytes;
		}


		std::wstring ASCIIEncoding::GetString(std::vector<_byte>& bytes)
		{
			return GetString(bytes, 0, bytes.size());
		}
		std::wstring ASCIIEncoding::GetString(std::vector<_byte>& bytes, int index, int count)
		{
			if (bytes.empty())
				throw "bytes is null";
			if (index > bytes.size())
				throw "Out of range";
			if (index + count > bytes.size())
				throw "Out of range";
			std::string result = "";
			for (int i = index; i < index + count; i++)
				result += (char)bytes[i];
			return std::wstring(result.begin(), result.end());
		}

	}
}
