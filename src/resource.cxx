#include "pch.h"
#include "resource.h"

namespace
{
    std::string GetResourcePath(const std::string& src)
    {
        return "res/" + src;
    }

    std::streamsize GetFileSize(std::ifstream& stream)
    {
        stream.ignore(std::numeric_limits<std::streamsize>::max());
        const std::streamsize length = stream.gcount();
        stream.clear();
        stream.seekg(0, std::ios_base::beg);
        return length;
    }
}

namespace tron
{
    Resource::Resource(const std::string& name): m_name(GetResourcePath(name)),
                                                 m_in(GetResourcePath(name), std::ifstream::ate | std::ifstream::binary),
                                                 m_size(GetFileSize(m_in))
    {

    }
}
