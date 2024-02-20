#include "memorymappedfile.h"

#include <system_error>

#ifdef _WIN32
#else
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#endif

namespace tron
{
#ifdef _WIN32
#else
    MemoryMappedFile::MemoryMappedFile(const std::string& path)
    {
        m_fd = open(path.c_str(), O_RDWR);
        if (m_fd == -1)
            throw std::system_error(errno, std::system_category(), "Couldn't open fd.");

        struct stat st{};
        if (fstat(m_fd, &st) < 0)
            throw std::system_error(errno, std::system_category(), "Couldn't stat fd.");
        m_size = st.st_size;

        m_ptr = mmap(
            nullptr,
            st.st_size,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE,
            m_fd,
            0);

        if (m_ptr == MAP_FAILED)
            throw std::system_error(errno, std::system_category(), "Couldn't mmap fd.");
    }

    MemoryMappedFile::~MemoryMappedFile() noexcept(false)
    {
        if (munmap(m_ptr, m_size))
            throw std::system_error(errno, std::system_category(), "Couldn't unmap fd.");
    }

    size_t MemoryMappedFile::GetSize() const
    {
        return m_size;
    }

    void* MemoryMappedFile::GetAddress() const
    {
        return m_ptr;
    }
#endif
}
