#pragma once
#include <vector>
#include <iostream>
#include "Exception.h"

namespace System
{
	class Buffer
	{
	public:
		Buffer()
		{

		}
		~Buffer()
		{

		}

		template<class T>
		static bool BlockCopy(std::vector<T>& src, int src_offset, std::vector<T>& dest, int dest_offset, int count)
		{
			if (src.empty() || src.size() == 0)
				throw new ArgumentNullException(L"src in null");

			if ((src_offset > (int)src.size() - count) || (dest_offset > (int)dest.size() - count))
				return false;

			T* src_buf = src.data() + src_offset;
			T* dest_buf = dest.data() + dest_offset;

			memcpy(dest_buf, src_buf, count);
			return true;
		}

		template<class T>
		static void InternalBlockCopy(std::vector<T>& src, int src_offset, std::vector<T>& dest, int dest_offset, int count)
		{
			if (src.empty() || src.size() == 0)
				throw new ArgumentNullException(L"src in null");

			if ((src_offset > (int)src.size() - count) || (dest_offset > (int)dest.size() - count))
				return;

			for (int i = src_offset; i < src_offset + count; i++)
			{
				dest[dest_offset + i] = src[i];
			}
		}
	};
}