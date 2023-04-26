#include "Convert.h"
#include <string>
namespace System
{
	std::vector<char> Convert::base64Table = std::vector<char>{
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',
		'P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d',
		'e','f','g','h','i','j','k','l','m','n','o','p','q','r','s',
		't','u','v','w','x','y','z','0','1','2','3','4','5','6','7',
		'8','9','+','/','='
	};

	Convert::Convert()
	{
	}

	Convert::~Convert()
	{
	}

	char Convert::ToChar(int8 value)
	{
		if (value < 0)
			throw "Overflow_Char";
		return (char)value;
	}

	char Convert::ToChar(_byte value)
	{
		return (char)value;
	}

	char Convert::ToChar(short value)
	{
		if (value < 0)
			throw "Overflow_Char";
		return (char)value;
	}

	char Convert::ToChar(int value)
	{
		if (value < 0 && value > CHAR_MAX)
			throw "Overflow_Char";
		return (char)value;
	}

	char Convert::ToChar(unsigned int value)
	{
		if (value < 0 && value > CHAR_MAX)
			throw "Overflow_Char";
		return (char)value;
	}

	char Convert::ToChar(_int64 value)
	{
		if (value < 0 && value > CHAR_MAX)
			throw "Overflow_Char";
		return (char)value;
	}

	char Convert::ToChar(uint64 value)
	{
		if (value < 0 && value > CHAR_MAX)
			throw "Overflow_Char";
		return (char)value;
	}

	std::wstring Convert::ToBase64String(std::vector<_byte> inArray)
	{
		if (inArray.empty() || inArray.size() == 0)
			throw "inArray";
		std::wstring te = ToBase64String(inArray, 0, inArray.size(), Base64FormattingOptions::None);
		return te;
	}

	std::wstring Convert::ToBase64String(std::vector<_byte> inArray, Base64FormattingOptions options)
	{
		if (inArray.empty() || inArray.size() == 0)
			throw "inArray";
		return ToBase64String(inArray, 0, inArray.size(), options);
	}

	std::wstring Convert::ToBase64String(std::vector<_byte> inArray, int offset, int length)
	{
		if (inArray.empty() || inArray.size() == 0)
			throw "inArray";
		return ToBase64String(inArray, offset, length, Base64FormattingOptions::None);
	}

	std::wstring Convert::ToBase64String(std::vector<_byte> inArray, int offset, int length, Base64FormattingOptions options)
	{
		if (inArray.empty() || inArray.size() == 0)
			throw "inArray";
		if (length < 0)
			throw "length_ArgumentOutOfRange_Index";
		if (offset < 0)
			throw "offset_ArgumentOutOfRange_GenericPositive";
		if (options < Base64FormattingOptions::None || options > Base64FormattingOptions::InsertLineBreaks)
			throw "Arg_EnumIllegalVal";

		int inArrayLength;
		int stringLength;

		inArrayLength = inArray.size();
		if (offset > (inArrayLength - length))
			throw "offset_ArgumentOutOfRange_OffsetLength";

		if (inArrayLength == 0)
			return L"";

		bool insertLineBreaks = (options == Base64FormattingOptions::InsertLineBreaks);
		//Create the new string.  This is the maximally required length.
		stringLength = ToBase64_CalculateAndValidateOutputLength(length, insertLineBreaks);

		std::wstring returnString = L"";
		char* outChars = (char*)returnString.c_str();

		_byte* inData = &inArray.data()[0];
		int j = ConvertToBase64Array(outChars, inData, offset, length, insertLineBreaks);

		wchar_t * wcstring = new wchar_t[j + 1];
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, j + 1, outChars, _TRUNCATE);
		std::wstring base64String = wcstring;
		return base64String;
	}

	int Convert::ToBase64_CalculateAndValidateOutputLength(int inputLength, bool insertLineBreaks)
	{
		long outlen = ((long)inputLength) / 3 * 4;          // the base length - we want integer division here. 
		outlen += ((inputLength % 3) != 0) ? 4 : 0;         // at most 4 more chars for the remainder

		if (outlen == 0)
			return 0;

		if (insertLineBreaks) {
			long newLines = outlen / base64LineBreakPosition;
			if ((outlen % base64LineBreakPosition) == 0) {
				--newLines;
			}
			outlen += newLines * 2;              // the number of line break chars we'll add, "\r\n"
		}

		// If we overflow an int then we cannot allocate enough
		// memory to output the value so throw
		if (outlen > INT_MAX)
			throw "OutOfMemoryException";

		return (int)outlen;
	}

	int Convert::ConvertToBase64Array(char* outChars, _byte* inData, int offset, int length, bool insertLineBreaks)
	{
		int lengthmod3 = length % 3;
		int calcLength = offset + (length - lengthmod3);
		int j = 0;
		int charcount = 0;
		//Convert three bytes at a time to base64 notation.  This will consume 4 chars.
		int i;

		// get a pointer to the base64Table to avoid unnecessary range checking
		char* base64 = &base64Table.data()[0];
		for (i = offset; i < calcLength; i += 3)
		{
			if (insertLineBreaks) {
				if (charcount == base64LineBreakPosition) {
					outChars[j++] = '\r';
					outChars[j++] = '\n';
					charcount = 0;
				}
				charcount += 4;
			}
			outChars[j] = base64[(inData[i] & 0xfc) >> 2];
			outChars[j + 1] = base64[((inData[i] & 0x03) << 4) | ((inData[i + 1] & 0xf0) >> 4)];
			outChars[j + 2] = base64[((inData[i + 1] & 0x0f) << 2) | ((inData[i + 2] & 0xc0) >> 6)];
			outChars[j + 3] = base64[(inData[i + 2] & 0x3f)];
			j += 4;
		}

		//Where we left off before
		i = calcLength;

		if (insertLineBreaks && (lengthmod3 != 0) && (charcount == base64LineBreakPosition)) {
			outChars[j++] = '\r';
			outChars[j++] = '\n';
		}

		switch (lengthmod3)
		{
		case 2: //One character padding needed
			outChars[j] = base64[(inData[i] & 0xfc) >> 2];
			outChars[j + 1] = base64[((inData[i] & 0x03) << 4) | ((inData[i + 1] & 0xf0) >> 4)];
			outChars[j + 2] = base64[(inData[i + 1] & 0x0f) << 2];
			outChars[j + 3] = base64[64]; //Pad
			j += 4;
			break;
		case 1: // Two character padding needed
			outChars[j] = base64[(inData[i] & 0xfc) >> 2];
			outChars[j + 1] = base64[(inData[i] & 0x03) << 4];
			outChars[j + 2] = base64[64]; //Pad
			outChars[j + 3] = base64[64]; //Pad
			j += 4;
			break;
		}

		return j;
	}

	std::vector<_byte> Convert::FromBase64String(std::wstring s)
	{
		if (s == L"")
			throw "s";

		std::string str(s.begin(), s.end());
		char* sPtr = (char*)str.c_str();
		return FromBase64CharPtr(sPtr, s.size());
	}

	int Convert::ToBase64CharArray(std::vector<_byte> inArray, int offsetIn, int length, std::vector<char> outArray, int offsetOut)
	{
		return ToBase64CharArray(inArray, offsetIn, length, outArray, offsetOut, Base64FormattingOptions::None);
	}

	int Convert::ToBase64CharArray(std::vector<_byte> inArray, int offsetIn, int length, std::vector<char>& outArray, int offsetOut, Base64FormattingOptions options)
	{
		if (inArray.empty() || inArray.size() == 0)
			throw "inArray";
		if (outArray.empty() || outArray.size() == 0)
			throw "outArray";
		if (length < 0)
			throw "length_ArgumentOutOfRange_Index";
		if (offsetIn < 0)
			throw "offsetIn_ArgumentOutOfRange_GenericPositive";
		if (offsetOut < 0)
			throw "offsetOut_ArgumentOutOfRange_GenericPositive";

		if (options < Base64FormattingOptions::None || options > Base64FormattingOptions::InsertLineBreaks)
			throw "Arg_EnumIllegalVal";

		int retVal;

		int inArrayLength;
		int outArrayLength;
		int numElementsToCopy;

		inArrayLength = inArray.size();

		if (offsetIn > (int)(inArrayLength - length))
			throw "offsetIn_ArgumentOutOfRange_OffsetLength";

		if (inArrayLength == 0)
			return 0;

		bool insertLineBreaks = (options == Base64FormattingOptions::InsertLineBreaks);
		//This is the maximally required length that must be available in the char array
		outArrayLength = outArray.size();

		// Length of the char buffer required
		numElementsToCopy = ToBase64_CalculateAndValidateOutputLength(length, insertLineBreaks);

		if (offsetOut > (int)(outArrayLength - numElementsToCopy))
			throw "offsetOut_ArgumentOutOfRange_OffsetOut";

		char* outChars = &outArray.data()[offsetOut];
		_byte* inData = &inArray.data()[0];
		retVal = ConvertToBase64Array(outChars, inData, offsetIn, length, insertLineBreaks);

		return retVal;
	}

	std::vector<_byte> Convert::FromBase64CharArray(std::vector<char> inArray, int offset, int length)
	{
		if (inArray.empty() || inArray.size() == 0)
			throw "inArray";

		if (length < 0)
			throw "length_ArgumentOutOfRange_Index";

		if (offset < 0)
			throw "offset_ArgumentOutOfRange_GenericPositive";

		if (offset > inArray.size() - length)
			throw "offset_ArgumentOutOfRange_OffsetLength";

		char* inArrayPtr = &inArray.data()[0];

		return FromBase64CharPtr(inArrayPtr + offset, length);
	}

	std::vector<_byte> Convert::FromBase64CharPtr(char* inputPtr, int inputLength)
	{
		// The validity of parameters much be checked by callers, thus we are Critical here.

		assert(0 <= inputLength);

		// We need to get rid of any trailing white spaces.
		// Otherwise we would be rejecting input such as "abc= ":
		while (inputLength > 0)
		{
			int lastChar = inputPtr[inputLength - 1];
			if (lastChar != (int)' ' && lastChar != (int)'\n' && lastChar != (int)'\r' && lastChar != (int)'\t')
				break;
			inputLength--;
		}

		// Compute the output length:
		int resultLength = FromBase64_ComputeResultLength(inputPtr, inputLength);

		assert(0 <= resultLength);

		// resultLength can be zero. We will still enter FromBase64_Decode and process the input.
		// It may either simply write no bytes (e.g. input = " ") or throw (e.g. input = "ab").

		// Create result byte blob:
		std::vector<_byte> decodedBytes = std::vector<_byte>(resultLength);

		// Convert Base64 chars into bytes:
		int actualResultLength;
		_byte* decodedBytesPtr = &decodedBytes.data()[0];
		actualResultLength = FromBase64_Decode(inputPtr, inputLength, decodedBytesPtr, resultLength);

		// Note that actualResultLength can differ from resultLength if the caller is modifying the array
		// as it is being converted. Silently ignore the failure.
		// Consider throwing exception in an non in-place release.

		// We are done:
		return decodedBytes;
	}

	int Convert::FromBase64_ComputeResultLength(char* inputPtr, int inputLength)
	{
		const unsigned int intEq = (unsigned int)'=';
		const unsigned int intSpace = (unsigned int)' ';

		assert(0 <= inputLength);

		char* inputEndPtr = inputPtr + inputLength;
		int usefulInputLength = inputLength;
		int padding = 0;

		while (inputPtr < inputEndPtr) {

			unsigned int c = (unsigned int)(*inputPtr);
			inputPtr++;

			// We want to be as fast as possible and filter out spaces with as few comparisons as possible.
			// We end up accepting a number of illegal chars as legal white-space chars.
			// This is ok: as soon as we hit them during actual decode we will recognise them as illegal and throw.
			if (c <= intSpace)
				usefulInputLength--;

			else if (c == intEq) {
				usefulInputLength--;
				padding++;
			}
		}

		assert(0 <= usefulInputLength);

		// For legal input, we can assume that 0 <= padding < 3. But it may be more for illegal input.
		// We will notice it at decode when we see a '=' at the wrong place.
		assert(0 <= padding);

		// Perf: reuse the variable that stored the number of '=' to store the number of bytes encoded by the
		// last group that contains the '=':
		if (padding != 0) {

			if (padding == 1)
				padding = 2;
			else if (padding == 2)
				padding = 1;
			else
				throw "Format_BadBase64Char";
		}

		// Done:
		return (usefulInputLength / 4) * 3 + padding;
	}

	int Convert::FromBase64_Decode(char* startInputPtr, int inputLength, _byte* startDestPtr, int destLength)
	{
		// You may find this method weird to look at. It’s written for performance, not aesthetics.
			// You will find unrolled loops label jumps and bit manipulations.

		const unsigned int intA = (unsigned int)'A';
		const unsigned int inta = (unsigned int)'a';
		const unsigned int int0 = (unsigned int)'0';
		const unsigned int intEq = (unsigned int)'=';
		const unsigned int intPlus = (unsigned int)'+';
		const unsigned int intSlash = (unsigned int)'/';
		const unsigned int intSpace = (unsigned int)' ';
		const unsigned int intTab = (unsigned int)'\t';
		const unsigned int intNLn = (unsigned int)'\n';
		const unsigned int intCRt = (unsigned int)'\r';
		const unsigned int intAtoZ = (unsigned int)('Z' - 'A');  // = ('z' - 'a')
		const unsigned int int0to9 = (unsigned int)('9' - '0');

		char* inputPtr = startInputPtr;
		_byte* destPtr = startDestPtr;

		// Pointers to the end of input and output:
		char* endInputPtr = inputPtr + inputLength;
		_byte* endDestPtr = destPtr + destLength;

		// Current char code/value:
		unsigned int currCode;

		// This 4-byte integer will contain the 4 codes of the current 4-char group.
		// Eeach char codes for 6 bits = 24 bits.
		// The remaining byte will be FF, we use it as a marker when 4 chars have been processed.            
		unsigned int currBlockCodes = 0x000000FFu;

		while (true) {

			// break when done:
			if (inputPtr >= endInputPtr)
				goto _AllInputConsumed;

			// Get current char:
			currCode = (unsigned int)(*inputPtr);
			inputPtr++;

			// Determine current char code:

			if (currCode - intA <= intAtoZ)
				currCode -= intA;

			else if (currCode - inta <= intAtoZ)
				currCode -= (inta - 26u);

			else if (currCode - int0 <= int0to9)
				currCode -= (int0 - 52u);

			else {
				// Use the slower switch for less common cases:
				switch (currCode) {

					// Significant chars:
				case intPlus:  currCode = 62u;
					break;

				case intSlash: currCode = 63u;
					break;

					// Legal no-value chars (we ignore these):
				case intCRt:  case intNLn:  case intSpace:  case intTab:
					continue;

					// The equality char is only legal at the end of the input.
					// Jump after the loop to make it easier for the JIT register predictor to do a good job for the loop itself:
				case intEq:
					goto _EqualityCharEncountered;

					// Other chars are illegal:
				default:
					throw "Format_BadBase64Char";
				}
			}

			// Ok, we got the code. Save it:
			currBlockCodes = (currBlockCodes << 6) | currCode;

			// Last bit in currBlockCodes will be on after in shifted right 4 times:
			if ((currBlockCodes & 0x80000000u) != 0u) {

				if ((int)(endDestPtr - destPtr) < 3)
					return -1;

				*(destPtr) = (_byte)(currBlockCodes >> 16);
				*(destPtr + 1) = (_byte)(currBlockCodes >> 8);
				*(destPtr + 2) = (_byte)(currBlockCodes);
				destPtr += 3;

				currBlockCodes = 0x000000FFu;
			}

		}

		// 'd be nice to have an assert that we never get here, but CS0162: Unreachable code detected.
		// Contract.Assert(false, "We only leave the above loop by jumping; should never get here.");

		// We jump here out of the loop if we hit an '=':
	_EqualityCharEncountered:

		assert(currCode == intEq);

		// Recall that inputPtr is now one position past where '=' was read.
		// '=' can only be at the last input pos:
		if (inputPtr == endInputPtr) {

			// Code is zero for trailing '=':
			currBlockCodes <<= 6;

			// The '=' did not complete a 4-group. The input must be bad:
			if ((currBlockCodes & 0x80000000u) == 0u)
				throw "Format_BadBase64CharArrayLength";

			if ((int)(endDestPtr - destPtr) < 2)  // Autch! We underestimated the output length!
				return -1;

			// We are good, store bytes form this past group. We had a single "=", so we take two bytes:
			*(destPtr++) = (_byte)(currBlockCodes >> 16);
			*(destPtr++) = (_byte)(currBlockCodes >> 8);

			currBlockCodes = 0x000000FFu;

		}
		else { // '=' can also be at the pre-last position iff the last is also a '=' excluding the white spaces:

		 // We need to get rid of any intermediate white spaces.
		 // Otherwise we would be rejecting input such as "abc= =":
			while (inputPtr < (endInputPtr - 1))
			{
				int lastChar = *(inputPtr);
				if (lastChar != (int)' ' && lastChar != (int)'\n' && lastChar != (int)'\r' && lastChar != (int)'\t')
					break;
				inputPtr++;
			}

			if (inputPtr == (endInputPtr - 1) && *(inputPtr) == '=') {

				// Code is zero for each of the two '=':
				currBlockCodes <<= 12;

				// The '=' did not complete a 4-group. The input must be bad:
				if ((currBlockCodes & 0x80000000u) == 0u)
					throw "Format_BadBase64CharArrayLength";

				if ((int)(endDestPtr - destPtr) < 1)  // Autch! We underestimated the output length!
					return -1;

				// We are good, store bytes form this past group. We had a "==", so we take only one byte:
				*(destPtr++) = (_byte)(currBlockCodes >> 16);

				currBlockCodes = 0x000000FFu;

			}
			else  // '=' is not ok at places other than the end:
				throw "Format_BadBase64Char";

		}

		// We get here either from above or by jumping out of the loop:
	_AllInputConsumed:

		// The last block of chars has less than 4 items
		if (currBlockCodes != 0x000000FFu)
			throw "Format_BadBase64CharArrayLength";

		// Return how many bytes were actually recovered:
		return (int)(destPtr - startDestPtr);
	}
}
