#ifndef MEMORYMAPPEDFILE_H
#define MEMORYMAPPEDFILE_H

#include "pch.h"

namespace tron::sys
{
    class MemoryMappedFile
    {
#ifdef _WIN32
		HANDLE m_fd;
		HANDLE m_mmf;
#else
        int    m_fd;
        size_t m_size;
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
