#include "pch.h"
#include "transform.h"

namespace
{
	glm::mat4 RotateGlobal(glm::mat4 m, const glm::vec3 rot)
	{
		glm::vec3 yNorm(0, 1, 0);
		glm::vec3 zNorm(0, 0, 1);

		m     = rotate(m, rot.x, glm::vec3(1, 0, 0));
		yNorm = rotate(yNorm, -rot.x, glm::vec3(1, 0, 0));
		m     = rotate(m, rot.y, yNorm);
		zNorm = rotate(zNorm, -rot.y, yNorm);
		m     = rotate(m, rot.z, zNorm);

		return m;
	}

	glm::mat4 Rotate(glm::mat4 m, const glm::vec3 rot)
	{
		constexpr glm::vec3 yNorm(0, 1, 0);
		constexpr glm::vec3 zNorm(0, 0, 1);

		m = rotate(m, rot.x, glm::vec3(1, 0, 0));
		m = rotate(m, rot.y, yNorm);
		m = rotate(m, rot.z, zNorm);

		return m;
	}
}// namespace

namespace tron
{
	Transform::Transform()
		: m_position(glm::vec3(0)),
		  m_rotation(glm::vec3(0)),
		  m_scale(glm::vec3(1))
	{
	}

	glm::vec3 Transform::ToParent(glm::vec3 v) const
	{
		v = rotate(v, Rotation.x, glm::vec3(1, 0, 0));
		v = rotate(v, Rotation.y, glm::vec3(0, 1, 0));
		v = rotate(v, Rotation.z, glm::vec3(0, 0, 1));
		return v;
	}

	glm::vec3 Transform::get_Position() const
	{
		return m_position;
	}

	void Transform::put_Position(const glm::vec3& v)
	{
		m_position = v;
	}

	glm::vec3 Transform::get_LocalPosition() const
	{
		return {0, 0, 0};
	}

	void Transform::put_LocalPosition(const glm::vec3& v)
	{
		Position += ToParent(v);
	}

	glm::vec3 Transform::get_Rotation() const
	{
		return m_rotation;
	}

	void Transform::put_Rotation(const glm::vec3& v)
	{
		m_rotation = v;
	}

	glm::vec3 Transform::get_Scale() const
	{
		return m_scale;
	}

	void Transform::put_Scale(const glm::vec3& v)
	{
		m_scale = v;
	}

	glm::mat4 Transform::get_Matrix() const
	{
		glm::mat4 m(1);
		m = translate(m, Position);
		m = Rotate(m, Rotation);
		m = scale(m, Scale);
		return m;
	}
}// namespace tron
