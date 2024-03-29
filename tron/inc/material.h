#ifndef MATERIAL_H
#define MATERIAL_H

#include "pch.h"
#include "program.h"
#include "texture.h"
#include "def.h"

namespace tron
{
    class Material
    {
        ptr<Program> m_program;
        ptr<Texture> m_texture;

    public:
        Material(ptr<Program> program, ptr<Texture> texture);

        // Interoperablity with tron-cg
        ~Material() = default;

        ptr<Program> get_Program();
        ptr<Texture> get_Texture();
        
        __declspec(property(get=get_Program))
        const ptr<Program> Program;

        __declspec(property(get=get_Texture))
        const ptr<Texture> Texture;

        void Bind();
    };
}

#endif //MATERIAL_H
