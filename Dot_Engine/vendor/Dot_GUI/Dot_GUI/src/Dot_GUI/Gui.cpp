#include "Gui.h"

std::vector<std::unique_ptr<GuiWrapper>> Gui::m_wrappers;


std::unique_ptr<GuiShader>	Gui::guiShader = NULL;
std::unique_ptr<GuiTransform> Gui::transform = NULL;
std::unique_ptr<GuiTexture>	Gui::wrp_texture = NULL;
std::unique_ptr<GuiTexture>	Gui::btn_texture = NULL;
std::unique_ptr<GuiTexture>	Gui::abtn_texture = NULL;
std::unique_ptr<GuiTexture>	Gui::checkbox_texture = NULL;
std::unique_ptr<GuiTexture>	Gui::slider_texture = NULL;

GLFWwindow* Gui::m_handler = NULL;

GLFWmousebuttonfun Gui::m_handler_mouseButtonCLB = NULL;
GLFWcursorposfun Gui::m_handler_cursorPosCLB = NULL;
GLFWframebuffersizefun Gui::m_handler_winSizeCLB = NULL;

bool Gui::EDIT_WRAPPER = false;
int Gui::attachedWrapper = 0;
bool Gui::Initialized = false;

bool Gui::EDIT_WIDGET = false;
glm::vec2 Gui::attachedWidget = glm::vec2(0,0);

int Gui::winWidth = 0;
int Gui::winHeight = 0;

float Gui::m_mousePosX = 0;
float Gui::m_mousePosY = 0;
int Gui::m_left_btn_counter = 0;
int Gui::m_right_btn_counter = 0;



unsigned int Gui::num_wrappers = 0;

std::shared_ptr<GuiIndexBuffer>  Gui::m_index		= NULL;	
std::shared_ptr<GuiVertexBuffer> Gui::m_vertex_w	= NULL;
std::shared_ptr<GuiVertexBuffer> Gui::m_vertex_b	= NULL;
std::shared_ptr<GuiVertexBuffer> Gui::m_vertex_ab	= NULL;
std::shared_ptr<GuiVertexBuffer> Gui::m_vertex_chb	= NULL;
std::shared_ptr<GuiVertexBuffer> Gui::m_vertex_sl	= NULL;

void Gui::Init(GLFWwindow * handler)
{
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	
	m_handler = handler;
	m_handler_mouseButtonCLB = glfwSetMouseButtonCallback(handler, Gui_MouseButtonCallback);
	m_handler_cursorPosCLB = glfwSetCursorPosCallback(handler, Gui_MousePositionCallback);
	m_handler_winSizeCLB = glfwSetFramebufferSizeCallback(handler, Gui_WindowSizeCallback);
	glfwGetWindowSize(handler, &winWidth, &winHeight);

	
	GuiText::Init();

	guiShader.reset(new GuiShader());
	guiShader->Init("res/shaders/Gui/GuiShader");

	wrp_texture.reset(new GuiTexture());
	wrp_texture->Create("res/textures/Gui/wrapper.jpg");
	btn_texture.reset(new GuiTexture());
	btn_texture->Create("res/textures/Gui/button.jpg");
	abtn_texture.reset(new GuiTexture());
	abtn_texture->Create("res/textures/Gui/arrows.jpg");
	checkbox_texture.reset(new GuiTexture());
	checkbox_texture->Create("res/textures/Gui/checkbox.jpg");
	slider_texture.reset(new GuiTexture());
	slider_texture->Create("res/textures/Gui/slider.jpg");

	transform.reset(new GuiTransform());
	transform->SetOrtho(glm::vec2(winWidth, winHeight));

	Wrapper m_wrapper_data = Wrapper();
	Button m_button_data = Button();
	CheckBox m_checkbox_data = CheckBox();
	Slider m_slider_data = Slider();
	ArrowButton m_arrowbtn_data = ArrowButton();


	GuiBufferLayout layout = {
		{ GuiShaderDataType::Float2, "position" },
		{ GuiShaderDataType::Float2, "texCoords" },
	};


	m_index.reset(new GuiIndexBuffer((unsigned int*)&m_button_data.indices[0], 6));	

	m_vertex_w.reset(new GuiVertexBuffer((GuiVertex*)&m_wrapper_data.m_vertices[0], 4 * sizeof(GuiVertex)));
	m_vertex_w->SetLayout(layout);
	m_vertex_b.reset(new GuiVertexBuffer((GuiVertex*)&m_button_data.m_vertices[0], 4 * sizeof(GuiVertex)));
	m_vertex_b->SetLayout(layout);
	m_vertex_ab.reset(new GuiVertexBuffer((GuiVertex*)&m_arrowbtn_data.m_vertices[0], 4 * sizeof(GuiVertex)));
	m_vertex_ab->SetLayout(layout);
	m_vertex_chb.reset(new GuiVertexBuffer((GuiVertex*)&m_checkbox_data.m_vertices[0], 4 * sizeof(GuiVertex)));
	m_vertex_chb->SetLayout(layout);
	m_vertex_sl.reset(new GuiVertexBuffer((GuiVertex*)&m_slider_data.m_vertices[0], 4 * sizeof(GuiVertex)));
	m_vertex_sl->SetLayout(layout);

	for (unsigned int i = 0; i < m_wrappers.size(); ++i)
	{
	
		m_wrappers[i]->Init(m_vertex_w->GetVBO(), m_index->GetVBO());
		m_wrappers[i]->SetData(glm::vec2(1, 1));
		m_wrappers[i]->InitButtons(m_vertex_b->GetVBO(), m_index->GetVBO());
		m_wrappers[i]->InitArrButtons(m_vertex_ab->GetVBO(), m_index->GetVBO());
		m_wrappers[i]->InitCheckBoxes(m_vertex_chb->GetVBO(), m_index->GetVBO());
		m_wrappers[i]->InitSliders(m_vertex_sl->GetVBO(), m_index->GetVBO());
	
	}

	Initialized = true;
}


void Gui::Render()
{	
	
	for (unsigned int j = 0; j < m_wrappers.size(); ++j)
	{
		if (m_wrappers[j]->GetVisible())
		{
			
			m_wrappers[j]->DrawTexts(*transform);
	
			guiShader->Bind();
			guiShader->UpdateTransparency(0.8f);
			
			btn_texture->Bind(0);
			m_wrappers[j]->DrawButtons(*guiShader,*transform);
	
			abtn_texture->Bind(0);
			m_wrappers[j]->DrawArrButtons(*guiShader, *transform);
	
			checkbox_texture->Bind(0);
			m_wrappers[j]->DrawCheckBoxes(*guiShader, *transform);
			
			slider_texture->Bind(0);
			m_wrappers[j]->DrawSliders(*guiShader, *transform);
	
			guiShader->UpdateTransparency(0.6);
			wrp_texture->Bind(0);
			m_wrappers[j]->Draw(*guiShader, *transform);
			
	
		}
	}
	
	glDepthFunc(GL_LESS);
}

void Gui::Update()
{
	
	if (EDIT_WIDGET)
	{
		m_wrappers[attachedWidget.x]->SetWidget(attachedWidget.y,glm::vec2(m_mousePosX, m_mousePosY));

	}
	if (EDIT_WRAPPER)
	{	
		m_wrappers[attachedWrapper]->SetData(glm::vec2(m_mousePosX, m_mousePosY));
		m_wrappers[attachedWrapper]->SetWidgetsFollow();
				
	}

	for (int j = 0; j < m_wrappers.size(); ++j)
	{
		if (m_wrappers[j]->GetVisible())
		{
			m_wrappers[j]->HandleCheckBoxCallbacks();
		}
	}
	
}


void Gui::Clear()
{
}




void Gui::HandleWrapperClick(GuiEvent & event)
{
	for (int i = 0; i < m_wrappers.size(); ++i)
	{
		if (m_wrappers[i]->GetVisible())
		{
			if (m_wrappers[i]->MouseHoover(glm::vec2(m_mousePosX, m_mousePosY)))
			{

				GuiMouseButtonPressEvent& e = (GuiMouseButtonPressEvent&)event;
				if (e.GetButton() == GLFW_MOUSE_BUTTON_LEFT)
				{
					
					if (m_wrappers[i]->Exit(glm::vec2(m_mousePosX, m_mousePosY)))
					{

					}
				}
				else if (e.GetButton() == GLFW_MOUSE_BUTTON_MIDDLE)
				{
					m_wrappers[i]->SetWidgetsNextTo();
				}
				else if (e.GetButton() == GLFW_MOUSE_BUTTON_RIGHT)
				{
					if (!EDIT_WIDGET)
					{
						//m_wrappers[attachedWrapper]->SetWidgets(m_wrappers[attachedWrapper]->GetCenter());
						m_wrappers[i]->GetPinned() = false;
						attachedWrapper = i;
						
						EDIT_WRAPPER = true;
					}
				}

			}
		}
	}
}

void Gui::HandleWidgetClick(GuiEvent& event)
{
	for (int j = 0; j < m_wrappers.size(); ++j)
	{
		if (m_wrappers[j]->GetVisible() && m_wrappers[j]->MouseHoover(glm::vec2(m_mousePosX, m_mousePosY)))
		{
			for (int i = 0; i < m_wrappers[j]->GetWidgets().size(); ++i)
			{
				if (m_wrappers[j]->GetWidgets()[i]->MouseHoover(glm::vec2(m_mousePosX, m_mousePosY)))
				{
					GuiMouseButtonPressEvent& e = (GuiMouseButtonPressEvent&)event;
					if (e.GetButton() == GLFW_MOUSE_BUTTON_RIGHT)
					{
						attachedWidget = glm::vec2(j, i);
						EDIT_WIDGET = true;

					}
					else if (e.GetButton() == GLFW_MOUSE_BUTTON_LEFT)
					{
						if (m_wrappers[j]->GetWidgets()[i]->Clicked() == true)
						{
							m_wrappers[j]->GetWidgets()[i]->Clicked() = false;
						}
						else
						{
							m_wrappers[j]->GetWidgets()[i]->Clicked() = true;
						}
					}
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

		if (m_wrappers[attachedWrapper]->MouseHoover(glm::vec2(m_mousePosX, m_mousePosY)))
		{
			
			if (!m_wrappers[attachedWrapper]->GetPinned())
			{
				if (m_wrappers[attachedWrapper]->PinToSide(glm::vec2(winWidth, winHeight)))
				{
					for (int j = 0; j < m_wrappers[attachedWrapper]->GetWidgets().size(); ++j)
					{
						m_wrappers[attachedWrapper]->GetWidgets()[j]->SetData(m_wrappers[attachedWrapper]->GetCenter());
					}
				}
			}
		}
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
	

			for (unsigned int j = 0; j < m_wrappers.size(); ++j)
			{
				if (m_wrappers[j]->GetVisible())
				{
					m_wrappers[j]->HandleButtonCallbacks();
					m_wrappers[j]->HandleSliders(m_mousePosX);
					
				}
			}
		
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
				for (unsigned int j = 0; j < m_wrappers.size(); ++j)
				{
					if (m_wrappers[j]->GetVisible())
					{
						m_wrappers[j]->HandleReleaseButton(e,m_mousePosX,m_mousePosY);
					}
				}
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
			
		m_mousePosX = (float)xPos;
		m_mousePosY = (float)yPos;
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

void Gui::AddWrapper(const std::string& name,int& index)
{
	m_wrappers.emplace(m_wrappers.begin()+num_wrappers, new GuiWrapper(name));
	num_wrappers++;
	index = num_wrappers-1;
}

GuiWrapper & Gui::GetWrapper(int index)
{
	return *m_wrappers[index];
}

