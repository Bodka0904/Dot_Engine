#include "Gui.h"

typedef std::function<void()> func_ptr;

std::vector<GuiWidget*> Gui::m_widgets;
std::vector<func_ptr> Gui::m_user_callbacks;

GuiIndexBuffer* Gui::m_index = NULL;
GuiVertexBuffer* Gui::m_vertex_b = NULL;
GuiVertexBuffer* Gui::m_vertex_chb = NULL;


GuiShader* Gui::guiShader = NULL;
GuiTransform* Gui::transform = NULL;
GLFWwindow* Gui::m_handler = NULL;

GLFWmousebuttonfun Gui::m_handler_mouseButtonCLB = NULL;
GLFWcursorposfun Gui::m_handler_cursorPosCLB = NULL;
GLFWframebuffersizefun Gui::m_handler_winSizeCLB = NULL;

bool Gui::EDIT_MODE = false;
int Gui::attachedWidget = -1;

int Gui::winWidth = 0;
int Gui::winHeight = 0;

float Gui::m_mousePosX = 0;
float Gui::m_mousePosY = 0;

unsigned int Gui::num_buttons = 0;
unsigned int Gui::num_checkboxes = 0;

void Gui::Init(GLFWwindow * handler, glm::vec3 color)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	
	m_handler = handler;
	m_handler_mouseButtonCLB = glfwSetMouseButtonCallback(handler, Gui::Gui_MouseButtonCallback);
	m_handler_cursorPosCLB = glfwSetCursorPosCallback(handler, Gui::Gui_MousePositionCallback);
	m_handler_winSizeCLB = glfwSetFramebufferSizeCallback(handler, Gui::Gui_WindowSizeCallback);

	GuiText::Init();

	guiShader = new GuiShader();
	guiShader->Init("res/shaders/GuiShader");

	transform = new GuiTransform();

	Button m_button_data = Button(color);
	CheckBox m_checkbox_data = CheckBox(color);

	GuiBufferLayout layout = {
		{ GuiShaderDataType::Float2, "position" },
		{ GuiShaderDataType::Float4, "color" },
	};


	m_index = new GuiIndexBuffer((unsigned int*)&m_button_data.indices[0], 6);
	
	m_vertex_b = new GuiVertexBuffer((GuiVertex*)&m_button_data.m_vertices[0], 4 * sizeof(GuiVertex));
	m_vertex_b->SetLayout(layout);

	for (int i = 0; i < num_buttons; ++i)
	{
		m_widgets[i]->Init(m_vertex_b->GetVBO(), m_index->GetVBO());
		m_widgets[i]->GetText()->SetData(glm::vec2(0, 0));
		m_widgets[i]->SetData(glm::vec2(0, 0));
	}


	m_vertex_chb = new GuiVertexBuffer((GuiVertex*)&m_checkbox_data.m_vertices[0], 4 * sizeof(GuiVertex));
	m_vertex_chb->SetLayout(layout);

	for (int i = num_buttons; i < m_widgets.size(); ++i)
	{
		m_widgets[i]->Init(m_vertex_chb->GetVBO(), m_index->GetVBO());
		m_widgets[i]->GetText()->SetData(glm::vec2(0, 0));
		m_widgets[i]->SetData(glm::vec2(0, 0));
	}

	
}


void Gui::Render()
{	
	GuiText::StartDraw();
	for (auto i : m_widgets)
	{
		i->GetText()->UpdateData(*transform);
		i->GetText()->Draw();

	}

	guiShader->Bind();
	for (auto i : m_widgets)
	{
		i->UpdateData(*transform);
		guiShader->Update(*transform);
		i->Draw();
		
	}
}

void Gui::Update()
{
	if (EDIT_MODE)
	{
		m_widgets[attachedWidget]->SetData(glm::vec2(m_mousePosX,m_mousePosY));
		m_widgets[attachedWidget]->GetText()->SetData(glm::vec2(m_mousePosX, m_mousePosY));		
	}

	for (int i = num_buttons; i < m_widgets.size(); ++i)
	{
		if (m_widgets[i]->Clicked())
		{
			m_user_callbacks[i]();
		}
	}
}

void Gui::Clear()
{
	for (int i = 0; i < m_widgets.size(); ++i)
	{
		delete m_widgets[i];

	}	
	delete m_handler;
	delete guiShader;
	delete transform;

	delete m_index;
	delete m_vertex_b;
	delete m_vertex_chb;
	
}


void Gui::HandleWidgetCallbacks()
{
	for (int i = 0; i < num_buttons; i++)
	{
		if (m_widgets[i]->Clicked())
		{
			m_user_callbacks[i]();

			m_widgets[i]->Clicked() = false;
		}
	}
}

void Gui::HandlePressWidget(GuiEvent& event)
{
	for (int i = 0; i < m_widgets.size(); ++i)
	{
		if (m_widgets[i]->MouseHoover(glm::vec2(m_mousePosX,m_mousePosY)))
		{
			GuiMouseButtonPressEvent& e = (GuiMouseButtonPressEvent&)event;
			if (e.GetButton() == GLFW_MOUSE_BUTTON_RIGHT)
			{
				if (attachedWidget == i)
				{
					attachedWidget = -1;
					EDIT_MODE = false;
				}
				else
				{
					attachedWidget = i;
					EDIT_MODE = true;
				}
			}
			if (e.GetButton() == GLFW_MOUSE_BUTTON_LEFT)
			{
				if (m_widgets[i]->Clicked() == true)
				{
					m_widgets[i]->Clicked() = false;
				}
				else
				{
					m_widgets[i]->Clicked() = true;
				}
			}

		}
		
	}
}

void Gui::HandleReleaseButtons(GuiEvent& event)
{
	for (int i = 0; i < num_buttons; ++i)
	{
		if (m_widgets[i]->MouseHoover(glm::vec2(m_mousePosX, m_mousePosY)))
		{
			GuiMouseButtonReleaseEvent& e = (GuiMouseButtonReleaseEvent&)event;
			if (e.GetButton() == GLFW_MOUSE_BUTTON_LEFT)
			{
				m_widgets[i]->Clicked() = false;
			}
		}
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
			HandlePressWidget(e);
			HandleWidgetCallbacks();
		
		
		}
		case GLFW_RELEASE:
		{
			glfwGetWindowSize(window, &winWidth, &winHeight);
			GuiMouseButtonReleaseEvent e(button);
			HandleReleaseButtons(e);
			
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

		GuiText::UpdateViewPort(width, height);
	}
}

void Gui::AddButton(func_ptr func, const std::string& name)
{
	m_user_callbacks.emplace(m_user_callbacks.begin(), func);
	m_widgets.emplace(m_widgets.begin(),new GuiButton(name));
	num_buttons++;
}

void Gui::AddCheckBox(func_ptr func, const std::string & name)
{	
	m_user_callbacks.emplace(m_user_callbacks.begin() + num_buttons, func);
	m_widgets.emplace(m_widgets.begin() + num_buttons,new GuiCheckBox(name));
	num_checkboxes++;
}

void Gui::SetDarkTheme()
{

	
}
