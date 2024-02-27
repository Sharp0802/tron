#ifndef TRON_MASSERT_H
#define TRON_MASSERT_H

#include <cstdint>
#include <cstdlib>

__attribute__((alloc_size(2), malloc(::free), returns_nonnull))
void* malloc_s(void* ptr, size_t size);

#endif //TRON_MASSERT_H
