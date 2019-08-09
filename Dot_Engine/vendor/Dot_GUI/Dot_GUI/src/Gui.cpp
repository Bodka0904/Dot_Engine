#include "Gui.h"

std::map<std::string,std::unique_ptr<GuiWrapper>> Gui::m_wrappers;


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
std::string Gui::attachedWrapper;
bool Gui::Initialized = false;

bool Gui::EDIT_WIDGET = false;
std::pair<std::string,int> Gui::attachedWidget;

int Gui::winWidth = 0;
int Gui::winHeight = 0;

float Gui::m_mousePosX = 0;
float Gui::m_mousePosY = 0;
int Gui::m_left_btn_counter = 0;
int Gui::m_right_btn_counter = 0;


bool Gui::HandlingEvent = false;

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
	

	GuiBufferLayout layout = {
		{0,GuiShaderDataType::Float2, "position" },
		{1,GuiShaderDataType::Float2, "texCoords" },
		{2,GuiShaderDataType::Float2, "offsets" },
	};


	m_index = std::make_shared<GuiIndexBuffer>((unsigned int*)& m_wrapper_data.indices[0], 6);
	m_vertex_w.reset(new GuiVertexBuffer((GuiVertex*)&m_wrapper_data.m_vertices[0], 4 * sizeof(GuiVertex)));
	m_vertex_w->SetLayout(layout);


	for (auto& it : m_wrappers)
	{
		it.second->SetWidgetsNextTo();
	}

	Initialized = true;
}


void Gui::Render()
{	
	glEnable(GL_BLEND);
	
	for (auto& it : m_wrappers)
	{
		if (it.second->GetVisible())
		{		
			it.second->DrawTexts(*transform);
	
			guiShader->Bind();
			guiShader->UpdateTransparency(0.8f);
			
			btn_texture->Bind(0);
			transform->SetScale(glm::vec2(1, 1));
			it.second->DrawButtons(*guiShader,*transform);
			
			abtn_texture->Bind(0);
			it.second->DrawArrButtons(*guiShader, *transform);
	
			checkbox_texture->Bind(0);
			it.second->DrawCheckBoxes(*guiShader, *transform);
			
			slider_texture->Bind(0);
			it.second->DrawSliders(*guiShader, *transform);
	
			guiShader->UpdateTransparency(0.6);
			wrp_texture->Bind(0);
			it.second->Draw(*guiShader, *transform);
			
		}
	}
	
	glDisable(GL_BLEND);

}

void Gui::Update()
{
	
	if (EDIT_WIDGET)
	{
		m_wrappers[attachedWidget.first]->SetWidget(attachedWidget.second,glm::vec2(m_mousePosX, m_mousePosY));

	}
	if (EDIT_WRAPPER)
	{	
		m_wrappers[attachedWrapper]->SetData(glm::vec2(m_mousePosX, m_mousePosY));
		m_wrappers[attachedWrapper]->SetWidgetsFollow();
				
	}

	for (auto& it : m_wrappers)
	{
		if (it.second->GetVisible())
		{
			it.second->HandleCheckBoxCallbacks();
		}
	}
	
}


void Gui::Clear()
{
	
}




void Gui::HandleWrapperClick(GuiEvent & event)
{
	if (!EDIT_WIDGET)
	{
		for (auto& it : m_wrappers)
		{
			if (it.second->GetVisible())
			{
				if (it.second->MouseHoover(glm::vec2(m_mousePosX, m_mousePosY)))
				{
					HandlingEvent = true;
					GuiMouseButtonPressEvent& e = (GuiMouseButtonPressEvent&)event;
					if (e.GetButton() == GLFW_MOUSE_BUTTON_LEFT)
					{
						break;
					}
					else if (e.GetButton() == GLFW_MOUSE_BUTTON_MIDDLE)
					{
						it.second->SetWidgetsNextTo();
						break;
					}
					else if (e.GetButton() == GLFW_MOUSE_BUTTON_RIGHT)
					{


						it.second->GetPinned() = false;
						attachedWrapper = it.first;

						EDIT_WRAPPER = true;


						break;
					}

				}
			}
		}
	}
}

void Gui::HandleWidgetClick(GuiEvent& event)
{
	for (auto& it : m_wrappers)
	{
		if (it.second->GetVisible())
		{
			if (it.second->MouseHoover(glm::vec2(m_mousePosX, m_mousePosY)))
			{
				HandlingEvent = true;

				for (int i = 0; i < it.second->GetWidgets().size(); ++i)
				{
					if (it.second->GetWidgets()[i]->MouseHoover(glm::vec2(m_mousePosX, m_mousePosY)))
					{
						GuiMouseButtonPressEvent& e = (GuiMouseButtonPressEvent&)event;
						if (e.GetButton() == GLFW_MOUSE_BUTTON_RIGHT)
						{
							
							attachedWidget = std::pair<std::string,int>(it.first, i);
							attachedWrapper = it.first;
							EDIT_WIDGET = true;
							

						}
						else if (e.GetButton() == GLFW_MOUSE_BUTTON_LEFT)
						{
							if (it.second->GetWidgets()[i]->Clicked() == true)
							{
								it.second->GetWidgets()[i]->Clicked() = false;

							}
							else
							{
								it.second->GetWidgets()[i]->Clicked() = true;
								it.second->HandleButtonCallback(i);
								it.second->HandleSlider(i,m_mousePosX);
							}
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
			HandlingEvent = true;

			if (!m_wrappers[attachedWrapper]->GetPinned())
			{
				if (m_wrappers[attachedWrapper]->PinToSide(glm::vec2(winWidth, winHeight)))
				{			
					m_wrappers[attachedWrapper]->SetWidgetsFollow();
					m_wrappers[attachedWrapper]->SetWidgetsNextTo();
					
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
		switch (action)
		{
		case GLFW_PRESS:
		{
			GuiMouseButtonPressEvent e(button);
			HandleWidgetClick(e);
			HandleWrapperClick(e);	
	
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

				if (m_right_btn_counter > 2)
				{
					m_right_btn_counter = 1;
				}	
				else if (m_left_btn_counter == 2 || m_right_btn_counter == 2)
				{
					m_left_btn_counter = 0;
					m_right_btn_counter = 0;

					GuiMouseButtonReleaseEvent e(button);
					HandleReleaseWrapper(e);
					HandleReleaseWidget(e);

					for (auto& it : m_wrappers)
					{
						it.second->HandleReleaseButton(e, m_mousePosX, m_mousePosY);
					}
				}
			}
			}
			
		}	

		}
		
		Gui::m_handler_mouseButtonCLB(window, button, action, mods);
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

void Gui::AddWrapper(const std::string& name)
{
	m_wrappers[name] = std::unique_ptr<GuiWrapper>();
	m_wrappers[name].reset(new GuiWrapper(name));
}

GuiWrapper & Gui::GetWrapper(const std::string& name)
{
	return *m_wrappers[name];
}

