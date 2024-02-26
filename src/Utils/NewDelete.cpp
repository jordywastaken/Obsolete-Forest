
#include <stdint.h>

// sysPrxForUser imports
extern "C"
{
    void* _sys_malloc(size_t size);
    void* _sys_memalign(size_t boundary, size_t size);
    void _sys_free(void* ptr);
}

void* operator new(size_t size) 
{
	return _sys_malloc(size);
}

void* operator new(size_t size, size_t boundary)
{
	return _sys_memalign(size, boundary);
}

void* operator new[](size_t size) 
{
	return _sys_malloc(size);
}

void* operator new[](size_t size, size_t boundary)
{
	return _sys_memalign(size, boundary);
}

void operator delete(void* ptr) 
{
	_sys_free(ptr);
}

void operator delete[](void* ptr)
{
	_sys_free(ptr);
}