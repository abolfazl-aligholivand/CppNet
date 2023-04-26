#pragma once
#include "Encoding.h"

namespace System
{
	namespace Text
	{
		class ASCIIEncoding : public Encoding
		{
		public:
			ASCIIEncoding();
			~ASCIIEncoding();

			int GetBytes(const std::wstring& s, int charIndex, int charCount, std::vector<_byte>& bytes, int byteIndex) override;
			std::vector<_byte> GetBytes(const std::wstring& s) override;

			std::wstring GetString(std::vector<_byte>& bytes) override;
			std::wstring GetString(std::vector<_byte>& bytes, int index, int count) override;

			//int GetCharCount(std::vector<byte>& s, int index, int count) override;
			//int GetCharCount(std::vector<byte>& chars) override;

			//int GetChars(std::vector<byte>& bytes, int byteIndex, int byteCount, std::vector<byte>& chras, int charCount) override;
			//std::vector<byte> GetChars(std::vector<byte>& bytes) override;
			//std::vector<byte> GetChars(std::vector<byte>& bytes, int index, int count) override;

		};

	}
}
