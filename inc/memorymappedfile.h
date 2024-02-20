#ifndef MEMORYMAPPEDFILE_H
#define MEMORYMAPPEDFILE_H
#include <string>

namespace tron
{
    class MemoryMappedFile
    {
#ifdef _WIN32
#else
        int    m_fd;
        void*  m_ptr;
        size_t m_size;
#endif

    public:
        MemoryMappedFile(const MemoryMappedFile&) = delete;
        MemoryMappedFile& operator =(const MemoryMappedFile&) = delete;

        explicit MemoryMappedFile(const std::string& path);

        ~MemoryMappedFile() noexcept(false);

        [[nodiscard]]
        size_t GetSize() const;

        [[nodiscard]]
        void* GetAddress() const;
    };
}

#endif //MEMORYMAPPEDFILE_H
