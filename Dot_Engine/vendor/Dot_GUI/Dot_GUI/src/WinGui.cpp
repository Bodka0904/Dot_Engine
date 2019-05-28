#include "stdafx.h"
#include "WinGui.h"




WinGui::WinGui(WinGuiParams params)
	: g_data(params)
{
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	g_window = glfwCreateWindow(g_data.width, g_data.height,g_data.title, NULL, NULL);
	glfwMakeContextCurrent(g_window);

	if (glewInit() != GLEW_OK)
	{
		LOG_ERR("OpenGLContext: Could not initialize glew")
	};
	glfwSetWindowUserPointer(g_window, this);

	glfwSetCursorEnterCallback(g_window, [](GLFWwindow * window, int entered)
	{
		WinGui* self = (WinGui*)glfwGetWindowUserPointer(window);
		self->OnCursorEnter(entered != GLFW_FALSE);
	});

}




void WinGui::InitWidgets()
{
	// Position
	glGenBuffers(3, button_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, button_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buttons[0]->GetParams().position) * buttons.size(), 0, GL_STATIC_DRAW);
	for (int i = 0 ; i < buttons.size();++i)
	{
		glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(buttons[0]->GetParams().position), sizeof(buttons[i]->GetParams().position), &buttons[i]->GetParams().position);
	}

	// Indices
	glGenBuffers(1, &button_VBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, button_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buttons[0]->GetParams().indices) * buttons.size(), 0, GL_STATIC_DRAW);
	for (int i = 0; i < buttons.size(); ++i)
	{
		glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(buttons[0]->GetParams().indices), sizeof(buttons[i]->GetParams().indices), &buttons[i]->GetParams().indices);
	}

	for (int i = 0; i < buttons.size(); ++i)
	{
		glGenVertexArrays(1, &buttons[i]->GetParams().VAO);
		glBindVertexArray(buttons[i]->GetParams().VAO);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, button_VBO[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(i * sizeof(buttons[0]->GetParams().indices)));
		
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, button_VBO[1]);
}

void WinGui::DrawWidgets()
{
	for (int i = 0; i < buttons.size(); ++i)
	{
		glBindVertexArray(buttons[i]->GetParams().VAO);

		glDrawElements(GL_TRIANGLES, sizeof(buttons[i]->GetParams().indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}
}

void WinGui::Update()
{
	glfwMakeContextCurrent(g_window);
}



GLFWwindow * WinGui::GetWindow() const
{
	return g_window;
}



	




