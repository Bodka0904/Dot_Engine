#pragma once
#include <glm/glm.hpp>

namespace Dot {

	class Text
	{
	public:
		Text(const std::string& text, float x,float y, int sizex,int sizey);
		~Text();

		void PrintText(const std::string& name);
		const glm::vec2& GetPosition() const { return m_Position; }

	private:
		unsigned int m_VAO;
		unsigned int m_VertSize;

		glm::vec2 m_Position;
	};

}