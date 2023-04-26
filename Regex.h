#pragma once
#include "pch.h"
#include <regex>
using namespace std;

namespace System
{
	namespace Text
	{
		namespace RegularExpression
		{
			class Regex
			{
			public:
				Regex();
				Regex(const std::wstring pattern);
				~Regex();

				//Methods
				static bool IsMatch(const std::wstring input, const std::wstring pattern);
				bool IsMatch(const std::wstring input);
				static wsmatch Match(const std::wstring input, const std::wstring pattern);
				std::wstring Replace(std::wstring input, std::wstring replacement);
				static std::wstring Replace(std::wstring input, const std::wstring pattern, std::wstring replacement);
				std::wstring ToString();
			private:
				std::wstring pattern;

			};
		}
	}
}
