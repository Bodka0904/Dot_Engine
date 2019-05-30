#include "GuiWindow.h"



GuiWindow::GuiWindow(glm::vec2 size, glm::vec3 color)
	:data(size,color)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	data.colors[0].w = transparency;
	data.colors[1].w = transparency;
	data.colors[2].w = transparency;
	data.colors[3].w = transparency;

}


GuiWindow::~GuiWindow()
{
}

void GuiWindow::Init()
{
	for (auto i : buttons)
	{
		i->Init();
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(data.vertices), data.vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &m_vbo[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(data.indices), data.indices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_vbo[2]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data.colors), &data.colors, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindVertexArray(0);
}

void GuiWindow::Draw()
{
	for (auto i : buttons)
	{
		i->Draw();
	}

	glBindVertexArray(m_vao);
	
	glDrawElements(GL_TRIANGLES, sizeof(data.indices) / sizeof(data.indices[0]), GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
}

