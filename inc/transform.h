#ifndef TRON_TRANSFORM_H
#define TRON_TRANSFORM_H

#include "pch.h"

namespace tron
{

	class Transform
	{
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;
		
	public:
		Transform();
		
		[[nodiscard]]
		glm::vec3 get_Position() const;
		void put_Position(const glm::vec3& v);

		[[nodiscard]]
		glm::vec3 get_Rotation() const;
		void put_Rotation(const glm::vec3& v);
		
		[[nodiscard]]
		glm::vec3 get_Scale() const;
		void put_Scale(const glm::vec3& v);

		[[nodiscard]]
		glm::mat4 get_Matrix() const;
		
		__declspec(property(get=get_Position, put=put_Position))
		glm::vec3 Position;

		__declspec(property(get=get_Rotation, put=put_Rotation))
		glm::vec3 Rotation;

		__declspec(property(get=get_Scale, put=put_Scale))
		glm::vec3 Scale;

		__declspec(property(get=get_Matrix))
		glm::mat4 Matrix;
	};

}

#endif
