#pragma once

namespace System
{
	namespace IO
	{
		// 
		enum class FileOptions
		{
			// NOTE: any change to FileOptions enum needs to be 
			// matched in the FileStream ctor for error validation
			None = 0,
			WriteThrough = (int)0x80000000,
			Asynchronous = (int)0x40000000, // FILE_FLAG_OVERLAPPED
			// NoBuffering = 0x20000000,
			RandomAccess = 0x10000000,
			DeleteOnClose = 0x04000000,
			SequentialScan = 0x08000000,
			// AllowPosix = 0x01000000,  // FILE_FLAG_POSIX_SEMANTICS
			// BackupOrRestore,
			// DisallowReparsePoint = 0x00200000, // FILE_FLAG_OPEN_REPARSE_POINT
			// NoRemoteRecall = 0x00100000, // FILE_FLAG_OPEN_NO_RECALL
			// FirstPipeInstance = 0x00080000, // FILE_FLAG_FIRST_PIPE_INSTANCE
			Encrypted = 0x00004000, // FILE_ATTRIBUTE_ENCRYPTED
		};
	}
}