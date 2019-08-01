#pragma once
#define GLT_IMPLEMENTATION
#define GLT_MANUAL_VIEWPORT
#include <GL/glew.h>
#include "gltext.h"
#include <string>
#include <glm/glm.hpp>
#include "graphics/GuiTransform.h"

class GuiText
{
public:
	GuiText(const std::string & rendText, const glm::vec2& offset,const glm::vec2& scale = glm::vec2(0.8,0.8));
	~GuiText();

	static void Init();
	static void StartDraw();
	static void UpdateViewPort(int width, int height);


	void UpdateData(GuiTransform& transform);
	void SetData(glm::vec2 pos);
	void Draw();


	

private:
	GLTtext *text;
	glm::vec2 m_position;
	glm::vec2 m_scale;
	glm::vec2 m_offset;

	
	std::string m_text;
	static glm::vec2 m_windowSize;

};

