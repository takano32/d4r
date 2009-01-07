#include "ResourceClass.h"

using namespace base;

ResourceClass::ResourceClass(char *path)
{
	_pSystem = new ResourceSystem(path);
}

ResourceClass::~ResourceClass()
{
	delete _pSystem;
}

unsigned int ResourceClass::GetFileSize(char *filename)
{
	return _pSystem->GetFileSize(filename);
}

bool ResourceClass::ReadIn(char *filename, unsigned char *buf, unsigned int buf_size)
{
	unsigned long size_read=0;
	return (0 != _pSystem->ReadIn(filename, buf, buf_size, &size_read));
}
