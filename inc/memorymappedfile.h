#ifndef MEMORYMAPPEDFILE_H
#define MEMORYMAPPEDFILE_H

#if _WIN32
#include <windows.h>
#endif
#include <string>

#include "resource.h"

namespace tron
{
    class MemoryMappedFile
    {
#ifdef _WIN32
		HANDLE m_fd;
		HANDLE m_mmf;
#else
        int    m_fd;
#endif
		void*  m_ptr;

    public:
        MemoryMappedFile(const MemoryMappedFile&) = delete;
        MemoryMappedFile& operator =(const MemoryMappedFile&) = delete;

        explicit MemoryMappedFile(const std::string& path);

        ~MemoryMappedFile() noexcept(false);

        [[nodiscard]]
        void* GetAddress() const;
    };
}

#endif //MEMORYMAPPEDFILE_H
