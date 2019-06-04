#include "Gui.h"
#include "stdafx.h"

typedef std::function<void()> func_ptr;

std::vector<func_ptr> Gui::m_user_callbacks_B;

std::vector<GuiButton*> Gui::m_buttons;
GuiShader* Gui::guiShader = NULL;
GLFWwindow* Gui::m_handler = NULL;

GLFWmousebuttonfun Gui::m_handler_mouseButtonCLB = NULL;
GLFWcursorposfun Gui::m_handler_cursorPosCLB = NULL;
GLFWframebuffersizefun Gui::m_handler_winSizeCLB = NULL;

bool Gui::EDIT_MODE = false;

int Gui::winWidth = 0;
int Gui::winHeight = 0;

float Gui::m_mousePosX = 0;
float Gui::m_mousePosY = 0;

void Gui::Init(GLFWwindow * handler)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	
	m_handler = handler;
	m_handler_mouseButtonCLB = glfwSetMouseButtonCallback(handler, Gui::Gui_MouseButtonCallback);
	m_handler_cursorPosCLB = glfwSetCursorPosCallback(handler, Gui::Gui_MousePositionCallback);
	m_handler_winSizeCLB = glfwSetFramebufferSizeCallback(handler, Gui::Gui_WindowSizeCallback);


	guiShader = new GuiShader();
	guiShader->Init("res/shaders/GuiShader");

	for (auto i : m_buttons)
	{
		i->Init();
	}
}



void Gui::Render()
{
	guiShader->Bind();
	for (auto i : m_buttons)
	{

		guiShader->Update(i->GetTranslation());
		i->Draw();
	}
	
}




void Gui::Update()
{
	if (EDIT_MODE)
	{
		m_buttons[GuiButton::GetAttachedButton()]->GetTranslation().x = m_mousePosX - m_buttons[GuiButton::GetAttachedButton()]->GetData().scale / 2;
		m_buttons[GuiButton::GetAttachedButton()]->GetTranslation().y = m_mousePosY - m_buttons[GuiButton::GetAttachedButton()]->GetData().scale / 2;		
	}
}

void Gui::Clear()
{
	for (int i = 0; i < m_buttons.size(); ++i)
	{
		delete m_buttons[i];
	}
	delete m_handler;
}



void Gui::HandleButtonCallbacks()
{
	for (int i = 0; i < m_buttons.size(); i++)
	{
		if (m_buttons[i]->Clicked())
		{
			m_user_callbacks_B[i]();
		}
	}
}

void Gui::HandleButtonsPress(GuiEvent & event)
{
	if (event.GetEventType() == GuiEventType::MouseButtonPressed)
	{
		for (int i = 0; i < m_buttons.size(); ++i)
		{
			if (m_buttons[i]->MouseHoover(glm::vec2(m_mousePosX, m_mousePosY)))
			{
				GuiMouseButtonPressEvent& e = (GuiMouseButtonPressEvent&)event;
				if (e.GetButton() == GLFW_MOUSE_BUTTON_RIGHT)
				{
					if (GuiButton::GetAttachedButton() == i)
					{
						GuiButton::GetAttachedButton() = -1;
						EDIT_MODE = false;
					}
					else
					{
						GuiButton::GetAttachedButton() = i;
						EDIT_MODE = true;
					}
				}
				if (e.GetButton() == GLFW_MOUSE_BUTTON_LEFT)
				{
					m_buttons[i]->GetData().clicked = true;
				}
			
			}
			else
			{
				m_buttons[i]->GetData().clicked = false;
			}
		}
	
	}
	if (event.GetEventType() == GuiEventType::MouseButtonReleased)
	for (int i = 0; i < m_buttons.size(); ++i)
	{
		if (m_buttons[i]->MouseHoover(glm::vec2(m_mousePosX, m_mousePosY)))
		{
			GuiMouseButtonReleaseEvent& e = (GuiMouseButtonReleaseEvent&)event;
			if (e.GetButton() == GLFW_MOUSE_BUTTON_LEFT)
			{
				m_buttons[i]->GetData().clicked = true;
			}
		}
	}
	
}

void Gui::HandleButtonsRelease(GuiEvent& event)
{
	for (int i = 0; i < m_buttons.size(); i++)
	{
		m_buttons[i]->GetData().clicked = false;
	}

}

void Gui::Gui_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

	if (Gui::m_handler_mouseButtonCLB != NULL)
	{
		Gui::m_handler_mouseButtonCLB(window, button, action, mods);
		switch (action)
		{
		case GLFW_PRESS:
		{
			glfwGetWindowSize(window, &winWidth, &winHeight);
			GuiMouseButtonPressEvent e(button);
			HandleButtonsPress(e);
			HandleButtonCallbacks();
		}
		case GLFW_RELEASE:
		{
			glfwGetWindowSize(window, &winWidth, &winHeight);
			GuiMouseButtonReleaseEvent e(button);
			HandleButtonsRelease(e);
			HandleButtonCallbacks();
		}

		}
		

	}
}

void Gui::Gui_MousePositionCallback(GLFWwindow * window, double xPos, double yPos)
{
	if (Gui::m_handler_cursorPosCLB != NULL)
	{
		Gui::m_handler_cursorPosCLB(window, xPos, yPos);
		
		
		m_mousePosX = (xPos / winWidth - 0.5) * 2;
		m_mousePosY = -(yPos / winHeight - 0.5) * 2;
	}
}

void Gui::Gui_WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	if (Gui::m_handler_cursorPosCLB != NULL)
	{
		m_handler_winSizeCLB(window, width, height);

		winWidth = width;
		winHeight = height;

		
	}
}

void Gui::AddButton(func_ptr func)
{
	m_user_callbacks_B.push_back(func); 
	m_buttons.push_back(new GuiButton());
}
