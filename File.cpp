#include "File.h"
#include <ostream>

namespace System
{
	namespace IO
	{
		File::File()
		{

		}

		File::~File()
		{

		}

		bool File::Exists(const std::wstring path)
		{
			std::ifstream file(path.c_str());
			auto tem = file.good();
			file.close();
			return tem;
			//file.
		}

		FileStream* File::Create(const std::wstring path)
		{
			return Create(path, DefaultBufferSize);
		}

		FileStream* File::Create(const std::wstring path, int bufferSize)
		{
			return new FileStream(path, FileMode::Create, FileAccess::ReadWrite, FileShare::None);
		}

		void File::Delete(const std::wstring path)
		{
			if(path == L"")
				throw "Path is empty";

			std::wstring fullPath = Path::GetFullPath(path);

			if (Exists(fullPath))
			{
				std::string p(path.begin(), path.end());
				if (remove(p.c_str()) != 0)
					throw "Error deleting file";
			}
		}

		std::vector<_byte> File::ReadAllBytes(const std::wstring path)
		{
			std::vector<_byte> bytes;
			FileStream* stream = new FileStream(path, FileMode::Open, FileAccess::Read, FileShare::None);
			stream->Read(bytes, 0, stream->Length());
			return bytes;
		}

		void File::Copy(const std::wstring sourceFileName, const std::wstring destFileName)
		{
			if (!Exists(sourceFileName))
				throw "Source file not exists";

			if (Exists(destFileName))
				throw "The destination file already exists";


			//sourceFile or destFile is directory
			//if (Directory::Exists(sourceFileName) || Directory::Exists(destFileName))
			//	throw "The source path or destination path is a directory";

			std::ifstream source(sourceFileName.c_str(), std::ios::binary);
			std::ofstream dest(destFileName.c_str(), std::ios::binary);

			std::istreambuf_iterator<char> begin_source(source);
			std::istreambuf_iterator<char> end_source;
			std::ostreambuf_iterator<char> begin_dest(dest);
			std::copy(begin_source, end_source, begin_dest);

			source.close();
			dest.close();
		}

	}
}