#include "stdafx.h"
#include "Button.h"
#include "WidgetStack.h"

#include <GL/glew.h>


namespace Dot {

	Button::Button(const std::string& label, const glm::vec2& position, const glm::vec2& size)
		:	
		m_Size(size)
	{
		m_Transform.SetPos(position);

		glm::vec2 texcoords[4] = {
			glm::vec2(0,0),
			glm::vec2(0.5,0),
			glm::vec2(0.5,0.25),
			glm::vec2(0,0.25)
		};
		Quad quad(glm::vec2(0,0), size, &texcoords[0]);

		m_VAO = std::make_shared<ArrayBuffer>();

		BufferLayout layout = {
			{0,ShaderDataType::Float2,"a_Position"},
			{1,ShaderDataType::Float2,"a_TexCoord"},
			
		};
		Ref<VertexBuffer> VBO = std::make_shared<VertexBuffer>(&quad.m_Vertices[0],4*sizeof(Vertex),D_STATIC_DRAW);
		VBO->SetLayout(layout);
		
		m_VAO->AddVBO(VBO);

		Ref<IndexBuffer> IBO = std::make_shared<IndexBuffer>((void*)& quad.m_Indices[0], 6);
		m_VAO->AddIBO(IBO);

		float letterSize = size.x / label.length();
		if (letterSize >= 40)
		{
			letterSize = 40;
		}
		else if (letterSize <= 15)
		{
			letterSize = 15;
		}
		float offsetx = size.x - (label.length() * letterSize);
		m_Label = std::make_shared<Text>(label, offsetx/2, size.y/4, letterSize, size.y/2);

	}

	Button::~Button()
	{
	}
	
	

	void Button::Render(const Ref<Shader>& shader)
	{
		shader->UploadUniformFloat("u_TexOffset", 0);
		m_VAO->Bind();
		glDrawElements(GL_TRIANGLES, m_VAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);	
		m_Clicked = false;
	}

	void Button::RenderLabel()
	{
		m_Label->PrintText("ArialBolt");
	}

	void Button::SetPosition(const glm::vec2& pos)
	{
		m_Transform.SetPos(pos);
		m_Transform.UpdateModel();
	}

	

	bool Button::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = GetCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			return true;
		}
		return false;
		
	}

	void Button::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size)
	{
		Ref<Button> button;
		button = std::make_shared<Button>(label, position, size);

		WidgetStack::AddWidget(label,button);
	}

	glm::vec4 Button::GetCoords()
	{	
		return glm::vec4(m_Transform.GetPos().x,
			m_Transform.GetPos().y + m_Size.y,
			m_Transform.GetPos().x + m_Size.x,
			m_Transform.GetPos().y);
		
	}

	
}