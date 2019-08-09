#include "GuiWrapper.h"
#include "widgets/GuiButton.h"
#include "widgets/GuiArrowButton.h"
#include "widgets/GuiSlider.h"
#include "widgets/GuiCheckBox.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

float Wrapper::WRAPPER_SIZE_X = 400.0f;
float Wrapper::WRAPPER_SIZE_Y = 400.0f;


GuiWrapper::GuiWrapper(const std::string& name)
	: m_text(new GuiText(name, glm::vec2(0.0f, -15.0f)))
{
	std::shared_ptr<GuiVertexBuffer> m_VBO;
	std::shared_ptr<GuiIndexBuffer> m_IBO;

	m_VAO.reset(new GuiArrayBuffer());


	Wrapper m_data;
	GuiBufferLayout layout = {
		{0,GuiShaderDataType::Float2, "position" },
		{1,GuiShaderDataType::Float2, "texCoords" }
	};

	m_VBO.reset(new GuiVertexBuffer(&m_data.m_vertices[0], 4 * sizeof(GuiVertex)));
	m_VBO->SetLayout(layout);
	m_VAO->AddVBO(m_VBO);

	m_IBO.reset(new GuiIndexBuffer((unsigned int*)& m_data.indices[0], 6));
	m_VAO->AddIBO(m_IBO);
}


GuiWrapper::~GuiWrapper()
{
	delete m_text;
}


void GuiWrapper::Draw(GuiShader & shader, GuiTransform & transform)
{


	UpdateData(transform);
	shader.Update(transform);
	shader.UpdateColor(glm::vec3(m_color, m_color, m_color));
	shader.UpdateTexOffset(glm::vec2(0,0));
	if (m_pinned)
	{
		shader.UpdateTexOffset(glm::vec2(0, 0.1));
	}
	m_VAO->Bind();

	glDrawElements(GL_TRIANGLES, m_VAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void GuiWrapper::DrawButtons(GuiShader & shader, GuiTransform & transform)
{
	for (unsigned int i = 0; i < num_buttons; ++i)
	{
		m_widgets[i]->Draw(shader, transform);
	}
}



void GuiWrapper::DrawArrButtons(GuiShader & shader, GuiTransform & transform)
{
	for (unsigned int i = GetArrButtonsIndexed().x; i < GetArrButtonsIndexed().y; ++i)
	{
		m_widgets[i]->Draw(shader, transform);
	}
}

void GuiWrapper::DrawCheckBoxes(GuiShader & shader, GuiTransform & transform)
{
	for (unsigned int i = GetCheckBoxesIndexed().x; i < GetCheckBoxesIndexed().y; ++i)
	{
		m_widgets[i]->Draw(shader, transform);
	}
}

void GuiWrapper::DrawSliders(GuiShader & shader, GuiTransform & transform)
{
	for (int i = GetSlidersIndexed().x; i < GetSlidersIndexed().y; ++i)
	{
		m_widgets[i]->Draw(shader, transform);
	}
}

void GuiWrapper::DrawTexts(GuiTransform& transform)
{
	GuiText::StartDraw();

	for (int i = 0; i < m_widgets.size(); ++i)
	{
		m_widgets[i]->GetText()->UpdateData(transform);
		m_widgets[i]->GetText()->Draw();
	}
	m_text->UpdateData(transform);
	m_text->Draw();
}


void GuiWrapper::HandleButtonCallback(int i)
{
	if (i < num_buttons + num_arrow_btns)
	{
		m_user_callbacks[i]();
		m_widgets[i]->SetValue(0.5f);
		m_widgets[i]->Clicked() = false;
		
	}
}

void GuiWrapper::HandleCheckBoxCallbacks()
{
	for (int i = GetCheckBoxesIndexed().x; i < GetCheckBoxesIndexed().y; ++i)
	{
		if (m_widgets[i]->Clicked())
		{
			m_user_callbacks[i]();
		}
	}
}

void GuiWrapper::HandleSlider(int i,float xpos)
{
	if (i >= GetSlidersIndexed().x && i < GetSlidersIndexed().y)
	{
			
		m_widgets[i]->SetValue(xpos);
		m_widgets[i]->Clicked() = false;
		
	}
}

void GuiWrapper::HandleReleaseButton(GuiEvent & event,float xpos,float ypos)
{
	for (unsigned int i = 0; i < num_buttons + num_arrow_btns; ++i)
	{
		if (m_widgets[i]->MouseHoover(glm::vec2(xpos, ypos)))
		{
			GuiMouseButtonReleaseEvent& e = (GuiMouseButtonReleaseEvent&)event;
			if (e.GetButton() == GLFW_MOUSE_BUTTON_LEFT)
			{
				m_widgets[i]->SetValue(0.0f);
				m_widgets[i]->Clicked() = false;
			
			}
			break;
		}
	}
}


void GuiWrapper::UpdateData(GuiTransform & transform)
{
	transform.SetScale(m_scale);
	transform.SetPos(glm::vec2(m_position.x,
		m_position.y));
}

void GuiWrapper::SetData(glm::vec2& pos, glm::vec2& scale)
{
	
		m_scale = scale;
		m_position = glm::vec2(pos.x - Wrapper::WRAPPER_SIZE_X / 2 * m_scale.x, pos.y - Wrapper::WRAPPER_SIZE_Y / 2 * m_scale.y);
		m_text->SetData(m_position);
	
}

void GuiWrapper::SetWidgetsFollow()
{
		for (int i = 0; i < m_widgets.size(); ++i)
		{
			m_widgets[i]->SetData(GetCenter());
		}
	
}

void GuiWrapper::SetWidget(int index,const glm::vec2& position)
{
	m_widgets[index]->SetData(position);
}

void GuiWrapper::SetWidgetsNextTo()
{
	float xoffset = 0.0f;
	float yoffset = 20.0f;

	
		for (int i = 0; i < num_buttons; ++i)
		{

			glm::vec2 position = glm::vec2(m_position.x + xoffset + (m_widgets[i]->GetSize().x / 2), m_position.y + yoffset + (m_widgets[i]->GetSize().y / 2));
			m_widgets[i]->SetData(position);

			yoffset += (m_widgets[i]->GetSize().y + 20);
			if (yoffset >= (Wrapper::WRAPPER_SIZE_Y * m_scale.y) - m_widgets[i]->GetSize().y)
			{
				xoffset += (m_widgets[i]->GetSize().x + 20);
				yoffset = 20.0f;
			};
		}

		for (int i = num_buttons; i < num_buttons + num_arrow_btns; ++i)
		{
			glm::vec2 position = glm::vec2(m_position.x + xoffset + (m_widgets[i]->GetSize().x / 2), m_position.y + yoffset + (m_widgets[i]->GetSize().y / 2));
			m_widgets[i]->SetData(position);

			yoffset += (m_widgets[i]->GetSize().y + 20);
			if (yoffset >= (Wrapper::WRAPPER_SIZE_Y * m_scale.y) - m_widgets[i]->GetSize().y)
			{
				xoffset += (m_widgets[i]->GetSize().x + 20);
				yoffset = 20.0f;
			};


		}

		for (int i = GetCheckBoxesIndexed().x; i < GetCheckBoxesIndexed().y; ++i)
		{
			glm::vec2 position = glm::vec2(m_position.x + xoffset + (m_widgets[i]->GetSize().x / 2), m_position.y + yoffset + (m_widgets[i]->GetSize().y / 2));
			m_widgets[i]->SetData(position);

			yoffset += (m_widgets[i]->GetSize().y + 20);
			if (yoffset >= (Wrapper::WRAPPER_SIZE_Y * m_scale.y) - m_widgets[i]->GetSize().y)
			{
				xoffset += (m_widgets[i]->GetSize().x + 20);
				yoffset = 20.0f;
			};

		}

		for (int i = GetSlidersIndexed().x; i < GetSlidersIndexed().y; ++i)
		{
			glm::vec2 position = glm::vec2(m_position.x + xoffset + (m_widgets[i]->GetSize().x / 2), m_position.y + yoffset + (m_widgets[i]->GetSize().y / 2));
			m_widgets[i]->SetData(position);
			yoffset += (m_widgets[i]->GetSize().y + 20);
			if (yoffset >= (Wrapper::WRAPPER_SIZE_Y * m_scale.y) - m_widgets[i]->GetSize().y)
			{
				xoffset += (m_widgets[i]->GetSize().x + 20);
				yoffset = 20.0f;
			};


		}
	

}

bool GuiWrapper::PinToSide(glm::vec2& winSize)
{
	
	if (m_position.x > winSize.x - (Wrapper::WRAPPER_SIZE_X*m_scale.x))
	{
		m_pinned = true;
		m_scale.x = 0.5;
		m_scale.y = winSize.y / (Wrapper::WRAPPER_SIZE_Y * m_scale.y);

		m_position.x = winSize.x - (Wrapper::WRAPPER_SIZE_X*m_scale.x);
		m_position.y = 0;
		m_text->SetData( glm::vec2(m_position.x+20, 20));
		return true;
	}
	else if (m_position.x <  0)
	{
		
		m_pinned = true;
		m_scale.x = 0.5;
		m_scale.y = winSize.y / (Wrapper::WRAPPER_SIZE_Y * m_scale.y);

		m_position.x = 0;
		m_position.y = 0;
		m_text->SetData(m_position + glm::vec2(20, 20));
		return true;
	}
	else if (m_position.y < 0)
	{	
		m_text->SetData(glm::vec2(winSize.x / 2 - 20, 20));
		m_pinned = true;
		m_scale.y = 0.5;
		m_scale.x = (winSize.x - Wrapper::WRAPPER_SIZE_X) / (Wrapper::WRAPPER_SIZE_X * m_scale.x);

		m_position.x = Wrapper::WRAPPER_SIZE_X * 0.5;
		m_position.y = 0;
		return true;

	}
	else if (m_position.y > winSize.y - (Wrapper::WRAPPER_SIZE_Y * m_scale.y))
	{	
		m_text->SetData( glm::vec2(winSize.x/2-20,winSize.y-20));
		m_pinned = true;
		m_scale.y = 0.5;
		m_scale.x = (winSize.x - Wrapper::WRAPPER_SIZE_X) / (Wrapper::WRAPPER_SIZE_X * m_scale.x);

		m_position.x = Wrapper::WRAPPER_SIZE_X * 0.5;
		m_position.y = winSize.y - (Wrapper::WRAPPER_SIZE_Y*m_scale.y);
		return true;
	}
	else 
	{
		return false;
	}
	
}



bool GuiWrapper::MouseHoover(glm::vec2& mousePos)
{
	if (mousePos.x >= GetCoords().x && mousePos.x <= GetCoords().z
		&& mousePos.y <= GetCoords().y && mousePos.y >= GetCoords().w)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

glm::vec2 GuiWrapper::GetCenter() const
{
	return glm::vec2(m_position.x + (Wrapper::WRAPPER_SIZE_X/2 * m_scale.x), m_position.y + (Wrapper::WRAPPER_SIZE_Y/2 * m_scale.y));
}

glm::vec2 GuiWrapper::GetPosition() const
{
	return glm::vec2(m_position.x , m_position.y);

}

void GuiWrapper::AddButton(func_ptr func, const std::string & name, const glm::vec2& size)
{
	m_user_callbacks.emplace(m_user_callbacks.begin() + num_buttons, func);
	m_widgets.emplace(m_widgets.begin() + num_buttons, new GuiButton(name, size));
	num_buttons++;

}

void GuiWrapper::AddArrowButton(func_ptr func, const std::string & name, const glm::vec2& size)
{
	m_user_callbacks.emplace(m_user_callbacks.begin() + num_arrow_btns + num_buttons, func);
	m_widgets.emplace(m_widgets.begin() +num_arrow_btns + num_buttons, new GuiArrowButton(name, size));
	num_arrow_btns++;
	
}

void GuiWrapper::AddCheckBox(func_ptr func, const std::string & name, const glm::vec2& size)
{
	m_user_callbacks.emplace(m_user_callbacks.begin() + num_buttons + num_arrow_btns + num_checkboxes, func);
	m_widgets.emplace(m_widgets.begin() + num_buttons + num_arrow_btns + num_checkboxes, new GuiCheckBox(name, size));
	num_checkboxes++;
	
}

void GuiWrapper::AddSlider(const std::string & name, const glm::vec2& size)
{
	m_widgets.emplace(m_widgets.begin() + num_buttons + num_checkboxes + num_arrow_btns + num_sliders, new GuiSlider(name, size));
	num_sliders++;
	
}


glm::vec4 GuiWrapper::GetCoords()
{
	return glm::vec4(
		m_position.x,
		m_position.y + (Wrapper::WRAPPER_SIZE_Y*m_scale.y),
		m_position.x + (Wrapper::WRAPPER_SIZE_X*m_scale.x),
		m_position.y);
}

glm::vec2 GuiWrapper::GetButtonsIndexed() const
{
	return glm::vec2(0,num_buttons);
}

glm::vec2 GuiWrapper::GetArrButtonsIndexed() const
{
	return glm::vec2(num_buttons, m_widgets.size() - num_sliders - num_checkboxes);
}

glm::vec2 GuiWrapper::GetCheckBoxesIndexed() const
{
	return glm::vec2(num_buttons + num_arrow_btns, m_widgets.size() - num_sliders);
}

glm::vec2 GuiWrapper::GetSlidersIndexed() const
{
	return glm::vec2(num_checkboxes + num_buttons + num_arrow_btns, m_widgets.size());
}

GuiWidget & GuiWrapper::GetButton(int index)
{
	if (index > num_buttons - 1)
	{
		index = num_buttons - 1;
	}
	return *m_widgets[index];
}

GuiWidget & GuiWrapper::GetArrowButton(int index)
{
	if (index > num_arrow_btns + num_buttons - 1)
	{
		index = num_arrow_btns + num_buttons - 1;
	}
	return *m_widgets[num_buttons + index];
}

GuiWidget & GuiWrapper::GetCheckBox(int index)
{
	if (index > num_buttons + num_arrow_btns + num_checkboxes - 1)
	{
		index = num_buttons + num_arrow_btns + num_checkboxes - 1;
	}
	return *m_widgets[num_buttons + num_arrow_btns + index];
}

GuiWidget & GuiWrapper::GetSlider(int index)
{
	if (index > num_buttons + num_arrow_btns + num_checkboxes + num_sliders - 1)
	{
		index = num_buttons + num_arrow_btns + num_checkboxes + num_sliders - 1;
	}
	return *m_widgets[num_buttons + num_arrow_btns + num_checkboxes + index];
}
