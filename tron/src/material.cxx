#include "pch.h"
#include "material.h"

namespace tron
{
	Material::Material(ptr<tron::Program> program, ptr<tron::Texture> texture)
		: m_program(std::move(program)),
		  m_texture(std::move(texture))
	{
	}

	ptr<Program> Material::get_Program()
	{
		return m_program;
	}

	ptr<Texture> Material::get_Texture()
	{
		return m_texture;
	}

	void Material::Bind()
	{
		m_program->Use();
		m_texture->Bind();
	}
}

namespace tron
{
	EXPORT_CTOR(Material, ptr<Program> program, ptr<Texture> texture)
	{
		return std::make_shared<Material>(program, texture);
	}

	EXPORT_RDT(Material, ptr<Program>, get_Program);
	EXPORT_RDT(Material, ptr<Texture>, get_Texture);
	EXPORT_RDT_void(Material, Bind);
}
