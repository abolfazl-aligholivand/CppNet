#include "Regex.h"
#include <regex>

namespace System
{
	namespace Text
	{
		namespace RegularExpression
		{
			Regex::Regex()
			{
			}

			Regex::Regex(const std::wstring pattern)
			{
				this->pattern = pattern;
			}

			Regex::~Regex()
			{
			}

			bool Regex::IsMatch(const std::wstring input, const std::wstring pattern)
			{
				std::wregex reg(pattern);
				return std::regex_match(input, reg);
			}

			bool Regex::IsMatch(const std::wstring input)
			{
				std::wregex reg(pattern);
				return std::regex_match(input, reg);
			}

			wsmatch Regex::Match(const std::wstring input, const std::wstring pattern)
			{
				if (input.empty())
					throw "input";

				std::wregex reg(pattern);
				std::wsmatch match;
				std::regex_search(input, match, reg);
				return match;

			}

			std::wstring Regex::Replace(std::wstring input, std::wstring replacement)
			{
				std::wregex reg(pattern);
				return std::regex_replace(input, reg, replacement);
			}

			std::wstring Regex::Replace(std::wstring input, const std::wstring pattern, std::wstring replacement)
			{
				std::wregex reg(pattern);
				return std::regex_replace(input, reg, replacement);
			}

			std::wstring Regex::ToString()
			{
				return pattern;
			}
		}
	}
}