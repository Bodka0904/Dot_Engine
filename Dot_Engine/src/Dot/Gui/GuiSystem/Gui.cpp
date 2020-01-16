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
		m_NumQuads = 0;
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
		for (auto& console : m_Console)
		{
			if (console.second->MouseHoover(m_MousePosition))
			{
				console.second->ClickHandle();
				return true;
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
		for (auto& console : m_Console)
		{
			if (console.second->MouseHoover(m_MousePosition))
			{
				m_AttachedConsole = console.first;
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
		m_AttachedConsole.clear();
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
		else if (!m_AttachedConsole.empty())
		{
			m_Console[m_AttachedConsole]->Move(dif);
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
		
		for (auto it : m_Console)
		{
			it.second->Render(textShader, camera);
		}
		RenderCommand::Enable(D_DEPTH_TEST);
	}


	void Gui::AddWidget(const std::string& label, Ref<Widget> widget, bool start)
	{
		if (m_NumQuads < MAX_QUADS)
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
		}
		else
		{
			LOG_WARN("Reached maximum of widgets, not possible to add!");
		}
	}
	void Gui::AddConsole(const std::string& label, Ref<Console> console)
	{
		if (m_NumQuads < MAX_QUADS)
		{
			m_Console[label] = console;
		}
		else
		{
			LOG_WARN("Reached maximum of widgets, not possible to add!");
		}
	}
	void Gui::AddWrapper(const std::string label, Ref<Wrapper> wrapper)
	{
		if (m_NumQuads < MAX_QUADS)
		{
			m_Wrapper[label] = wrapper;
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
		std::vector<QuadVertex> quadLabel;
		quadLabel.resize(MAX_CHAR_PER_LABEL);
		std::vector<QuadVertex> quadText;
		quadText.resize(MAX_TEXT_CHAR);

		for (auto& it : m_Wrapper[label]->GetWidgets())
		{
			UpdateVertexBuffer(it.second->GetIndex(), &QuadVertex(), 1);		
			UpdateLabelBuffer(it.second->GetIndex(), &quadLabel[0], MAX_CHAR_PER_LABEL);
			UpdateTextBuffer(it.second->GetIndex(), &quadText[0], MAX_TEXT_CHAR);
		}	
		UpdateVertexBuffer(m_Wrapper[label]->GetIndex(), &QuadVertex(), 1);
		UpdateLabelBuffer(m_Wrapper[label]->GetIndex(), &quadLabel[0], MAX_CHAR_PER_LABEL);
		m_Wrapper.erase(label);
	}

	void Gui::RemoveConsole(const std::string& label)
	{
	}

	int Gui::PopIndex()
	{
		if (m_NumQuads < MAX_QUADS)
		{
			m_NumQuads++;
			unsigned int index = m_AvailableIndex.top();
			m_AvailableIndex.pop();
			return index;
		}
		return -1;
	}

	void Gui::PushIndex(unsigned int index)
	{
		m_AvailableIndex.push(index);
		m_NumQuads--;
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
		m_WidgetPerCol(widgetPerCol),
		m_Locked(locked)
	{
		glm::vec2 texCoords[4] = {
					 glm::vec2(0.0f, 0.75f),
					 glm::vec2(0.25f, 0.75f),
					 glm::vec2(0.25, 1.0f),
					 glm::vec2(0.0f, 1.0f)
		};
		m_Quad = QuadVertex(position, size, &texCoords[0]);	
		m_Index = Gui::Get()->PopIndex();
		m_Label = std::make_shared<Text>("Arial", label, glm::vec2(position.x, position.y ), glm::vec2(labelsize, labelsize), MAX_CHAR_PER_LABEL);
		m_Panel = Panel(m_Position, glm::vec2(m_Size.x, m_Label->GetSize().y));

		m_Panel.Index = Gui::Get()->PopIndex();
		m_Panel.IndexButton = Gui::Get()->PopIndex();

		Gui::Get()->UpdateVertexBuffer(m_Panel.Index, &m_Panel.Quad[0]);
		Gui::Get()->UpdateVertexBuffer(m_Panel.IndexButton, &m_Panel.Quad[1]);

		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0),m_Label->GetNumChar());
	}
	Wrapper::~Wrapper()
	{
		Gui::Get()->UpdateVertexBuffer(m_Panel.Index, &QuadVertex());
		Gui::Get()->UpdateVertexBuffer(m_Panel.IndexButton, &QuadVertex());
		
		Gui::Get()->PushIndex(m_Index);
		Gui::Get()->PushIndex(m_Panel.Index);
		Gui::Get()->PushIndex(m_Panel.IndexButton);
		m_Widget.clear();
	}
	void Wrapper::AddWidget(const std::string& label, Ref<Widget> widget, bool start)
	{
		glm::vec2 offset(10, 0);
		glm::vec2 newPos = glm::vec2(m_Position.x,m_Position.y + m_Label->GetSize().y);
		int counter = 1;
		for (auto& it : m_Widget)
		{
			if (m_WidgetPerCol <= counter)
			{
				counter = 0;
				newPos.y = m_Position.y + m_Label->GetSize().y;
				if (it.second->GetLabelSize().x > it.second->GetSize().x)
					newPos.x += it.second->GetLabelSize().x + offset.x;
				else
					newPos.x += it.second->GetSize().x + offset.x;
			}
			else
			{
				newPos.y += it.second->GetLabelSize().y + it.second->GetSize().y;
			}

			if (m_Position.x + m_Size.x < newPos.x + it.second->GetSize().x + offset.x)
			{
				m_Size.x = it.second->GetSize().x + offset.x + 20;
				m_Quad.SetPosition(m_Position, m_Size);
				m_Panel.Size.x = m_Size.x;
				m_Panel.SetPosition(m_Position);

				Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
				Gui::Get()->UpdateVertexBuffer(m_Panel.Index, &m_Panel.Quad[0]);
				Gui::Get()->UpdateVertexBuffer(m_Panel.IndexButton, &m_Panel.Quad[1]);

			}
			if (m_Position.y + m_Size.y < newPos.y + it.second->GetSize().y + offset.y)
			{
				m_Size.y += it.second->GetSize().y + offset.y + 20;
				m_Quad.SetPosition(m_Position, m_Size);
				Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);

				m_Panel.Size.y = m_Size.y;
				m_Panel.SetPosition(m_Position);

				Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
				Gui::Get()->UpdateVertexBuffer(m_Panel.Index, &m_Panel.Quad[0]);
				Gui::Get()->UpdateVertexBuffer(m_Panel.IndexButton, &m_Panel.Quad[1]);
			}

			counter++;
		}

		if (start)
			m_Widget.insert(m_Widget.begin(),std::pair<std::string,Ref<Widget>>(label,widget));
		else
			m_Widget[label] = widget;

		m_Widget[label]->SetPosition(glm::vec2(newPos.x,newPos.y + m_Widget[label]->GetLabelSize().y));
	}

	bool Wrapper::MouseHoover(const glm::vec2& mousePos)
	{
		if (!m_Locked)
		{
			glm::vec4 coords = getCoords();
			if (mousePos.x >= coords.x && mousePos.x <= coords.z
				&& mousePos.y <= coords.y && mousePos.y >= coords.w)
			{
				if (m_Panel.ButtonClicked(mousePos))
				{
					m_Minimized = !m_Minimized;
					Minimize();
					return false;
				}
				return true;
			}
		}
		return false;
	}
	bool Wrapper::MouseResize(const glm::vec2& mousePos)
	{
		if (!m_Locked && !m_Minimized)
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

	void Wrapper::Minimize()
	{
		if (m_Minimized)
		{
			m_CachedSize = m_Size;
			m_CachedPosition = m_Position;
			m_Size = m_Panel.Size;
			m_Quad.SetPosition(m_Position, m_Panel.Size);
			for (auto& it : m_Widget)
				it.second->Minimize();
		}
		else
		{
			m_Size = m_CachedSize;
			m_Quad.SetPosition(m_Position, m_Size);
			for (auto& it : m_Widget)
				it.second->Move(m_Position-m_CachedPosition);		
		}
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
	}

	void Wrapper::Resize(const glm::vec2& mousePos)
	{
		m_Size = abs(mousePos - m_Position);
		m_Panel.Size.x = m_Size.x;
		m_Panel.SetPosition(m_Panel.Position);
		m_Quad.SetPosition(m_Position, m_Size);

		Gui::Get()->UpdateVertexBuffer(m_Panel.Index, &m_Panel.Quad[0]);
		Gui::Get()->UpdateVertexBuffer(m_Panel.IndexButton, &m_Panel.Quad[1]);
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
	}
	void Wrapper::Move(const glm::vec2& pos)
	{
		m_Position += pos;
		if (!m_Minimized)
		{
			for (auto& it : m_Widget)
			{
				it.second->Move(pos);
			}
		}
		m_Panel.Move(pos);
		Gui::Get()->UpdateVertexBuffer(m_Panel.Index, &m_Panel.Quad[0]);
		Gui::Get()->UpdateVertexBuffer(m_Panel.IndexButton, &m_Panel.Quad[1]);

		m_Quad.SetPosition(m_Position, m_Size);
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);

		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
	}
	void Wrapper::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		if (!m_Minimized)
		{
			for (auto& it : m_Widget)
			{
				it.second->Move(pos);
			}
		}
		m_Panel.SetPosition(m_Position);
		Gui::Get()->UpdateVertexBuffer(m_Panel.Index, &m_Panel.Quad[0]);
		Gui::Get()->UpdateVertexBuffer(m_Panel.IndexButton, &m_Panel.Quad[1]);

		m_Quad.SetPosition(m_Position, m_Size);
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);

		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y));
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
			m_Quad.SetPosition(m_Position, m_Size);
			Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
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

}