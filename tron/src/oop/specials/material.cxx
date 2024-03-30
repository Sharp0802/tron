#include "oop/specials/material.h"

namespace tron::oop::specials
{
    void Material::Bind()
    {
        m_program.Use();
        m_texture->Bind();
    }

    getter_decl_(Material, Program)
    {
        return &m_program;
    }

    getter_decl_(Material, Texture)
    {
        return m_texture;
    }

    setter_decl_(Material, Texture)
    {
        m_texture = value;
    }
}
