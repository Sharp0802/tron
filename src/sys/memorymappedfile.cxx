#include "sys/memorymappedfile.h"

#include <system_error>

#ifdef _WIN32
#include <windows.h>
#include <vector>
#else
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#endif

namespace tron::sys
{
#ifdef _WIN32

#ifndef throw_last_error
#define throw_last_error_DEFINED_IN_LOCAL 1
#define throw_last_error(msg) throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), (msg))
#endif

	MemoryMappedFile::MemoryMappedFile(const std::string& path)
	{
		size_t size = std::mbstowcs(nullptr, path.data(), 0);
		std::vector<wchar_t> wpath(size + 1);
		std::mbstowcs(wpath.data(), path.data(), size + 1);
		wpath[size] = 0;

		m_fd = CreateFileW(
				wpath.data(),
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				nullptr,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				nullptr);
		if (m_fd == INVALID_HANDLE_VALUE)
			throw_last_error("Couldn't open fd.");

		m_mmf = CreateFileMappingW(
				m_fd,
				nullptr,
				PAGE_READWRITE,
				0,
				0,
				nullptr);
		if (m_mmf == nullptr)
			throw_last_error("Couldn't create mapping.");

		m_ptr = MapViewOfFile(
				m_mmf,
				FILE_MAP_WRITE,
				0,
				0,
				0);
		if (m_ptr == nullptr)
			throw_last_error("Couldn't map view.");
	}

	MemoryMappedFile::~MemoryMappedFile() noexcept(false)
	{
		if (!UnmapViewOfFile(m_ptr))
			throw_last_error("Couldn't unmap view");
		if (!CloseHandle(m_mmf))
			throw_last_error("Couldn't close mmf");
		if (!CloseHandle(m_fd))
			throw_last_error("Couldn't close fd");
	}

#if throw_last_error_DEFINED_IN_LOCAL
#undef throw_last_error
#endif

#else
    MemoryMappedFile::MemoryMappedFile(const std::string& path)
    {
        m_fd = open(path.c_str(), O_RDWR);
        if (m_fd == -1)
            throw std::system_error(errno, std::generic_category(), "Couldn't open fd.");

        struct stat st{};
        if (fstat(m_fd, &st) < 0)
            throw std::system_error(errno, std::generic_category(), "Couldn't stat fd.");
        m_size = st.st_size;

        m_ptr = mmap(
            nullptr,
            st.st_size,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE,
            m_fd,
            0);

        if (m_ptr == MAP_FAILED)
            throw std::system_error(errno, std::generic_category(), "Couldn't mmap fd.");
    }

    MemoryMappedFile::~MemoryMappedFile() noexcept(false)
    {
        if (munmap(m_ptr, m_size))
            throw std::system_error(errno, std::generic_category(), "Couldn't unmap fd.");
		if (close(m_fd))
			throw std::system_error(errno, std::generic_category(), "Couldn't close fd");
    }
#endif

	void* MemoryMappedFile::GetAddress() const
	{
		return m_ptr;
	}
}
