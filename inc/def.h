#ifndef TRON_DEF_H
#define TRON_DEF_H

#if _DEBUG
#define ASSERT(a) assert(a)
#else
#define ASSERT(a)
#endif

namespace tron
{
    template<typename T>
    using ptr = std::shared_ptr<T>;    
}

#endif //TRON_DEF_H
