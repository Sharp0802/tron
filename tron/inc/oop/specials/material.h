#ifndef MATERIAL_H
#define MATERIAL_H

#include "oop/oop.h"
#include "program.h"
#include "texture.h"

namespace tron::oop::specials
{
    class Material
    {
        Program m_program;
        Texture* m_texture = nullptr;

    public:
        void Bind();

        immutable_(Program*, Program);
        mutable_(Texture*, Texture);
    };
}

#endif //MATERIAL_H
