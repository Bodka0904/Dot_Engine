#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


namespace Dot {

	class Transform
	{
	public:

		Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
			:
			m_pos(pos),
			m_rot(rot),
			m_scale(scale),
			m_transformation(glm::mat4(1))
		{
		}

		inline glm::mat4 GetModel() const
		{
			return m_transformation;
		}

		inline void UpdateModel()
		{
			glm::mat4 posMatrix = glm::translate(m_pos);
			glm::mat4 rotXMatrix = glm::rotate(m_rot.x, glm::vec3(1, 0, 0));
			glm::mat4 rotYMatrix = glm::rotate(m_rot.y, glm::vec3(0, 1, 0));
			glm::mat4 rotZMatrix = glm::rotate(m_rot.z, glm::vec3(0, 0, 1));
			glm::mat4 scaleMatrix = glm::scale(m_scale);

			glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;


			m_transformation = posMatrix * rotMatrix * scaleMatrix;
		}

		inline glm::vec3& GetPos() { return m_pos; }
		inline glm::vec3& GetRot() { return m_rot; }
		inline glm::vec3& GetScale() { return m_scale; }

		inline void SetPos(const glm::vec3& pos) { m_pos = pos; }
		inline void SetRot(const glm::vec3& rot) { m_rot = rot; }
		inline void SetScale(const glm::vec3& scale) { m_scale = scale; }



	private:
		glm::vec3 m_pos;
		glm::vec3 m_rot;
		glm::vec3 m_scale;

		glm::mat4 m_transformation;

	};


	class Transform2D
	{
	public:
		Transform2D(const glm::vec2& pos = glm::vec2(0,0), const glm::vec2& rot = glm::vec2(0,0),const glm::vec2& scale = glm::vec2(1,1))
			:
			m_pos(pos),
			m_rot(rot),
			m_scale(scale),

			m_transformation(glm::mat4(1))
		{
		}

		inline glm::mat4 GetModel() const
		{
			return m_transformation;
		}

		inline void UpdateModel()
		{
			glm::mat4 posMatrix = glm::translate(glm::vec3(m_pos,1.0));
			
			glm::mat4 rotXMatrix = glm::rotate(m_rot.x, glm::vec3(1, 0, 0));
			glm::mat4 rotYMatrix = glm::rotate(m_rot.y, glm::vec3(0, 1, 0));
			
			glm::mat4 scaleMatrix = glm::scale(glm::vec3(m_scale,1.0));

			glm::mat4 rotMatrix = rotYMatrix * rotXMatrix ;


			m_transformation = posMatrix * rotMatrix * scaleMatrix;
		}

		inline glm::vec2& GetPos() { return m_pos; }
		inline glm::vec2& GetRot() { return m_rot; }
		inline glm::vec2& GetScale() { return m_scale; }

		inline void SetPos(const glm::vec2& pos) { m_pos = pos; }
		inline void SetRot(const glm::vec2& rot) { m_rot = rot; }
		inline void SetScale(const glm::vec2& scale) { m_scale = scale; }



	private:
		glm::vec2 m_pos;
		glm::vec2 m_rot;
		glm::vec2 m_scale;

		glm::mat4 m_transformation;

	};

}
