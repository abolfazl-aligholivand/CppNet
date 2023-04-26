#pragma once
#include "pch.h"
#include <iostream>
#include <memory>
namespace System
{
	namespace Text
	{
		class Encoding
		{
		private:
			static std::shared_ptr<Encoding> utf8Encoding;
			static std::shared_ptr<Encoding> utf16Encoding;
			static std::shared_ptr<Encoding> asciiEncoding;
		public:
			Encoding();
			Encoding(int codePage);
			~Encoding();
			static Encoding* ASCII();
			static Encoding* UTF8();
			static Encoding* Unicode();
			static Encoding* GetEncoding(int codepage);
			static Encoding* Default();

			virtual int GetBytes(const std::wstring& s, int charIndex, int charCount, std::vector<_byte>& bytes, int byteIndex) = 0;
			virtual std::vector<_byte> GetBytes(const std::wstring& s) = 0;

			virtual std::wstring GetString(std::vector<_byte>& bytes) = 0;
			virtual std::wstring GetString(std::vector<_byte>& bytes, int index, int count) = 0;

		};
	}
}
