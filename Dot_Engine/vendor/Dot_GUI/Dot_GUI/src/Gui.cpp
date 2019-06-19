#include "Gui.h"



typedef std::function<void()> func_ptr;

std::vector<GuiWrapper*> Gui::m_wrappers;
std::vector<GuiWidget*> Gui::m_widgets;
std::vector<func_ptr> Gui::m_user_callbacks;

GuiIndexBuffer* Gui::m_index = NULL;
GuiVertexBuffer* Gui::m_vertex_w = NULL;
GuiVertexBuffer* Gui::m_vertex_b = NULL;
GuiVertexBuffer* Gui::m_vertex_chb = NULL;
GuiVertexBuffer* Gui::m_vertex_sl = NULL;

GuiShader* Gui::guiShader = NULL;
GuiTransform* Gui::transform = NULL;
GLFWwindow* Gui::m_handler = NULL;

GLFWmousebuttonfun Gui::m_handler_mouseButtonCLB = NULL;
GLFWcursorposfun Gui::m_handler_cursorPosCLB = NULL;
GLFWframebuffersizefun Gui::m_handler_winSizeCLB = NULL;

bool Gui::EDIT_WRAPPER = false;
int Gui::attachedWrapper = -1;

bool Gui::EDIT_WIDGET = false;
int Gui::attachedWidget = -1;

int Gui::winWidth = 0;
int Gui::winHeight = 0;

float Gui::m_mousePosX = 0;
float Gui::m_mousePosY = 0;
int Gui::m_left_btn_counter = 0;
int Gui::m_right_btn_counter = 0;

unsigned int Gui::num_buttons = 0;
unsigned int Gui::num_checkboxes = 0;
unsigned int Gui::num_sliders = 0;

unsigned int Gui::lastWidgetID = 0;


void Gui::Init(GLFWwindow * handler)
{
	glDisable(GL_DEPTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	
	m_handler = handler;
	m_handler_mouseButtonCLB = glfwSetMouseButtonCallback(handler, Gui::Gui_MouseButtonCallback);
	m_handler_cursorPosCLB = glfwSetCursorPosCallback(handler, Gui::Gui_MousePositionCallback);
	m_handler_winSizeCLB = glfwSetFramebufferSizeCallback(handler, Gui::Gui_WindowSizeCallback);
	glfwGetWindowSize(handler, &winWidth, &winHeight);

	
	GuiText::Init();

	guiShader = new GuiShader();
	guiShader->Init("res/shaders/GuiShader");

	transform = new GuiTransform();
	transform->SetOrtho(glm::vec2(winWidth, winHeight));

	Wrapper m_wrapper_data = Wrapper(glm::vec3(0, 0, 0));
	Button m_button_data = Button(glm::vec3(0,0,0));
	CheckBox m_checkbox_data = CheckBox(glm::vec3(0, 0, 0));
	Slider m_slider_data = Slider(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));


	GuiBufferLayout layout = {
		{ GuiShaderDataType::Float2, "position" },
		{ GuiShaderDataType::Float4, "color" },
	};

	m_index = new GuiIndexBuffer((unsigned int*)&m_button_data.indices[0], 6);
	
	m_vertex_w = new GuiVertexBuffer((GuiVertex*)&m_wrapper_data.m_vertices[0], 4 * sizeof(GuiVertex));
	m_vertex_w->SetLayout(layout);

	for (int i = 0; i < m_wrappers.size(); ++i)
	{
		m_wrappers[i]->Init(m_vertex_w->GetVBO(), m_index->GetVBO());
		m_wrappers[i]->SetData(glm::vec2(1, 1));
	}

	m_vertex_b = new GuiVertexBuffer((GuiVertex*)&m_button_data.m_vertices[0], 4 * sizeof(GuiVertex));
	m_vertex_b->SetLayout(layout);

	for (int i = 0; i <  num_buttons; ++i)
	{
		
		m_widgets[i]->Init(m_vertex_b->GetVBO(), m_index->GetVBO());
		m_widgets[i]->GetText()->SetData(glm::vec2(0, 0));
		m_widgets[i]->SetData(glm::vec2(1, 1));
	}


	m_vertex_chb = new GuiVertexBuffer((GuiVertex*)&m_checkbox_data.m_vertices[0], 8 * sizeof(GuiVertex));
	m_vertex_chb->SetLayout(layout);

	for (int i = num_buttons; i < m_widgets.size() - num_sliders; ++i)
	{
		m_widgets[i]->Init(m_vertex_chb->GetVBO(), m_index->GetVBO());
		m_widgets[i]->GetText()->SetData(glm::vec2(0, 0));
		m_widgets[i]->SetData(glm::vec2(1, 1));
	}

	//m_vertex_sl = new GuiVertexBuffer((GuiVertex*)&m_slider_data.m_vertices[0], 8 * sizeof(GuiVertex));
	//m_vertex_sl->SetLayout(layout);
	//
	//
	//for (int i = num_checkboxes + num_buttons; i < m_widgets.size(); ++i)
	//{
	//	m_widgets[i]->Init(m_vertex_sl->GetVBO(), m_index->GetVBO());
	//	m_widgets[i]->GetText()->SetData(glm::vec2(0, 0));
	//	m_widgets[i]->SetData(glm::vec2(0, 0));
	//}
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
		i->Draw(*guiShader,*transform);
	}
	for (auto i : m_wrappers)
	{
		i->Draw(*guiShader, *transform);
	}
	
}

void Gui::Update()
{
	
	if (EDIT_WIDGET)
	{
		m_widgets[attachedWidget]->SetData(glm::vec2(m_mousePosX,m_mousePosY));
		m_widgets[attachedWidget]->GetText()->SetData(glm::vec2(m_mousePosX, m_mousePosY));		
	}
	if (EDIT_WRAPPER)
	{
		m_wrappers[attachedWrapper]->SetData(glm::vec2(m_mousePosX, m_mousePosY));
	
		for (int i = m_wrappers[attachedWrapper]->GetWidgetIndex().x; i < m_wrappers[attachedWrapper]->GetWidgetIndex().y; ++i)
		{	
			m_widgets[i]->SetData(glm::vec2(m_mousePosX - Wrapper::WRAPPER_SIZE_X/2, m_mousePosY - Wrapper::WRAPPER_SIZE_Y/2));
			m_widgets[i]->GetText()->SetData(glm::vec2(m_mousePosX - Wrapper::WRAPPER_SIZE_X/2, m_mousePosY - Wrapper::WRAPPER_SIZE_Y/2));
		}
	}

	HandleCheckBoxCallbacks();
}


void Gui::Clear()
{
	for (int i = 0; i < m_widgets.size(); ++i)
	{
		delete m_widgets[i];

	}	
	for (int i = 0; i < m_wrappers.size(); ++i)
	{
		delete m_wrappers[i];

	}
	delete m_handler;
	delete guiShader;
	delete transform;

	delete m_index;
	delete m_vertex_w;
	delete m_vertex_b;
	delete m_vertex_chb;
	delete m_vertex_sl;
}


void Gui::HandleButtonCallbacks()
{
	for (int i = 0; i < num_buttons ; i++)
	{
		if (m_widgets[i]->Clicked())
		{
			m_user_callbacks[i]();

			m_widgets[i]->GetColor() = 1;
			m_widgets[i]->Clicked() = false;
		}
	}
}

void Gui::HandleCheckBoxCallbacks()
{
	for (int i = num_buttons; i < m_widgets.size() - num_sliders; ++i)
	{
		if (m_widgets[i]->Clicked())
		{
			m_user_callbacks[i]();
		}
	}
}

void Gui::HandleWrapperClick(GuiEvent & event)
{
	for (int i = 0; i < m_wrappers.size(); ++i)
	{
		if (m_wrappers[i]->MouseHoover(glm::vec2(m_mousePosX, m_mousePosY)))
		{
			
			GuiMouseButtonPressEvent& e = (GuiMouseButtonPressEvent&)event;
			if (e.GetButton() == GLFW_MOUSE_BUTTON_LEFT)
			{
				if (m_wrappers[i]->Clicked() == true)
				{
					m_wrappers[i]->Clicked() = false;
				}
				else
				{
					m_wrappers[i]->Clicked() = true;
				}
			}
			if (e.GetButton() == GLFW_MOUSE_BUTTON_RIGHT)
			{
				if (!EDIT_WIDGET)
				{
					attachedWrapper = i;
					EDIT_WRAPPER = true;
				}
			}

		}
	}
}

void Gui::HandleWidgetClick(GuiEvent& event)
{
	for (int i = 0; i < m_widgets.size(); ++i)
	{
		if (m_widgets[i]->MouseHoover(glm::vec2(m_mousePosX,m_mousePosY)))
		{
			GuiMouseButtonPressEvent& e = (GuiMouseButtonPressEvent&)event;
			if (e.GetButton() == GLFW_MOUSE_BUTTON_RIGHT)
			{
				attachedWidget = i;
				EDIT_WIDGET = true;
				
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

void Gui::HandleReleaseWrapper(GuiEvent & event)
{
	GuiMouseButtonReleaseEvent& e = (GuiMouseButtonReleaseEvent&)event;
	if (e.GetButton() == GLFW_MOUSE_BUTTON_RIGHT)
	{
		EDIT_WRAPPER = false;
	}

}

void Gui::HandleReleaseWidget(GuiEvent & event)
{
	GuiMouseButtonReleaseEvent& e = (GuiMouseButtonReleaseEvent&)event;
	if (e.GetButton() == GLFW_MOUSE_BUTTON_RIGHT)
	{
		EDIT_WIDGET = false;
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
				m_widgets[i]->GetColor() = 0;
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
			HandleWidgetClick(e);
			HandleWrapperClick(e);	
			HandleButtonCallbacks();
		
		
		}
		case GLFW_RELEASE:
		{	
			switch (button)
			{
			case GLFW_MOUSE_BUTTON_LEFT:
			{
				m_left_btn_counter++;
			}
			case GLFW_MOUSE_BUTTON_RIGHT:
			{
				m_right_btn_counter++;
			}
			}
			if (m_left_btn_counter == 2 || m_right_btn_counter == 2)
			{
				m_left_btn_counter = 0;
				m_right_btn_counter = 0;

				GuiMouseButtonReleaseEvent e(button);
				HandleReleaseWrapper(e);
				HandleReleaseWidget(e);
				HandleReleaseButtons(e);
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
			
		m_mousePosX = xPos;
		m_mousePosY = yPos;
	}
}

void Gui::Gui_WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	if (Gui::m_handler_cursorPosCLB != NULL)
	{
		
		m_handler_winSizeCLB(window, width, height);
	
		winWidth = width;
		winHeight = height;

		transform->SetOrtho(glm::vec2(width, height));
		GuiText::UpdateViewPort(width, height);
	}
}

void Gui::AddWrapper()
{
	m_wrappers.emplace(m_wrappers.begin(), new GuiWrapper());
	m_wrappers[0]->SetWidgetIndex(glm::vec2(lastWidgetID, m_widgets.size()));
	
	lastWidgetID = m_widgets.size();
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

void Gui::AddSlider(const std::string & name)
{
	m_widgets.emplace(m_widgets.begin() + num_buttons + num_checkboxes, new GuiSlider(name));
	num_sliders++;
}

void Gui::SetDarkTheme()
{

}
