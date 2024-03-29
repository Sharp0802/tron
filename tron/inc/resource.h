#ifndef RESOURCE_H
#define RESOURCE_H

#include "pch.h"

namespace tron
{
    class Resource
    {
    protected:
        const std::string     m_name;
        std::ifstream         m_in;
        const std::streamsize m_size;

    public:
		Resource() = delete;
		Resource(const Resource&) = delete;
		Resource& operator =(const Resource&) = delete;

        explicit Resource(const std::string& name);
    };
}

#endif //RESOURCE_H
