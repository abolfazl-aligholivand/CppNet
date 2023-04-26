#pragma once
#include "Encoding.h"

namespace System
{
	namespace Text
	{
		class UTF16Encoding : public Encoding
		{
		public:
			UTF16Encoding();
			~UTF16Encoding();

			int GetBytes(const std::wstring& s, int charIndex, int charCount, std::vector<_byte>& bytes, int byteIndex) override;
			std::vector<_byte> GetBytes(const std::wstring& s) override;

			std::wstring GetString(std::vector<_byte>& bytes) override;
			std::wstring GetString(std::vector<_byte>& bytes, int index, int count) override;
		};

	}
}
