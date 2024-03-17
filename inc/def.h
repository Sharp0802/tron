#ifndef TRON_DEF_H
#define TRON_DEF_H

// TODO: Fix preprocessor
#if TRON_RELEASE
#define ASSERT(a)
#else
#define ASSERT(a) assert(a)
#endif

namespace tron
{
    template<typename T>
    using ptr = std::shared_ptr<T>;    
}

#endif //TRON_DEF_H
