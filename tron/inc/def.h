#ifndef TRON_DEF_H
#define TRON_DEF_H

#if _DEBUG
#define ASSERT(a) assert(a)
#else
#define ASSERT(a)
#endif

#define EXPORT extern "C" __attribute__((visibility("default")))


#include "pch.h"

namespace tron
{
    template<typename T>
    using ptr = std::shared_ptr<T>;
}

#define EXPORT_RDT_void(cls, fn) \
EXPORT void cls##__##fn(cls* self) { self-> fn (); }

#define EXPORT_RDT(cls, ret, fn) \
EXPORT ret cls##__##fn(cls* self) { return self-> fn (); }

#define EXPORT_MAP(cls, ret, fn, ...)       \
EXPORT ret cls##__##fn(cls* self, __VA_ARGS__)

#define EXPORT_CTOR(cls, ...) \
EXPORT tron::ptr<cls> cls##__ctor(__VA_ARGS__)

#endif //TRON_DEF_H
