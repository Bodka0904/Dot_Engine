#include "stdafx.h"
#include "WidgetStack.h"
#include "Dot/Input.h"
#include "Dot/Application.h"

#include <GL/glew.h>

namespace Dot {
	std::unordered_map<std::string, Ref<Widget>> WidgetStack::m_Widget;
	std::unordered_map<std::string, Ref<Wrapper>> WidgetStack::m_Wrapper;
	Ref<ArrayBuffer>WidgetStack::m_VAO_Widget;
	unsigned int WidgetStack::m_NumWidgets = 0;
	std::vector<glm::vec2> WidgetStack::m_Vertices;
	std::vector<glm::vec2> WidgetStack::m_TexCoords;



	std::string WidgetStack::m_EnabledWrapper = "";

	WidgetStack::WidgetStack(const std::string& widgetShader, const std::string& textShader, const std::string& texturePack)
	{
		
		m_Texture = std::make_shared<Texture>(texturePack);
		m_Camera = std::make_shared<OrthoCamera>(0, Application::Get().GetWin().GetWidth(), Application::Get().GetWin().GetHeight(), 0);

		m_Shader = std::make_shared<Shader>("GuiShader", widgetShader);
		m_Shader->AddUniform("u_ViewProjectionMatrix");
		m_Shader->AddUniform("u_MousePos");
		m_Shader->AddUniform("u_Texture");
		m_Shader->UploadUniformInt("u_Texture", 0);

		m_TextShader = std::make_shared<Shader>("TextShader", textShader);
		m_TextShader->AddUniform("u_ViewProjectionMatrix");
		m_TextShader->AddUniform("u_Position");


		m_VAO_Widget = std::make_shared<ArrayBuffer>();

		Ref<VertexBuffer>m_VBO[2];
		
		BufferLayout layout = {
				{0,ShaderDataType::Float2,"a_Position"},
		};
		m_VBO[0] = std::make_shared<VertexBuffer>((void*)&m_Vertices[0], m_NumWidgets*sizeof(Quad), D_DYNAMIC_DRAW);
		m_VBO[0]->SetLayout(layout);	
		m_VAO_Widget->AddVBO(m_VBO[0]);

		BufferLayout layout_tex = {
				{1,ShaderDataType::Float2,"a_TexCoord"},
		};
		m_VBO[1] = std::make_shared<VertexBuffer>(&m_TexCoords[0], m_NumWidgets * sizeof(glm::vec2)*4, D_DYNAMIC_DRAW);
		m_VBO[1]->SetLayout(layout_tex);
		m_VAO_Widget->AddVBO(m_VBO[1]);


		for (auto& it : m_Widget)
		{
			it.second->SetPosition(it.second->GetPosition());
		}
		for (auto& it : m_Wrapper)
		{
			it.second->SetPosition(it.second->GetPosition());
			it.second->SetWidgetPosition();
		}

		m_Vertices.clear();
		m_TexCoords.clear();
	}

	void WidgetStack::AddWidget(const std::string& label, Ref<Widget> widget, const Quad& quad, const glm::vec2* texcoords)
	{
		unsigned int numVertices = sizeof(quad.m_Vertices) / sizeof(glm::vec2);
		if (m_EnabledWrapper != "")
		{		
			m_Wrapper[m_EnabledWrapper]->AddWidget(label, widget, m_NumWidgets);

			for (int i = 0; i < numVertices; ++i)
			{
				m_Vertices.insert(m_Vertices.begin() + (numVertices * m_NumWidgets), quad.m_Vertices[i]);
				m_TexCoords.insert(m_TexCoords.begin() + (numVertices * m_NumWidgets), texcoords[i]);

			}
			m_NumWidgets++;
		}
		else
		{
			m_Widget[label] = widget;
			m_Widget[label]->SetIndex(m_NumWidgets);
			for (int i = 0; i < numVertices; ++i)
			{
				m_Vertices.insert(m_Vertices.begin() + (numVertices * m_NumWidgets), quad.m_Vertices[i]);
				m_TexCoords.insert(m_TexCoords.begin() + (numVertices * m_NumWidgets), texcoords[i]);
			}
			m_NumWidgets++;
		}
		
	}

	void WidgetStack::AddWrapper(const std::string& label, Ref<Wrapper> wrapper, const Quad& quad, const glm::vec2* texcoords)
	{
		m_Wrapper[label] = wrapper;
		m_Wrapper[label]->SetIndex(m_NumWidgets);

		for (int i = 0; i < sizeof(quad.m_Vertices) / sizeof(glm::vec2); ++i)
		{
			m_Vertices.push_back(quad.m_Vertices[i]);
			m_TexCoords.push_back(texcoords[i]);
		}
		m_NumWidgets++;
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
		for (auto& it : m_Wrapper)
		{

			if (it.second->HandleLeftClick())
			{		
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
		
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		for (auto& it : m_Widget) 
		{
			it.second->Update(m_Shader);		
		}
		for (auto& it : m_Wrapper)
		{
			it.second->Update(m_Shader);
		}
		m_VAO_Widget->Bind();
		glDrawArrays(GL_QUADS, 0, m_VAO_Widget->GetVertexBuffer(0)->GetCount());

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
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
	void WidgetStack::UpdatePosBuffer(unsigned int index, unsigned int size, const void* vertices)
	{
		m_VAO_Widget->GetVertexBuffer(0)->Update(vertices, size, index * sizeof(Quad));
	}
	void WidgetStack::UpdateTexBuffer(unsigned int index, unsigned int size, const void* texcoords)
	{
		m_VAO_Widget->GetVertexBuffer(1)->Update(texcoords, size, index * sizeof(glm::vec2)*4);
	}
	Wrapper::Wrapper(const std::string& label, const glm::vec2& position, const glm::vec2& size)
		:
		m_Size(size)
	{
		m_Transform.SetPos(position);
		
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
	void Wrapper::Update(const Ref<Shader>& shader)
	{
		for (auto& it : m_Widget)
		{
			it.second->Update(shader);
		}		
		shader->UploadUniformFloat("u_TexOffset", 0);
		
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

		glm::vec2 newPos[4] = {
			glm::vec2(m_Transform.GetPos()),
			glm::vec2(m_Transform.GetPos().x + m_Size.x,m_Transform.GetPos().y),
			glm::vec2(m_Transform.GetPos() + m_Size),
			glm::vec2(m_Transform.GetPos().x,m_Transform.GetPos().y + m_Size.y)
		};
		WidgetStack::UpdatePosBuffer(m_Index, sizeof(glm::vec2) * 4, (void*)& newPos[0]);
	}
	void Wrapper::SetWidgetPosition()
	{
		for (auto& it : m_Widget)
		{
			it.second->SetPosition(m_Transform.GetPos());
		}
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
	bool Wrapper::HandleLeftClick()
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
	void Wrapper::AddWidget(const std::string& label, Ref<Widget> widget, unsigned int index)
	{ 
		m_Widget[label] = widget;
		m_Widget[label]->SetIndex(index);
	
	}
	void Wrapper::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size)
	{
		glm::vec2 texcoords[4] = {
			glm::vec2(0,0.75),
			glm::vec2(0.5,0.75),
			glm::vec2(0.5,1),
			glm::vec2(0,1)
		};
		Quad quad(glm::vec2(0, 0), size);

		Ref<Wrapper> wrapper;
		wrapper = std::make_shared<Wrapper>(label, position, size);

		WidgetStack::AddWrapper(label, wrapper,quad,&texcoords[0]);
	}
	glm::vec4 Wrapper::GetCoords()
	{
		return glm::vec4(m_Transform.GetPos().x,
			m_Transform.GetPos().y + m_Size.y,
			m_Transform.GetPos().x + m_Size.x,
			m_Transform.GetPos().y);
	}
}