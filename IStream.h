#pragma once
#include "pch.h"

namespace System
{
	namespace Runtime
	{
		namespace InteropServices
		{
			namespace ComTypes
			{
				struct STATSTGS
				{
				public:
					std::wstring pwcsName;
					int type;
					_int64 cbSize;
					int grfMode;
					int grfLocksSupported;
					int grfStateBits;
					int reserved;
				};

				class IStreams
				{
				public:
					virtual void Read(std::vector<_byte> &pv, int cb, intptr_t pcbRead) = 0;
					virtual void Write(std::vector<_byte> &pv, int cb, intptr_t pcbWritten) = 0;

					// IStream portion
					virtual void Seek(_int64 dlibMove, int dwOrigin, intptr_t plibNewPosition) = 0;
					virtual void SetSize(_int64 libNewSize) = 0;
					virtual void CopyTo(IStreams* pstm, _int64 cb, intptr_t pcbRead, intptr_t pcbWritten) = 0;
					virtual void Commit(int grfCommitFlags) = 0;
					virtual void Revert() = 0;
					virtual void LockRegion(_int64 libOffset, _int64 cb, int dwLockType) = 0;
					virtual void UnlockRegion(_int64 libOffset, _int64 cb, int dwLockType) = 0;
					virtual void Stat(STATSTGS* pstatstg, int grfStatFlag) = 0;
					virtual void Clone(IStreams* ppstm) = 0;

				};
			}
		}
	}
}
