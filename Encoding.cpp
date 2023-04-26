#include "Encoding.h"
#include "UTF8Encoding.h"
#include "UTF16Encoding.h"
#include "ASCIIEncoding.h"
#include <codecvt>

namespace System
{
	namespace Text
	{
		std::shared_ptr<Encoding> Encoding::utf8Encoding = std::shared_ptr<Encoding>(nullptr);
		std::shared_ptr<Encoding> Encoding::utf16Encoding = std::shared_ptr<Encoding>(nullptr);
		std::shared_ptr<Encoding> Encoding::asciiEncoding = std::shared_ptr<Encoding>(nullptr);

		Encoding::Encoding()
		{

		}
		Encoding::Encoding(int codePage)
		{

		}
		Encoding::~Encoding()
		{

		}
		Encoding* Encoding::ASCII()
		{
			if (asciiEncoding == nullptr)
				asciiEncoding = std::shared_ptr<Encoding>(new ASCIIEncoding());
			return asciiEncoding.get();
		}

		Encoding* Encoding::UTF8()
		{
			if (utf8Encoding == nullptr)
				utf8Encoding = std::shared_ptr<Encoding>(new UTF8Encoding());
			return utf8Encoding.get();
		}

		Encoding* Encoding::Unicode()
		{
			if (utf16Encoding == nullptr)
				utf16Encoding = std::shared_ptr<Encoding>(new UTF16Encoding());
			return utf16Encoding.get();
		}

		Encoding* Encoding::GetEncoding(int codepage)
		{
			if (codepage < 0 || codepage > 65535) {
				throw "codepage ArgumentOutOfRange_Range";
			}

			Encoding* result = nullptr;

			if (codepage == 1200)
				result = Unicode();
			else if (codepage == 65001)
				result = UTF8();
			else if (codepage == 20127)
				result = ASCII();
			else
				throw "codepage is not valid";

			return result;
		}

		Encoding* Encoding::Default()
		{
			return UTF8();
		}
	}
}