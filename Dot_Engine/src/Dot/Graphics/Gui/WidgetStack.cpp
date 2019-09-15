#include "stdafx.h"
#include "WidgetStack.h"
#include "Dot/Input.h"
#include "Dot/Application.h"

#include <GL/glew.h>

namespace Dot {
	std::unordered_map<std::string, Ref<Widget>> WidgetStack::m_Widget;
	std::unordered_map<std::string, Ref<Wrapper>> WidgetStack::m_Wrapper;
	std::string WidgetStack::m_EnabledWrapper = "";

	WidgetStack::WidgetStack(const std::string& widgetShader, const std::string& textShader, const std::string& texturePack)
	{
		
		m_Texture = std::make_shared<Texture>(texturePack);
		m_Camera = std::make_shared<OrthoCamera>(0, Application::Get().GetWin().GetWidth(), Application::Get().GetWin().GetHeight(), 0);

		m_Shader = std::make_shared<Shader>("GuiShader", widgetShader);
		m_Shader->AddUniform("u_ViewProjectionMatrix");
		m_Shader->AddUniform("u_Position");
		m_Shader->AddUniform("u_MousePos");
		m_Shader->AddUniform("u_TexOffset");
		m_Shader->AddUniform("u_Texture");
		m_Shader->UploadUniformInt("u_Texture", 0);

		m_TextShader = std::make_shared<Shader>("TextShader", textShader);
		m_TextShader->AddUniform("u_ViewProjectionMatrix");
		m_TextShader->AddUniform("u_Position");
	}

	void WidgetStack::AddWidget(const std::string& label, Ref<Widget> widget)
	{
		if (m_EnabledWrapper != "")
		{
			m_Wrapper[m_EnabledWrapper]->AddWidget(label, widget);
		}
		else
		{
			m_Widget[label] = widget;
		}
	}

	void WidgetStack::AddWrapper(const std::string& label, Ref<Wrapper> wrapper)
	{
		m_Wrapper[label] = wrapper;
	}
	
	bool WidgetStack::HandleLeftClick()
	{
		for (auto& it : m_Widget)
		{
			if (it.second->MouseHoover(glm::vec2(Input::GetMouseX(), Input::GetMouseY())))
			{
				it.second->ClickHandle();
				return true;
			}
		}
		return false;
	}
	bool WidgetStack::HandleRightClick()
	{
		for (auto& it : m_Wrapper)
		{
			for (auto & i : it.second->GetWidgets())
			{
				if (i.second->MouseHoover(glm::vec2(Input::GetMouseX(), Input::GetMouseY())))
				{
					m_SelectedWidget = i.first;
					return true;
				}
			}
			if (it.second->MouseHoover(glm::vec2(Input::GetMouseX(), Input::GetMouseY())))
			{
				m_SelectedWidget = it.first;
				return true;
			}
		}
		for (auto& it : m_Widget)
		{
			if (it.second->MouseHoover(glm::vec2(Input::GetMouseX(), Input::GetMouseY())))
			{
				m_SelectedWidget = it.first;
				return true;
			}
		}
		return false;	
	}
	void WidgetStack::Release()
	{
		m_SelectedWidget = "";
	}
	void WidgetStack::Update()
	{
		for (auto& it : m_Wrapper)
		{
			if (it.second->GetWidgets().find(m_SelectedWidget) != it.second->GetWidgets().end())
			{
				it.second->GetWidgets()[m_SelectedWidget]->SetPosition(glm::vec2(Input::GetMouseX(), Input::GetMouseY()));
			}
		}
		if (m_Widget.find(m_SelectedWidget) != m_Widget.end())
		{
			m_Widget[m_SelectedWidget]->SetPosition(glm::vec2(Input::GetMouseX(), Input::GetMouseY()));
		}
		else if (m_Wrapper.find(m_SelectedWidget) != m_Wrapper.end())
		{
			m_Wrapper[m_SelectedWidget]->SetPosition(glm::vec2(Input::GetMouseX(), Input::GetMouseY()));
		}
		
	
	}
	void WidgetStack::RenderWidgets()
	{
	
		m_Shader->Bind();
		m_Shader->UploadUniformFloat2("u_MousePos", glm::vec2(Input::GetMouseX(), Input::GetMouseY()));
		m_Shader->UploadUniformMat4("u_ViewProjectionMatrix", m_Camera->GetViewProjectionMatrix());
		m_Texture->Bind(0);
		glEnable(GL_BLEND);
		for (auto& it : m_Widget) 
		{
			m_Shader->UploadUniformFloat2("u_Position", it.second->GetPosition());
			it.second->Render(m_Shader);		
		}
		for (auto& it : m_Wrapper)
		{
			it.second->Render(m_Shader);
		}
		glDisable(GL_BLEND);
	}
	void WidgetStack::RenderLabels()
	{
		m_TextShader->Bind();
		m_TextShader->UploadUniformMat4("u_ViewProjectionMatrix", m_Camera->GetViewProjectionMatrix());
		for (auto& it : m_Widget)
		{		
			m_TextShader->UploadUniformFloat2("u_Position", it.second->GetPosition());
			it.second->RenderLabel();
		}
		for (auto& it : m_Wrapper)
		{
			it.second->RenderLabels(m_TextShader);
	
		}
	}
	void WidgetStack::EnableWrapper(const std::string& wrapper)
	{
		m_EnabledWrapper = wrapper;	
	}
	void WidgetStack::DisableWrapper()
	{
		m_EnabledWrapper = "";
	}
	Wrapper::Wrapper(const std::string& label, const glm::vec2& position, const glm::vec2& size)
		:
		m_Size(size)
	{
		m_Transform.SetPos(position);
		glm::vec2 texcoords[4] = {
			glm::vec2(0,0.75),
			glm::vec2(0.5,0.75),
			glm::vec2(0.5,1),
			glm::vec2(0,1)
		};
		Quad quad(glm::vec2(0, 0), size, &texcoords[0]);

		m_VAO = std::make_shared<ArrayBuffer>();

		BufferLayout layout = {
			{0,ShaderDataType::Float2,"a_Position"},
			{1,ShaderDataType::Float2,"a_TexCoord"},

		};
		Ref<VertexBuffer> VBO = std::make_shared<VertexBuffer>(&quad.m_Vertices[0], 4 * sizeof(Vertex), D_STATIC_DRAW);
		VBO->SetLayout(layout);

		m_VAO->AddVBO(VBO);


		Ref<IndexBuffer> IBO = std::make_shared<IndexBuffer>((void*)& quad.m_Indices[0], 6);
		m_VAO->AddIBO(IBO);
		
		int sizeConstantDiv = 7;
		float letterSize = size.x / label.length() / sizeConstantDiv;

		if (letterSize >= 40)
		{
			letterSize = 40;
		}
		else if (letterSize <= 15)
		{
			letterSize = 15;
		}
		float offsetx = size.x - (label.length() * letterSize);
		m_Label = std::make_shared<Text>(label, offsetx / 2, -size.y / sizeConstantDiv, letterSize, size.y / sizeConstantDiv);

	}
	Wrapper::~Wrapper()
	{
	}
	void Wrapper::Render(const Ref<Shader>& shader)
	{
		for (auto& it : m_Widget)
		{
			shader->UploadUniformFloat2("u_Position", it.second->GetPosition());
			it.second->Render(shader);
		}		
		shader->UploadUniformFloat2("u_Position", m_Transform.GetPos());
		shader->UploadUniformFloat("u_TexOffset", 0);
		
		m_VAO->Bind();
		glDrawElements(GL_TRIANGLES, m_VAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

	}
	void Wrapper::RenderLabels(const Ref<Shader>& shader)
	{	
		for (auto& it : m_Widget)
		{
			shader->UploadUniformFloat2("u_Position", it.second->GetPosition());
			it.second->RenderLabel();
		}
		shader->UploadUniformFloat2("u_Position", m_Transform.GetPos());
		m_Label->PrintText("ArialBolt");
	}
	void Wrapper::SetPosition(const glm::vec2& pos)
	{	
		for (auto& it : m_Widget)
		{
			glm::vec2 posDif = it.second->GetPosition() - m_Transform.GetPos();
			it.second->SetPosition(pos + posDif);
		}
		m_Transform.GetPos() = pos;
		m_Transform.UpdateModel();
	}
	void Wrapper::ClickHandle()
	{
		
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
	void Wrapper::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size)
	{
		Ref<Wrapper> checkbox;
		checkbox = std::make_shared<Wrapper>(label, position, size);

		WidgetStack::AddWrapper(label, checkbox);
	}
	glm::vec4 Wrapper::GetCoords()
	{
		return glm::vec4(m_Transform.GetPos().x,
			m_Transform.GetPos().y + m_Size.y,
			m_Transform.GetPos().x + m_Size.x,
			m_Transform.GetPos().y);
	}
}