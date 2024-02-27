#include "massert.h"
#include "log.h"

void* malloc_s(void* ptr, size_t size)
{
	if (ptr)
	{
		if (auto* tmp = realloc(ptr, size); tmp)
		{
			ptr = tmp;
			return ptr;
		}
	}
	else if (ptr = malloc(size); ptr)
	{
		return ptr;
	}

	tron::log::crt.err() << "Couldn't allocate memory (" << std::hex << errno << std::dec << ')' << std::endl;
	throw std::system_error(errno, std::system_category());
}
