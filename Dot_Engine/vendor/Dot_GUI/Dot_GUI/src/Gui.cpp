#include "Gui.h"

std::vector<GuiButton*> Gui::m_buttons;
GuiShader* Gui::guiShader = NULL;
GLFWwindow* Gui::m_handler = NULL;
GLFWmousebuttonfun Gui::m_handler_mouseButtonCLB = NULL;
GLFWcursorposfun Gui::m_handler_cursorPosCLB = NULL;
GLFWframebuffersizefun Gui::m_handler_winSizeCLB = NULL;

bool Gui::m_mouseButtonPressedL = false;
bool Gui::m_mouseButtonPressedR = false;
int Gui::m_mousePressesR = 0;

float Gui::m_mousePosX = 0;
float Gui::m_mousePosY = 0;
int Gui::winWidth = 0;
int Gui::winHeight = 0;

void Gui::Init(GLFWwindow * handler,int widthWin,int heightWin)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_handler = handler;
	m_handler_mouseButtonCLB = glfwSetMouseButtonCallback(handler, Gui::Gui_MouseButtonCallback);
	m_handler_cursorPosCLB = glfwSetCursorPosCallback(handler, Gui::Gui_MousePositionCallback);
	m_handler_winSizeCLB = glfwSetFramebufferSizeCallback(handler, Gui::Gui_WindowSizeCallback);

	winWidth = widthWin;
	winHeight = heightWin;

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
		i->Draw();
	}
	
}

void Gui::TranslateWidgets()
{
	if (m_mouseButtonPressedR)
	{
		for (auto i : m_buttons)
		{

			if (i->MouseHoover(glm::vec2(m_mousePosX,m_mousePosY)))
			{
				i->GetTranslation().x = m_mousePosX - 0.1;
				i->GetTranslation().y = m_mousePosY - 0.1;
				guiShader->Update(i->GetTranslation());
			}
			else
			{
				m_mouseButtonPressedR = false;
				m_mousePressesR = 0;
			}
		}
	}
}

void Gui::Update()
{
	Gui::TranslateWidgets();
}

void Gui::Clear()
{
	for (int i = 0; i < m_buttons.size(); ++i)
	{
		delete m_buttons[i];
	}
	delete m_handler;
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
			if (button == GLFW_MOUSE_BUTTON_1)
			{
				Gui::m_mouseButtonPressedL = true;
				
			}
			if (button == GLFW_MOUSE_BUTTON_2)
			{
				Gui::m_mouseButtonPressedR = true;
				Gui::m_mousePressesR++;
				if (Gui::m_mousePressesR == 2)
				{
					m_mouseButtonPressedR = false;
					Gui::m_mousePressesR = 0;
				}
			}
		}
		case GLFW_RELEASE:
		{
			if (button == GLFW_MOUSE_BUTTON_1)
			{
				Gui::m_mouseButtonPressedL = false;
		
			}
			
		}

		}

	}
}

void Gui::Gui_MousePositionCallback(GLFWwindow * window, double xPos, double yPos)
{
	if (Gui::m_handler_cursorPosCLB != NULL)
	{
		Gui::m_handler_cursorPosCLB(window, xPos, yPos);
		
		
		m_mousePosX = (xPos/winWidth - 0.5)*2;
		m_mousePosY = -(yPos/winHeight - 0.5)*2;

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