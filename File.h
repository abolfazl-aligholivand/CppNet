#pragma once
#include "pch.h"
#include "FileStream.h"

namespace System
{
	namespace IO
	{
		class File
		{
		public:
			File();
			~File();

			static bool Exists(const std::wstring path);
			static FileStream* Create(const std::wstring path);
			static FileStream* Create(const std::wstring path, int bufferSize);
			static void Delete(const std::wstring path);
			static std::vector<_byte> ReadAllBytes(const std::wstring path);
			static void Copy(const std::wstring sourceFileName, const std::wstring destFileName);

		private:
			static const int DefaultBufferSize = 8192;
		};

	}
}