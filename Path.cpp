#include "Path.h"

namespace System
{
	namespace IO
	{
		Path::Path()
		{
		}

		Path::~Path()
		{
		}

		char Path::DirectorySeparatorChar = '\\';
		char Path::AltDirectorySeparatorChar = '/';
		char Path::VolumeSeparatorChar = ':';
		std::vector<char> Path::InvalidPathChars = { '\"', '<', '>', '|', '\0', (char)1, (char)2, (char)3, (char)4, (char)5, (char)6, (char)7, (char)8, (char)9, (char)10, (char)11, (char)12, (char)13, (char)14, (char)15, (char)16, (char)17, (char)18, (char)19, (char)20, (char)21, (char)22, (char)23, (char)24, (char)25, (char)26, (char)27, (char)28, (char)29, (char)30, (char)31 };
		std::vector<char> Path::InvalidPathcharsWithAdditionalChecks = { '\"', '<', '>', '|', '\0', (char)1, (char)2, (char)3, (char)4, (char)5, (char)6, (char)7, (char)8, (char)9, (char)10, (char)11, (char)12, (char)13, (char)14, (char)15, (char)16, (char)17, (char)18, (char)19, (char)20, (char)21, (char)22, (char)23, (char)24, (char)25, (char)26, (char)27, (char)28, (char)29, (char)30, (char)31, '*', '?' };
		std::vector<char> Path::InvalidFileNameChars = { '\"', '<', '>', '|', '\0', (char)1, (char)2, (char)3, (char)4, (char)5, (char)6, (char)7, (char)8, (char)9, (char)10, (char)11, (char)12, (char)13, (char)14, (char)15, (char)16, (char)17, (char)18, (char)19, (char)20, (char)21, (char)22, (char)23, (char)24, (char)25, (char)26, (char)27, (char)28, (char)29, (char)30, (char)31, ':', '*', '?', '\\', '/' };
		char Path::PathSeparator = ';';
		std::wstring Path::DirectorySeparatorCharAsString = L"\\";

		std::wstring Path::ChangeExtension(std::wstring path, std::wstring extension)
		{
			std::wstring s = path;
			if (path != L"")
			{
				for (int i = path.size(); --i >= 0;) {
					char ch = path[i];
					if (ch == '.') {
						s = path.substr(0, i);
						break;
					}
					if (ch == DirectorySeparatorChar || ch == AltDirectorySeparatorChar || ch == VolumeSeparatorChar) break;
				}
				if (extension != L"" && path.size() != 0) {
					if (extension.size() == 0 || extension[0] != '.') {
						s = s + L".";
					}
					s = s + extension;
				}
				return s;
			}
			return L"";
		}

		std::wstring Path::Combine(std::wstring path1, std::wstring path2)
		{
			if (path1 == L"" || path2 == L"")
				throw "One of the pathes is null";

			return CombineNoChecks(path1, path2);
		}

		std::wstring Path::GetDirectoryName(std::wstring path)
		{
			if (GetPathRoot(path) == path)
				return L"";

			std::vector<wchar_t> PathSeparatorChars = std::vector<wchar_t>{
				(wchar_t)DirectorySeparatorChar, (wchar_t)AltDirectorySeparatorChar, (wchar_t)VolumeSeparatorChar
			};

			int nLast = 0;

			for (int i = 0; i < PathSeparatorChars.size(); i++)
			{
				nLast = path.find_last_of(PathSeparatorChars[i]);
				if (nLast != -1)
					break;
			}

			if (nLast == 0)
				nLast++;

			if (nLast > 0)
			{
				std::wstring ret = path.substr(0, nLast);
				int l = ret.size();

				if (l >= 2 && DirectorySeparatorChar == '\\' && ret[l - 1] == VolumeSeparatorChar)
					return ret + (wchar_t)DirectorySeparatorChar;
				else if (l == 1 && DirectorySeparatorChar == '\\' && path.size() >= 2 && path[nLast] == VolumeSeparatorChar)
					return ret + (wchar_t)VolumeSeparatorChar;
				else
				{
					//
					// Important: do not use CanonicalizePath here, use
					// the custom CleanPath here, as this should not
					// return absolute paths
					//
					return CleanPath(ret);
				}
			}
			return L"";
		}

		std::wstring Path::GetFileName(std::wstring path)
		{
			if (path != L"")
			{
				int length = path.size();
				for (int i = length; --i >= 0;) {
					char ch = path[i];
					if (ch == DirectorySeparatorChar || ch == AltDirectorySeparatorChar || ch == VolumeSeparatorChar)
						return path.substr(i + 1, length - i - 1);
				}
			}
			return path;
		}

		std::wstring Path::GetFileNameWithoutExtension(std::wstring path)
		{
			path = GetFileName(path);
			if (path != L"")
			{
				int i;
				if ((i = path.find_last_of('.')) == -1)
					return path; // No path extension found
				else
					return path.substr(0, i);
			}
			return L"";
		}

		std::wstring Path::GetFullPath(std::wstring path)
		{
			if (path == L"")
				throw "path";

			std::wstring newPath = NormalizePath(path, true);

			return newPath;
		}

		std::wstring Path::GetPathRoot(std::wstring path)
		{
			if (path == L"") return L"";

			// Expanding short paths has no impact on the path root- there is no such thing as an
			// 8.3 volume or server/share name.
			path = NormalizePath(path, false);
			return path.substr(0, GetRootLength(path));
		}

		bool Path::IsPathRooted(std::wstring path)
		{
			if (path != L"")
			{
				int length = path.size();
				if ((length >= 1 && (path[0] == DirectorySeparatorChar || path[0] == AltDirectorySeparatorChar)) || (length >= 2 && path[1] == VolumeSeparatorChar))
					return true;
			}
			return false;
		}

		std::wstring Path::GetExtension(std::wstring path)
		{
			if (path == L"")
				return L"";

			int length = path.size();
			for (int i = length; --i >= 0;) {
				char ch = path[i];
				if (ch == '.')
				{
					if (i != length - 1)
						return path.substr(i, length - i);
					else
						return L"";
				}
				if (ch == DirectorySeparatorChar || ch == AltDirectorySeparatorChar || ch == VolumeSeparatorChar)
					break;
			}
			return L"";
		}

		std::wstring Path::CombineNoChecks(std::wstring path1, std::wstring path2)
		{
			if (path2.size() == 0)
				return path1;

			if (path1.size() == 0)
				return path2;

			if (IsPathRooted(path2))
				return path2;

			char ch = path1[path1.size() - 1];
			if (ch != DirectorySeparatorChar && ch != AltDirectorySeparatorChar && ch != VolumeSeparatorChar)
				return path1 + DirectorySeparatorCharAsString + path2;
			return path1 + path2;
		}

		std::wstring Path::CleanPath(std::wstring path)
		{
			int l = path.size();
			int sub = 0;
			int start = 0;

			// Host prefix?
			wchar_t s0 = path[0];
			if (l > 2 && s0 == L'\\' && path[1] == L'\\')
			{
				start = 2;
			}

			// We are only left with root
			if (l == 1 && (s0 == DirectorySeparatorChar || s0 == AltDirectorySeparatorChar))
				return path;

			// Cleanup
			for (int i = start; i < l; i++)
			{
				wchar_t c = path[i];

				if (c != DirectorySeparatorChar && c != AltDirectorySeparatorChar)
					continue;
				if (i + 1 == l)
					sub++;
				else
				{
					c = path[i + 1];
					if (c == DirectorySeparatorChar || c == AltDirectorySeparatorChar)
						sub++;
				}
			}

			if (sub == 0)
				return path;

			std::vector<wchar_t> copy = std::vector<wchar_t>(l - sub);
			if (start != 0)
			{
				copy[0] = '\\';
				copy[1] = '\\';
			}
			for (int i = start, j = start; i < l && j < (int)copy.size(); i++)
			{
				wchar_t c = path[i];

				if (c != DirectorySeparatorChar && c != AltDirectorySeparatorChar)
				{
					copy[j++] = c;
					continue;
				}

				// For non-trailing cases.
				if (j + 1 != (int)copy.size())
				{
					copy[j++] = DirectorySeparatorChar;
					for (; i < l - 1; i++)
					{
						c = path[i + 1];
						if (c != DirectorySeparatorChar && c != AltDirectorySeparatorChar)
							break;
					}
				}
			}
			return std::wstring(copy.begin(), copy.end());
		}

		std::wstring Path::NormalizePath(std::wstring path, bool fullCheck)
		{
			if (path.size() >= 4 && path[0] == '\\' && (path[1] == '\\' || path[1] == '?') && path[2] == '?' && path[3] == '\\')
			{
				return path;
			}

			std::wstring normalizedPath = L"";
			int maxPathLength = 260;
			bool expandShortPaths = true;
			if (fullCheck == false)
			{
				normalizedPath = NewNormalizePathLimitedChecks(path, maxPathLength, expandShortPaths);
			}
			else
			{
				//normalizedPath = NewNormalizePath(path, maxPathLength, true);
				normalizedPath = NewNormalizePathLimitedChecks(path, maxPathLength, expandShortPaths);
			}

			if (normalizedPath == L"")
				throw "Arg_PathIllegal";

			return normalizedPath;
		}

		std::wstring Path::NewNormalizePathLimitedChecks(std::wstring path, int maxPathLength, bool expandShortPaths)
		{
			if (path == L"") return path;

			char current;
			int start = PathStartSkip(path);

			if (start == 0)
			{
				// Make a pass to see if we need to normalize so we can potentially skip allocating
				bool normalized = true;

				for (int i = 0; i < path.size(); i++)
				{
					current = path[i];
					if (IsDirectorySeparator(current)
						&& (current != DirectorySeparatorChar
							// Check for sequential separators past the first position (we need to keep initial two for UNC/extended)
							|| (i > 0 && i + 1 < path.size() && IsDirectorySeparator(path[i + 1]))))
					{
						normalized = false;
						break;
					}
				}

				if (normalized) return path;
			}

			std::wstring builder = L"";

			if (IsDirectorySeparator(path[start]))
			{
				start++;
				builder += (wchar_t)DirectorySeparatorChar;
			}

			for (int i = start; i < path.size(); i++)
			{
				current = path[i];

				// If we have a separator
				if (IsDirectorySeparator(current))
				{
					// If the next is a separator, skip adding this
					if (i + 1 < path.size() && IsDirectorySeparator(path[i + 1]))
					{
						continue;
					}

					// Ensure it is the primary separator
					current = DirectorySeparatorChar;
				}

				builder += (wchar_t)current;
			}
			
			return builder;
		}

		int Path::PathStartSkip(std::wstring path)
		{
			int startIndex = 0;
			while (startIndex < path.size() && path[startIndex] == ' ') startIndex++;

			if (startIndex > 0 && (startIndex < path.size() && IsDirectorySeparator(path[startIndex]))
				|| (startIndex + 1 < path.size() && path[startIndex + 1] == VolumeSeparatorChar && IsValidDriveChar(path[startIndex])))
			{
				// Go ahead and skip spaces as we're either " C:" or " \"
				return startIndex;
			}

			return 0;
		}

		bool Path::IsDirectorySeparator(char c)
		{
			return c == DirectorySeparatorChar || c == AltDirectorySeparatorChar;
		}

		bool Path::IsValidDriveChar(char value)
		{
			return ((value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z'));
		}

		int Path::GetRootLength(std::wstring path)
		{
			unsigned int i = 0;
			unsigned long pathLength = path.size();
			unsigned int volumeSeparatorLength = 2;  // Length to the colon "C:"
			unsigned int uncRootLength = 2;          // Length to the start of the server name "\\"
			std::wstring ExtendedPathPrefix = L"\\\\?\\";
			std::wstring UncExtendedPathPrefix = L"\\\\?\\UNC\\";

			bool extendedSyntax = StartsWithOrdinal((char*)path.c_str(), pathLength, ExtendedPathPrefix);
			bool extendedUncSyntax = StartsWithOrdinal((char*)path.c_str(), pathLength, UncExtendedPathPrefix);
			if (extendedSyntax)
			{
				// Shift the position we look for the root from to account for the extended prefix
				if (extendedUncSyntax)
				{

					// "\\" -> "\\?\UNC\"
					uncRootLength = (unsigned int)UncExtendedPathPrefix.size();
				}
				else
				{
					// "C:" -> "\\?\C:"
					volumeSeparatorLength += (unsigned int)ExtendedPathPrefix.size();
				}
			}

			if ((!extendedSyntax || extendedUncSyntax) && pathLength > 0 && IsDirectorySeparator(path[0]))
			{
				// UNC or simple rooted path (e.g. "\foo", NOT "\\?\C:\foo")

				i = 1; //  Drive rooted (\foo) is one character
				if (extendedUncSyntax || (pathLength > 1 && IsDirectorySeparator(path[1])))
				{
					// UNC (\\?\UNC\ or \\), scan past the next two directory separators at most
					// (e.g. to \\?\UNC\Server\Share or \\Server\Share\)
					i = uncRootLength;
					int n = 2; // Maximum separators to skip
					while (i < pathLength && (!IsDirectorySeparator(path[i]) || --n > 0)) i++;
				}
			}
			else if (pathLength >= volumeSeparatorLength && path[volumeSeparatorLength - 1] == VolumeSeparatorChar)
			{
				// Path is at least longer than where we expect a colon, and has a colon (\\?\A:, A:)
				// If the colon is followed by a directory separator, move past it
				i = volumeSeparatorLength;
				if (pathLength >= volumeSeparatorLength + 1 && IsDirectorySeparator(path[volumeSeparatorLength])) i++;
			}
			return i;
		}

		bool Path::StartsWithOrdinal(char* source, unsigned long sourceLength, std::wstring value)
		{
			if (sourceLength < (unsigned long)value.size()) return false;
			for (int i = 0; i < value.size(); i++)
			{
				if (value[i] != source[i]) return false;
			}
			return true;
		}
	}
}