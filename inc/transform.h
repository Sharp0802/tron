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
		glm::vec3 ToParent(glm::vec3 v) const;
		
		[[nodiscard]]
		glm::vec3 get_Position() const;
		void put_Position(const glm::vec3& v);

		[[nodiscard]]
		glm::vec3 get_LocalPosition() const;
		void put_LocalPosition(const glm::vec3& v);

		[[nodiscard]]
		glm::vec3 get_Rotation() const;
		void put_Rotation(const glm::vec3& v);
		
		[[nodiscard]]
		glm::vec3 get_Scale() const;
		void put_Scale(const glm::vec3& v);

		[[nodiscard]]
		glm::mat4 get_Matrix() const;

		/// <summary>Position vector with parent's coordinate</summary>
		__declspec(property(get=get_Position, put=put_Position))
		glm::vec3 Position;

		/// <summary>
		/// Position vector with local coordinate.
		/// Because it's on local coordinate,
		/// Getter will return always zero-initialized vector (0, 0, 0).
		/// </summary>
		__declspec(property(get=get_LocalPosition, put=put_LocalPosition))
		glm::vec3 LocalPosition;

		/// <summary>Euler-Rotation vector with parent's coordinate</summary>
		__declspec(property(get=get_Rotation, put=put_Rotation))
		glm::vec3 Rotation;

		/// <summary>Scale vector with parent's coordinate</summary>
		__declspec(property(get=get_Scale, put=put_Scale))
		glm::vec3 Scale;

		/// <summary>Matrix for model</summary>
		__declspec(property(get=get_Matrix))
		glm::mat4 Matrix;
	};

}

#endif
