#pragma once

#include "ResourceSystem.h"

namespace base
{
	class ResourceClass
	{
	public:
		ResourceClass(char* path);
		virtual ~ResourceClass();

		unsigned int GetFileSize(char* filename);
		bool ReadIn(char* filename, unsigned char* buf, unsigned int buf_size);
	private:
		ResourceSystem* _pSystem;
		bool _isArchive;
	};
}
