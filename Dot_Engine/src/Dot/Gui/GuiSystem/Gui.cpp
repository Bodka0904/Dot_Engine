#include "stdafx.h"
#include "Gui.h"
#include "Dot/Core/Input.h"
#include "Dot/Debug/Log.h"

#include "Dot/Utils/Text/Font.h"
#include "Dot/Renderer/RenderCommand.h"

namespace Dot {

	Gui* Gui::s_Instance = NULL;
	void Gui::Init(const std::string& texturePack)
	{
		if (!s_Instance)
		{
			s_Instance = new Gui(texturePack);
		}
	}
	Gui::Gui(const std::string& texturePack)
		:
		m_LabelColor(glm::vec3(0.2, 1, 0.5)),
		m_TextColor(glm::vec3(0.2, 1, 0.5))
	{
		m_NumWidgets = 0;
		m_Texture = Texture2D::Create(texturePack, true);
		m_GuiRenderer = std::make_shared<Renderer2D>(MAX_QUADS);
		m_LabelRenderer = std::make_shared<Renderer2D>(MAX_QUADS * MAX_CHAR_PER_LABEL);
		m_TextRenderer = std::make_shared<Renderer2D>(MAX_QUADS * MAX_TEXT_CHAR);

		for (unsigned int index = 0; index < MAX_QUADS; ++index)
		{
			m_AvailableIndex.push(index);
		}
	}

	Gui::~Gui()
	{
		delete s_Instance;
	}


	bool Gui::HandleLeftClick()
	{
		m_MousePosition = glm::vec2(Input::GetMouseX(), Input::GetMouseY());
		for (auto& wrap : m_Wrapper)
		{
			if (wrap.second->MouseResize(m_MousePosition))
			{
				m_ResizedWrapper = wrap.first;
				return true;
			}
			else
			{
				for (auto& widg : wrap.second->GetWidgets())
				{
					if (widg.second->MouseHoover(m_MousePosition))
					{
						widg.second->ClickHandle();
						return true;
					}
				}
			}
		}
		for (auto& widg : m_Widget)
		{
			if (widg.second->MouseHoover(m_MousePosition))
			{
				widg.second->ClickHandle();
				return true;
			}
		}
		return false;
	}


	bool Gui::HandleRightClick()
	{
		m_MousePosition = glm::vec2(Input::GetMouseX(), Input::GetMouseY());
		for (auto& wrap : m_Wrapper)
		{
			for (auto& widg : wrap.second->GetWidgets())
			{
				if (widg.second->MouseHoover(m_MousePosition))
				{
					m_AttachedWidget = widg.first;
					m_AttachedWrapper = wrap.first;
					return true;
				}
			}

			if (wrap.second->MouseHoover(m_MousePosition))
			{
				m_AttachedWrapper = wrap.first;
				return true;
			}
		}
		for (auto& it : m_Widget)
		{
			if (it.second->MouseHoover(m_MousePosition))
			{
				m_AttachedWidget = it.first;
				return true;
			}
		}
		return false;
	}

	void Gui::HandleRelease()
	{
		m_AttachedWidget.clear();
		m_AttachedWrapper.clear();
		m_ResizedWrapper.clear();
	}
	void Gui::UpdateVertexBuffer(unsigned int index, const QuadVertex* vertices, unsigned int len)
	{
		m_GuiRenderer->PushOffset(vertices, len, index);
	}	

	void Gui::UpdateTextBuffer(unsigned int index, const QuadVertex* vertices, unsigned int len)
	{
		m_TextRenderer->PushOffset(&vertices[0], len, index * MAX_TEXT_CHAR);
	}

	void Gui::UpdateLabelBuffer(unsigned int index, const QuadVertex* vertices, unsigned int len)
	{
		m_LabelRenderer->PushOffset(&vertices[0], len, index * MAX_CHAR_PER_LABEL);
	}

	void Gui::Update()
	{
		glm::vec2 newMousePos;
		newMousePos.x = Input::GetMouseX();
		newMousePos.y = Input::GetMouseY();
		glm::vec2 dif = newMousePos - m_MousePosition;
		m_MousePosition = newMousePos;

		if (!m_AttachedWrapper.empty() && !m_AttachedWidget.empty())
		{
			m_Wrapper[m_AttachedWrapper]->GetWidget(m_AttachedWidget).Move(dif);
		}
		else if (!m_AttachedWidget.empty())
		{
			m_Widget[m_AttachedWidget]->Move(dif);
		}
		else if (!m_AttachedWrapper.empty())
		{
			m_Wrapper[m_AttachedWrapper]->Move(dif);
		}
		else if (!m_ResizedWrapper.empty())
		{
			m_Wrapper[m_ResizedWrapper]->Resize(m_MousePosition);
		}
	}

	void Gui::Render(const Ref<Shader>& shader, const Ref<Shader>& textShader, const Ref<OrthoCamera> camera)
	{
		RenderCommand::Disable(D_DEPTH_TEST);
		m_GuiRenderer->BeginScene(shader, camera);
		{
			m_Texture->Bind(0);
			shader->Bind();
			shader->UploadUniformFloat2("u_MousePos", glm::vec2(Input::GetMouseX(), Input::GetMouseY()));
			m_GuiRenderer->Render();
		}
		m_GuiRenderer->EndScene();
		m_TextRenderer->BeginScene(textShader, camera);
		{
			Font::Bind("Arial");
			textShader->UploadUniformFloat3("u_Color", m_TextColor);
			m_TextRenderer->Render();
		}
		m_TextRenderer->EndScene();

		m_LabelRenderer->BeginScene(textShader, camera);
		{
			Font::Bind("Arial");
			textShader->UploadUniformFloat3("u_Color", m_LabelColor);
			m_LabelRenderer->Render();
		}
		m_LabelRenderer->EndScene();
		RenderCommand::Enable(D_DEPTH_TEST);
	}


	void Gui::AddWidget(const std::string& label, Ref<Widget> widget, bool start)
	{
		if (m_NumWidgets < MAX_QUADS)
		{	
			if (!m_EnabledWrapper.empty())
				m_Wrapper[m_EnabledWrapper]->AddWidget(label, widget,start);
			else
			{
				if (start)
					m_Widget.insert(m_Widget.begin(), std::pair<std::string, Ref<Widget>>(label, widget));
				else
					m_Widget[label] = widget;
			}
			m_NumWidgets++;
		}
		else
		{
			LOG_WARN("Reached maximum of widgets, not possible to add!");
		}
		
	}
	void Gui::AddWrapper(const std::string label, Ref<Wrapper> wrapper)
	{
		if (m_NumWidgets < MAX_QUADS)
		{
			m_Wrapper[label] = wrapper;
			m_NumWidgets += 2;
		}
		else
		{
			LOG_WARN("Reached maximum of widgets, not possible to add!");
		}
	}

	void Gui::RemoveWidget(const std::string& label)
	{

	}
	void Gui::RemoveWrapper(const std::string& label)
	{
		for (auto& it : m_Wrapper[label]->GetWidgets())
		{
			m_AvailableIndex.push(it.second->GetIndex());
			UpdateVertexBuffer(it.second->GetIndex(), &QuadVertex(), 1);
			UpdateTextBuffer(it.second->GetIndex(), &QuadVertex(), 1);
			UpdateLabelBuffer(it.second->GetIndex(), &QuadVertex(), MAX_CHAR_PER_LABEL);
			UpdateTextBuffer(it.second->GetIndex(), &QuadVertex(), MAX_TEXT_CHAR);
			m_NumWidgets--;
		}
		m_Wrapper[label]->RemoveWidgets();
		m_AvailableIndex.push(m_Wrapper[label]->GetIndex().first);
		m_AvailableIndex.push(m_Wrapper[label]->GetIndex().second);

		UpdateVertexBuffer(m_Wrapper[label]->GetIndex().first, &QuadVertex(), 1);
		UpdateVertexBuffer(m_Wrapper[label]->GetIndex().second, &QuadVertex(), 1);
		UpdateLabelBuffer(m_Wrapper[label]->GetIndex().first, &QuadVertex(), MAX_CHAR_PER_LABEL);

		m_Wrapper.erase(label);
		m_NumWidgets -= 2;

	}

	int Gui::PopIndex()
	{
		if (m_NumWidgets < MAX_QUADS)
		{
			unsigned int index = m_AvailableIndex.top();
			m_AvailableIndex.pop();
			return index;
		}
		return -1;
	}

	void Gui::EnableWrapper(const std::string& label)
	{
		if (s_Instance)
			m_EnabledWrapper = label;
	}
	void Gui::DisableWrapper()
	{
		if (s_Instance)
		{
			m_EnabledWrapper.clear();
		}
	}
	Wrapper::Wrapper(const std::string& label, const glm::vec2& position, const glm::vec2& size, int widgetPerCol, bool locked, float labelsize)
		:
		m_Position(position),
		m_Size(size),
		m_Index(0),
		m_ExitButton(glm::vec2(position.x + size.x - 20, position.y), glm::vec2(20, 20)),
		m_WidgetPerCol(widgetPerCol),
		m_Locked(locked)
	{
		glm::vec2 texCoords[8] = {
					  glm::vec2(0, 0.75),
					  glm::vec2(0.25, 0.75),
					  glm::vec2(0.25, 1),
					  glm::vec2(0, 1),
					  // Exit button
					  glm::vec2(0.5, 0.75),
					  glm::vec2(0.75, 0.75),
					  glm::vec2(0.75, 1),
					  glm::vec2(0.5,  1)
		};
		m_Quad[0] = QuadVertex(position, size, &texCoords[0]);
		m_Quad[1] = QuadVertex(glm::vec2(position.x + size.x - 20, position.y), glm::vec2(20, 20), &texCoords[4]);
	
		m_Index = Gui::Get()->PopIndex();
		m_ExitButton.SetIndex(Gui::Get()->PopIndex());
		m_Label = std::make_shared<Text>("Arial", label, glm::vec2(position.x, position.y - Font::GetFont("Arial")->GetData().lineHeight * labelsize), glm::vec2(labelsize, labelsize), MAX_CHAR_PER_LABEL);
	
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad[0]);
		Gui::Get()->UpdateVertexBuffer(m_ExitButton.GetIndex(), &m_Quad[1]);
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0),m_Label->GetNumChar());
	}
	void Wrapper::AddWidget(const std::string& label, Ref<Widget> widget, bool start)
	{
		glm::vec2 offset(10, 0);
		glm::vec2 newPos = m_Position;
		int counter = 1;
		for (auto& it : m_Widget)
		{
			if (m_WidgetPerCol <= counter)
			{
				counter = 0;
				newPos.y = m_Position.y;
				newPos.x += it.second->GetLabelSize().x + it.second->GetSize().x + offset.x;
			}
			else
			{
				newPos.y += it.second->GetLabelSize().y + it.second->GetSize().y;
			}

			if (m_Position.x + m_Size.x < newPos.x + it.second->GetSize().x + offset.x)
			{
				m_Size.x += it.second->GetSize().x + offset.x + 20;
				m_ExitButton.SetPosition(glm::vec2(m_Position.x + m_Size.x - m_ExitButton.GetSize().x, m_Position.y));
				m_Quad[0].SetPosition(m_Position, m_Size);
				Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad[0]);
			}
			if (m_Position.y + m_Size.y < newPos.y + it.second->GetSize().y + offset.y)
			{
				m_Size.y += it.second->GetSize().y + offset.y + 20;
				m_ExitButton.SetPosition(glm::vec2(m_Position.x + m_Size.x - m_ExitButton.GetSize().x, m_Position.y));
				m_Quad[0].SetPosition(m_Position, m_Size);
				Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad[0]);
			}

			counter++;
		}

		if (start)
			m_Widget.insert(m_Widget.begin(),std::pair<std::string,Ref<Widget>>(label,widget));
		else
			m_Widget[label] = widget;

		m_Widget[label]->SetPosition(glm::vec2(newPos.x,newPos.y + m_Widget[label]->GetLabelSize().y));
	}

	void Wrapper::RemoveWidgets()
	{
		m_Widget.clear();
	}

	bool Wrapper::MouseHoover(const glm::vec2& mousePos)
	{
		if (!m_Locked)
		{
			glm::vec4 coords = getCoords();

			if (mousePos.x >= coords.x && mousePos.x <= coords.z
				&& mousePos.y <= coords.y && mousePos.y >= coords.w)
			{
				return true;
			}
		}
		return false;
	}
	bool Wrapper::MouseResize(const glm::vec2& mousePos)
	{
		if (!m_Locked)
		{
			int offset = 20;
			if (mousePos.x < m_Position.x + m_Size.x && mousePos.x > m_Position.x + m_Size.x - offset // BOTTOM RIGHT CORNER
				&& mousePos.y < m_Position.y + m_Size.y && mousePos.y > m_Position.y + m_Size.y - offset)
			{
				return true;
			}
		}
		return false;
	}

	bool Wrapper::Exit(const glm::vec2& mousePos)
	{
		if (m_ExitButton.MouseHoover(mousePos))
		{
			return true;
		}
		return false;
	}

	void Wrapper::Resize(const glm::vec2& mousePos)
	{
		m_Size = abs(mousePos - m_Position);
		m_ExitButton.SetPosition(glm::vec2(m_Position.x + m_Size.x - m_ExitButton.GetSize().x, m_Position.y));

		m_Quad[0].SetPosition(m_Position, m_Size);
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad[0]);
	}
	void Wrapper::Move(const glm::vec2& pos)
	{
		m_Position += pos;
		for (auto& it : m_Widget)
		{
			it.second->Move(pos);
		}
		m_ExitButton.Move(pos);

		m_Quad[0].SetPosition(m_Position, m_Size);
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad[0]);

		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label->GetSize().y));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
	}
	void Wrapper::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		for (auto& it : m_Widget)
		{
			it.second->Move(pos);
		}
		glm::vec2 posDif = m_ExitButton.GetPosition() - m_Position;
		m_ExitButton.SetPosition(posDif);

		m_Quad[0].SetPosition(m_Position, m_Size);
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad[0]);

		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label->GetSize().y));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
	}
	
	void Wrapper::SetWidgetPosition()
	{
		glm::vec2 newPos = m_Position;
		for (auto& it : m_Widget)
		{
			newPos.y += it.second->GetLabelSize().y;
			it.second->SetPosition(newPos);
			newPos.y += it.second->GetSize().y;
			if (m_Size.x < it.second->GetSize().x)
				m_Size.x = it.second->GetSize().x + 20;
		}
		if (newPos.y > m_Size.y + m_Position.y)
		{
			m_Size.y = (newPos.y - m_Position.y) + 20;
			m_ExitButton.SetPosition(glm::vec2(m_Position.x + m_Size.x - m_ExitButton.GetSize().x, m_Position.y));
			m_Quad[0].SetPosition(m_Position, m_Size);
			Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad[0]);
		}
	}
	void Wrapper::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, int widgetPerCol, bool locked, float labelsize)
	{
		if (Gui::Get())
		{			
			D_ASSERT(label.size() < MAX_CHAR_PER_LABEL, "Max len of label is %d", MAX_CHAR_PER_LABEL);
			Ref<Wrapper> wrapper = std::make_shared<Wrapper>(label,position, size, widgetPerCol, locked);
			Gui::Get()->AddWrapper(label, wrapper);
		}
	}
	glm::vec4 Wrapper::getCoords()
	{
		return glm::vec4(m_Position.x,
			m_Position.y + m_Size.y,
			m_Position.x + m_Size.x,
			m_Position.y);
	}
	Wrapper::ActionButton::ActionButton(const glm::vec2& position, const glm::vec2& size)
		:
		m_Position(position), m_Size(size), m_Index(0)
	{
	}
	bool Wrapper::ActionButton::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = getCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			return true;
		}
		return false;
	}
	void Wrapper::ActionButton::Move(const glm::vec2& pos)
	{
		m_Position += pos;
		glm::vec2 texCoords[4] = {
			glm::vec2(0.5, 0.75),
			glm::vec2(0.75, 0.75),
			glm::vec2(0.75, 1),
			glm::vec2(0.5, 1)
		};
		QuadVertex newVertex = QuadVertex(m_Position, m_Size,&texCoords[0]);
		Gui::Get()->UpdateVertexBuffer(m_Index, &newVertex);
	}
	void Wrapper::ActionButton::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		glm::vec2 texCoords[4] = {
			glm::vec2(0.5, 0.75),
			glm::vec2(0.75, 0.75),
			glm::vec2(0.75, 1),
			glm::vec2(0.5, 1)
		};
		QuadVertex newVertex = QuadVertex(m_Position, m_Size, &texCoords[0]);
		Gui::Get()->UpdateVertexBuffer(m_Index, &newVertex);
	}
	glm::vec4 Wrapper::ActionButton::getCoords()
	{
		return glm::vec4(m_Position.x,
			m_Position.y + m_Size.y,
			m_Position.x + m_Size.x,
			m_Position.y);
	}

}