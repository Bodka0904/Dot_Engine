#include "stdafx.h"
#include "DGui.h"
#include "Dot/Input.h"

#include <GL/glew.h>


namespace Dot {

	Ref<Shader>		 DGui::m_Shader;
	Ref<Shader>		 DGui::m_TextShader;
	Ref<OrthoCamera> DGui::m_Camera;
	Ref<Texture>	 DGui::m_Texture;
	Ref<ArrayBuffer> DGui::m_VAO;

	std::vector<glm::vec2>  DGui::m_Vertice;
	std::vector <glm::vec2> DGui::m_TexCoord;
	unsigned int			DGui::m_NumWidgets = 0;
	std::string				DGui::m_AttachedWidget;
	std::string				DGui::m_AttachedWrapper;
	std::string				DGui::m_EnabledWrapper;


	std::unordered_map<std::string, Ref<DWidget>>	 DGui::m_Widget;
	std::unordered_map < std::string, Ref<DWrapper>> DGui::m_Wrapper;

	void DGui::Init(const std::string& texturePack, const std::string& shader, const std::string& textShader)
	{
		m_VAO = std::make_shared<ArrayBuffer>();
		m_Texture = std::make_shared<Texture>(texturePack,true);
		m_Shader = std::make_shared<Shader>("GuiShader", shader);
		m_Camera = std::make_shared<OrthoCamera>(0, Input::GetWindowSize().first, Input::GetWindowSize().second, 0);

		m_Shader->AddUniform("u_ViewProjectionMatrix");
		m_Shader->AddUniform("u_MousePos");
		m_Shader->AddUniform("u_Texture");
		m_Shader->UploadUniformInt("u_Texture", 0);

		m_VAO = std::make_shared<ArrayBuffer>();
		Ref<VertexBuffer>m_VBO[2];

		BufferLayout layout = {
				{0,ShaderDataType::Float2,"a_Position"},
		};
		m_VBO[0] = std::make_shared<VertexBuffer>((void*)& m_Vertice[0], m_NumWidgets * sizeof(DQuad), D_DYNAMIC_DRAW);
		m_VBO[0]->SetLayout(layout);
		m_VAO->AddVBO(m_VBO[0]);

		BufferLayout layout_tex = {
				{1,ShaderDataType::Float2,"a_TexCoord"},
		};
		m_VBO[1] = std::make_shared<VertexBuffer>(&m_TexCoord[0], m_NumWidgets * sizeof(glm::vec2) * 4, D_DYNAMIC_DRAW);
		m_VBO[1]->SetLayout(layout_tex);
		m_VAO->AddVBO(m_VBO[1]);

		for (auto& wrap : m_Wrapper)
		{
			wrap.second->SetWidgetPosition();
		}

		m_Vertice.clear();
		m_TexCoord.clear();

	}

	
	bool DGui::HandleLeftClick()
	{
		for (auto& wrap : m_Wrapper)
		{
			for (auto& widg : wrap.second->GetWidgets())
			{

				if (widg.second->MouseHoover(glm::vec2(Input::GetMouseX(), Input::GetMouseY())))
				{
					widg.second->ClickHandle();
					return true;
				}
			}
		}
		for (auto& widg : m_Widget)
		{
			if (widg.second->MouseHoover(glm::vec2(Input::GetMouseX(), Input::GetMouseY())))
			{
				widg.second->ClickHandle();
				return true;
			}
		}
		return false;
	}


	bool DGui::HandleRightClick()
	{
		glm::vec2 mousePos = glm::vec2(Input::GetMouseX(), Input::GetMouseY());
		for (auto& wrap : m_Wrapper)
		{
			if (wrap.second->MouseHoover(mousePos))
			{
				m_AttachedWrapper = wrap.first;
			}
			for (auto& widg : wrap.second->GetWidgets())
			{
				if (widg.second->MouseHoover(mousePos))
				{
					m_AttachedWidget = widg.first;
					m_AttachedWrapper = wrap.first;
					return true;
				}
			}
			
		}
		for (auto& it : m_Widget)
		{
			if (it.second->MouseHoover(mousePos))
			{
				m_AttachedWidget = it.first;
				return true;
			}
		}
		return false;
	}
	void DGui::HandleRelease()
	{
		m_AttachedWidget.clear();
		m_AttachedWrapper.clear();
	}
	void DGui::UpdatePosBuffer(unsigned int index, unsigned int size, const void* vertices)
	{
		m_VAO->GetVertexBuffer(0)->Update(vertices, size, index * sizeof(DQuad));
	}
	void DGui::UpdateTexBuffer(unsigned int index, unsigned int size, const void* texcoords)
	{
		m_VAO->GetVertexBuffer(1)->Update(texcoords, size, index * sizeof(glm::vec2) * 4);
	}
	void DGui::UpdateCamera(const glm::vec2& window)
	{
		m_Camera->SetProjectionMatrix(0, window.x, window.y, 0);
	}
	void DGui::Update()
	{
		if (!m_AttachedWrapper.empty() && !m_AttachedWidget.empty())
		{
			m_Wrapper[m_AttachedWrapper]->GetWidget(m_AttachedWidget).SetPosition(glm::vec2(Input::GetMouseX(), Input::GetMouseY()));
		}
		else if (!m_AttachedWidget.empty())
		{
			m_Widget[m_AttachedWidget]->SetPosition(glm::vec2(Input::GetMouseX(), Input::GetMouseY()));
		}
		else if (!m_AttachedWrapper.empty())
		{
			m_Wrapper[m_AttachedWrapper]->SetPosition(glm::vec2(Input::GetMouseX(), Input::GetMouseY()));
		}
	}
	void DGui::Render()
	{
		m_Shader->Bind();
		m_Shader->UploadUniformFloat2("u_MousePos", glm::vec2(Input::GetMouseX(), Input::GetMouseY()));
		m_Shader->UploadUniformMat4("u_ViewProjectionMatrix", m_Camera->GetViewProjectionMatrix());
		m_Texture->Bind(0);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_VAO->Bind();
		glDrawArrays(GL_QUADS, 0, m_VAO->GetVertexBuffer(0)->GetCount());

		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_COLOR, GL_SRC_COLOR);
		glEnable(GL_DEPTH_TEST);
	}
	void DGui::AddWidget(const std::string& label, Ref<DWidget> widget, const DQuad& quad, glm::vec2* texcoord, int num)
	{
		if (!m_EnabledWrapper.empty())
		{
			m_Wrapper[m_EnabledWrapper]->AddWidget(label,widget,m_NumWidgets);
		}
		else
		{
			m_Widget[label] = widget;
			m_Widget[label]->SetIndex(m_NumWidgets);
		}
		for (int i = 0; i < num; ++i)
		{
			m_Vertice.push_back(quad.m_Vertices[i]);
			m_TexCoord.push_back(texcoord[i]);
		}
		m_NumWidgets++;
	}
	void DGui::AddWrapper(const std::string label, Ref<DWrapper> wrapper, const DQuad& quad, glm::vec2* texcoord, int num)
	{
		wrapper->SetIndex(m_NumWidgets);
		m_Wrapper[label] = wrapper;
		for (int i = 0; i < num; ++i)
		{
			m_Vertice.push_back(quad.m_Vertices[i]);
			m_TexCoord.push_back(texcoord[i]);
		}
		m_NumWidgets++;
	}
	void DGui::EnableWrapper(const std::string& label)
	{
		m_EnabledWrapper = label;
	}
	void DGui::DisableWrapper()
	{
		m_EnabledWrapper.clear();
	}
	DWrapper::DWrapper(const std::string& label, const glm::vec2& position, const glm::vec2& size)
		:
		m_Position(position),
		m_Size(size),
		m_Index(0)
	{
	}
	void DWrapper::AddWidget(const std::string& label, Ref<DWidget> widget, unsigned int index)
	{
		m_Widget[label] = widget;
		m_Widget[label]->SetIndex(index);
	}
	bool DWrapper::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = GetCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			return true;
		}
		return false;
	}
	void DWrapper::SetPosition(const glm::vec2& pos)
	{
		for (auto& it : m_Widget)
		{
			glm::vec2 posDif = it.second->GetPosition() - m_Position;
			it.second->SetPosition(pos + posDif);
		}
		m_Position = pos;

		glm::vec2 newPos[4] = {
			glm::vec2(m_Position),
			glm::vec2(m_Position.x + m_Size.x,m_Position.y),
			glm::vec2(m_Position + m_Size),
			glm::vec2(m_Position.x,m_Position.y + m_Size.y)
		};
		DGui::UpdatePosBuffer(m_Index, sizeof(glm::vec2) * 4, (void*)& newPos[0]);
	}
	void DWrapper::SetWidgetPosition()
	{
		for (auto& it : m_Widget)
		{
			it.second->SetPosition(m_Position);
		}
	}
	void DWrapper::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size)
	{
		glm::vec2 texCoords[4] = {
			   glm::vec2(0, 0.75),
			   glm::vec2(0.5, 0.75),
			   glm::vec2(0.5, 1),
			   glm::vec2(0, 1)
		};
		DQuad quad(position, size);
		Ref<DWrapper> button = std::make_shared<DWrapper>(label, position, size);
		DGui::AddWrapper(label, button, quad, &texCoords[0]);
	}
	glm::vec4 DWrapper::GetCoords()
	{
		return glm::vec4(m_Position.x,
			m_Position.y + m_Size.y,
			m_Position.x + m_Size.x,
			m_Position.y);
	}
}