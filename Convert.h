#pragma once
#include "pch.h"
#include <assert.h>

namespace System
{
	enum Base64FormattingOptions {
		None = 0,
		InsertLineBreaks = 1
	};

	class Convert
	{
	public:
		Convert();
		~Convert();

		//Methods
		static char ToChar(int8 value);
		static char ToChar(_byte value);
		static char ToChar(short value);
		static char ToChar(int value);
		static char ToChar(unsigned int value);
		static char ToChar(_int64 value);
		static char ToChar(uint64 value);
		static std::wstring ToBase64String(std::vector<_byte> inArray);
		static std::wstring ToBase64String(std::vector<_byte> inArray, Base64FormattingOptions options);
		static std::wstring ToBase64String(std::vector<_byte> inArray, int offset, int length);
		static std::wstring ToBase64String(std::vector<_byte> inArray, int offset, int length, Base64FormattingOptions options);
		static int ToBase64CharArray(std::vector<_byte> inArray, int offsetIn, int length, std::vector<char> outArray, int offsetOut);
		static int ToBase64CharArray(std::vector<_byte> inArray, int offsetIn, int length, std::vector<char>& outArray, int offsetOut, Base64FormattingOptions optiosn);
		static std::vector<_byte> FromBase64String(std::wstring s);
		static std::vector<_byte> FromBase64CharArray(std::vector<char> inArray, int offset, int length);


	private:
		static int ConvertToBase64Array(char* outChars, _byte* inData, int offset, int length, bool insertLineBreaks);
		static int ToBase64_CalculateAndValidateOutputLength(int inputLength, bool insertLineBreaks);
		static std::vector<_byte> FromBase64CharPtr(char* inputPtr, int inputLength);
		static int FromBase64_ComputeResultLength(char* inputPtr, int inputLength);
		static int FromBase64_Decode(char* startInputPtr, int inputLength, _byte* startDestPtr, int destLength);

	private:
		static std::vector<char> base64Table;
		static const int base64LineBreakPosition = 76;
	};
}
