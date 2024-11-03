#pragma once

#include "Core.h"

namespace Citrom::FileSystem
{
	/*struct FilePath
	{
		const char* path;
	};

	class FileBase
	{
	public:
		enum class Mode
		{
			Read,			// r: Open for reading. The file must exist.
			ReadBinary,
			Write,			// w: Open for writing. Creates an empty file or truncates an existing file.
			WriteBinary,
			Append,			// a: Open for appending. Writes data at the end of the file. Creates the file if it does not exist.
			ReadAndWrite,	// r+: Open for reading and writing. The file must exist.
			ReadAndWriteCreate,	// w+: Open for reading and writing. Creates an empty file or truncates an existing file.
			ReadAndAppend,	// a+: Open for reading and appending. The file is created if it does not exist.
		};
	public:
		FileBase(const char* path, const Mode mode) {} // Open
		virtual ~FileBase() {} // Close
		
		virtual size_t Read() = 0;

		virtual void* Data() { return m_Internal; }
	private:
		void* m_Internal = nullptr;
	};*/
}