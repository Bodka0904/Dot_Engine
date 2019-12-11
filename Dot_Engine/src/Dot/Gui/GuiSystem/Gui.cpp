#include "stdafx.h"
#include "Gui.h"
#include "Dot/Core/Input.h"

#include "Dot/Utils/Text/Font.h"
#include "Dot/Utils/Text/Text.h"

#include <GL/glew.h>


namespace Dot {

	Ref<Texture>	 Gui::s_Texture;
	Ref<ArrayBuffer> Gui::s_VAO;

	std::vector<glm::vec2>  Gui::s_Vertice;
	std::vector <glm::vec2> Gui::s_TexCoord;
	unsigned int			Gui::s_NumWidgets = 0;
	bool					Gui::s_Locked = false;
	glm::vec2				Gui::s_MousePosition;
	std::string				Gui::s_AttachedWidget;
	std::string				Gui::s_AttachedWrapper;
	std::string				Gui::s_ResizedWrapper;
	std::string				Gui::s_EnabledWrapper;


	std::unordered_map<std::string, Ref<Widget>>	 Gui::s_Widget;
	std::unordered_map < std::string, Ref<Wrapper>> Gui::s_Wrapper;

	void Gui::Init(const std::string& texturePack)
	{
		s_VAO = std::make_shared<ArrayBuffer>();
		s_Texture = std::make_shared<Texture>(texturePack,true);

		s_VAO = std::make_shared<ArrayBuffer>();
		Ref<VertexBuffer>s_VBO[2];

		BufferLayout layout = {
				{0,ShaderDataType::Float2,"a_Position"},
		};
		s_VBO[0] = std::make_shared<VertexBuffer>((void*)& s_Vertice[0], s_NumWidgets * sizeof(Quad), D_DYNAMIC_DRAW);
		s_VBO[0]->SetLayout(layout);
		s_VAO->AddVBO(s_VBO[0]);

		BufferLayout layout_tex = {
				{1,ShaderDataType::Float2,"a_TexCoord"},
		};
		s_VBO[1] = std::make_shared<VertexBuffer>(&s_TexCoord[0], s_NumWidgets * sizeof(glm::vec2) * 4,  D_DYNAMIC_DRAW);
		s_VBO[1]->SetLayout(layout_tex);
		s_VAO->AddVBO(s_VBO[1]);

		
		for (auto& wrap : s_Wrapper)
		{
			wrap.second->SetWidgetPosition();
		}

		s_Vertice.clear();
		s_TexCoord.clear();

	}

	
	bool Gui::HandleLeftClick()
	{
		s_MousePosition = glm::vec2(Input::GetMouseX(), Input::GetMouseY());
		for (auto& wrap : s_Wrapper)
		{
			if (wrap.second->MouseResize(s_MousePosition))
			{
				s_ResizedWrapper = wrap.first;
				return true;
			}
			else
			{
				for (auto& widg : wrap.second->GetWidgets())
				{
					if (widg.second->MouseHoover(s_MousePosition))
					{
						widg.second->ClickHandle();
						return true;
					}
				}
			}
		}
		for (auto& widg : s_Widget)
		{
			if (widg.second->MouseHoover(s_MousePosition))
			{
				widg.second->ClickHandle();
				return true;
			}
		}
		return false;
	}


	bool Gui::HandleRightClick()
	{
		s_MousePosition = glm::vec2(Input::GetMouseX(), Input::GetMouseY());
		if (!s_Locked)
		{
			for (auto& wrap : s_Wrapper)
			{
				for (auto& widg : wrap.second->GetWidgets())
				{
					if (widg.second->MouseHoover(s_MousePosition))
					{
						s_AttachedWidget = widg.first;
						s_AttachedWrapper = wrap.first;
						return true;
					}
				}
				if (wrap.second->MouseHoover(s_MousePosition))
				{
					s_AttachedWrapper = wrap.first;
					return true;
				}
			}
			for (auto& it : s_Widget)
			{
				if (it.second->MouseHoover(s_MousePosition))
				{
					s_AttachedWidget = it.first;
					return true;
				}
			}
		}
		return false;
	}
	bool Gui::HandleMiddleClick()
	{
		if (!s_Locked)
			s_Locked = true;
		else
			s_Locked = false;

		return true;
	}
	void Gui::HandleRelease()
	{
		s_AttachedWidget.clear();
		s_AttachedWrapper.clear();	
		s_ResizedWrapper.clear();
	}
	void Gui::UpdatePosBuffer(unsigned int index, unsigned int size, const void* vertices)
	{
		s_VAO->GetVertexBuffer(0)->Update(vertices, size, index * sizeof(Quad));
	}
	void Gui::UpdateTexBuffer(unsigned int index, unsigned int size, const void* texcoords)
	{
		s_VAO->GetVertexBuffer(1)->Update(texcoords, size, index * sizeof(glm::vec2) * 4);
	}

	void Gui::Update()
	{
		glm::vec2 newMousePos;
		newMousePos.x = Input::GetMouseX();
		newMousePos.y = Input::GetMouseY();
		glm::vec2 dif = newMousePos - s_MousePosition;
		s_MousePosition = newMousePos;

		if (!s_AttachedWrapper.empty() && !s_AttachedWidget.empty())
		{
			s_Wrapper[s_AttachedWrapper]->GetWidget(s_AttachedWidget).Move(dif);
		}
		else if (!s_AttachedWidget.empty())
		{
			s_Widget[s_AttachedWidget]->Move(dif);
		}
		else if (!s_AttachedWrapper.empty())
		{
			s_Wrapper[s_AttachedWrapper]->Move(dif);
		}
		else if (!s_ResizedWrapper.empty())
		{
			s_Wrapper[s_ResizedWrapper]->Resize(s_MousePosition);
		}
	}
	
	void Gui::AddWidget(const std::string& label, Ref<Widget> widget, const Quad& quad, glm::vec2* texcoord, int num)
	{
		if (!s_EnabledWrapper.empty())
		{
			s_Wrapper[s_EnabledWrapper]->AddWidget(label,widget,s_NumWidgets);
		}
		else
		{
			s_Widget[label] = widget;
			s_Widget[label]->SetIndex(s_NumWidgets);
		}
		for (int i = 0; i < num; ++i)
		{
			s_Vertice.push_back(quad.m_Vertices[i]);
			s_TexCoord.push_back(texcoord[i]);
		}
		s_NumWidgets++;
	}
	void Gui::AddWrapper(const std::string label, Ref<Wrapper> wrapper, const Quad& quad, glm::vec2* texcoord, int num)
	{
		wrapper->SetIndex(s_NumWidgets);
		s_Wrapper[label] = wrapper;
		for (int i = 0; i < num; ++i)
		{
			s_Vertice.push_back(quad.m_Vertices[i]);
			s_TexCoord.push_back(texcoord[i]);
		}
		s_NumWidgets++;
	}
	void Gui::EnableWrapper(const std::string& label)
	{
		s_EnabledWrapper = label;
	}
	void Gui::DisableWrapper()
	{
		s_EnabledWrapper.clear();
	}
	Wrapper::Wrapper(const std::string& label, const glm::vec2& position, const glm::vec2& size)
		:
		m_Position(position),
		m_Size(size),
		m_Index(0)
	{
	}
	void Wrapper::AddWidget(const std::string& label, Ref<Widget> widget, unsigned int index)
	{
		m_Widget[label] = widget;
		m_Widget[label]->SetIndex(index);
	}
	bool Wrapper::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = GetCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			return true;
		}
		return false;
	}
	bool Wrapper::MouseResize(const glm::vec2& mousePos)
	{
		int offset = 20;
		if (mousePos.x < m_Position.x + m_Size.x && mousePos.x > m_Position.x + m_Size.x - offset // BOTTOM RIGHT CORNER
			&& mousePos.y < m_Position.y + m_Size.y && mousePos.y > m_Position.y + m_Size.y - offset)
		{
			return true;
		}
		return false;
	}
	
	void Wrapper::Resize(const glm::vec2& mousePos)
	{
		m_Size = abs(mousePos - m_Position);
		glm::vec2 newPos[4] = {
			glm::vec2(m_Position),
			glm::vec2(m_Position.x + m_Size.x,m_Position.y),
			glm::vec2(m_Position + m_Size),
			glm::vec2(m_Position.x,m_Position.y + m_Size.y)
		};
		Gui::UpdatePosBuffer(m_Index, sizeof(glm::vec2) * 4, (void*)& newPos[0]);
	}
	void Wrapper::Move(const glm::vec2& pos)
	{
		m_Position += pos;
		for (auto& it : m_Widget)
		{
			it.second->Move(pos);
		}

		glm::vec2 newPos[4] = {
			glm::vec2(m_Position),
			glm::vec2(m_Position.x + m_Size.x,m_Position.y),
			glm::vec2(m_Position + m_Size),
			glm::vec2(m_Position.x,m_Position.y + m_Size.y)
		};
		Gui::UpdatePosBuffer(m_Index, sizeof(glm::vec2) * 4, (void*)& newPos[0]);
	}
	void Wrapper::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		for (auto& it : m_Widget)
		{
			glm::vec2 posDif = it.second->GetPosition() - m_Position;
			it.second->SetPosition(posDif);
		}
			
		glm::vec2 newPos[4] = {
			glm::vec2(m_Position),
			glm::vec2(m_Position.x + m_Size.x,m_Position.y),
			glm::vec2(m_Position + m_Size),
			glm::vec2(m_Position.x,m_Position.y + m_Size.y)
		};	
		Gui::UpdatePosBuffer(m_Index, sizeof(glm::vec2) * 4, (void*)& newPos[0]);	
	}
	void Wrapper::SetWidgetPosition()
	{
		glm::vec2 newPos = m_Position;
		for (auto& it : m_Widget)
		{
			newPos.y += it.second->GetLabelSize().y;
			it.second->SetPosition(newPos);
			newPos.y += it.second->GetSize().y;	
		}
		if (newPos.y > m_Size.y + m_Position.y)
		{
			m_Size.y = newPos.y - m_Position.y;
			glm::vec2 newPos[4] = {
				glm::vec2(m_Position),
				glm::vec2(m_Position.x + m_Size.x,m_Position.y),
				glm::vec2(m_Position + m_Size),
				glm::vec2(m_Position.x,m_Position.y + m_Size.y)
			};
			Gui::UpdatePosBuffer(m_Index, sizeof(glm::vec2) * 4, (void*)& newPos[0]);
		}
	}
	void Wrapper::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size)
	{
		glm::vec2 texCoords[4] = {
			   glm::vec2(0, 0.75),
			   glm::vec2(0.5, 0.75),
			   glm::vec2(0.5, 1),
			   glm::vec2(0, 1)
		};
		Quad quad(position, size);
		Ref<Wrapper> button = std::make_shared<Wrapper>(label, position, size);
		Gui::AddWrapper(label, button, quad, &texCoords[0]);
	}
	glm::vec4 Wrapper::GetCoords()
	{
		return glm::vec4(  m_Position.x,
			m_Position.y + m_Size.y,
			m_Position.x + m_Size.x,
			m_Position.y);
	}
}