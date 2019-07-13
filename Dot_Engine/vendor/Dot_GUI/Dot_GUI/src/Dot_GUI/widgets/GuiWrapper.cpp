#include "GuiWrapper.h"
#include "Dot_GUI/widgets/GuiButton.h"
#include "Dot_GUI/widgets/GuiArrowButton.h"
#include "Dot_GUI/widgets/GuiSlider.h"
#include "Dot_GUI/widgets/GuiCheckBox.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

float Wrapper::WRAPPER_SIZE_X = 400.0f;
float Wrapper::WRAPPER_SIZE_Y = 400.0f;


GuiWrapper::GuiWrapper(const std::string& name)
	: m_text(new GuiText(name, glm::vec2(0, -15)))
{
}


GuiWrapper::~GuiWrapper()
{
	glDeleteVertexArrays(1, &m_vao);
	delete m_text;
}

void GuiWrapper::Init(unsigned int & VBO, unsigned int & IBO)
{
	glGenVertexArrays(1, &m_vao);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (const void*)8);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glBindVertexArray(0);
}

void GuiWrapper::InitButtons(unsigned int & VBO, unsigned int & IBO)
{
	for (unsigned int i = 0; i < num_buttons; ++i)
	{
		m_widgets[i]->Init(VBO, IBO);
		m_widgets[i]->GetText()->SetData(glm::vec2(0, 0));
		m_widgets[i]->SetData(glm::vec2(1, 1));
	}
}

void GuiWrapper::InitArrButtons(unsigned int & VBO, unsigned int & IBO)
{
	for (unsigned int i = GetArrButtonsIndexed().x; i < GetArrButtonsIndexed().y; ++i)
	{
		m_widgets[i]->Init(VBO,IBO);
		m_widgets[i]->GetText()->SetData(glm::vec2(0, 0));
		m_widgets[i]->SetData(glm::vec2(1, 1));
	}
}

void GuiWrapper::InitCheckBoxes(unsigned int & VBO, unsigned int & IBO)
{
	for (int i = GetCheckBoxesIndexed().x; i < GetCheckBoxesIndexed().y; ++i)
	{
		m_widgets[i]->Init(VBO,IBO);
		m_widgets[i]->GetText()->SetData(glm::vec2(0, 0));
		m_widgets[i]->SetData(glm::vec2(1, 1));
	}
}

void GuiWrapper::InitSliders(unsigned int & VBO, unsigned int & IBO)
{
	for (int i = GetSlidersIndexed().x; i < GetSlidersIndexed().y; ++i)
	{
		m_widgets[i]->Init(VBO,IBO);
		m_widgets[i]->GetText()->SetData(glm::vec2(0, 0));
		m_widgets[i]->SetData(glm::vec2(1, 1));
	}
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

	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

void GuiWrapper::HandleButtonCallbacks()
{
	for (unsigned int i = 0; i < num_buttons + num_arrow_btns; i++)
	{
		if (m_widgets[i]->Clicked())
		{
			m_user_callbacks[i]();
			m_widgets[i]->SetValue(0.5f);
			m_widgets[i]->Clicked() = false;
		}
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

void GuiWrapper::HandleSliders(float xpos)
{
	for (int i = GetSlidersIndexed().x; i < GetSlidersIndexed().y; ++i)
	{
		if (m_widgets[i]->Clicked())
		{
			m_widgets[i]->SetValue(xpos);
			m_widgets[i]->Clicked() = false;
		}
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
		}
	}
}


void GuiWrapper::UpdateData(GuiTransform & transform)
{
	transform.SetScale(m_scale);
	transform.SetPos(glm::vec2(m_position.x,
		m_position.y));
}

void GuiWrapper::SetData(glm::vec2 pos, glm::vec2 scale)
{
	m_scale = scale;
	m_position = glm::vec2(pos.x - Wrapper::WRAPPER_SIZE_X/2*m_scale.x, pos.y - Wrapper::WRAPPER_SIZE_Y / 2*m_scale.y);
	m_text->SetData(m_position);
}

bool GuiWrapper::PinToSide(glm::vec2 winSize)
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
		m_scale.x = winSize.x / (Wrapper::WRAPPER_SIZE_X * m_scale.x);

		m_position.x = 0;
		m_position.y = 0;
		return true;

	}
	else if (m_position.y > winSize.y - (Wrapper::WRAPPER_SIZE_Y * m_scale.y))
	{	
		m_text->SetData( glm::vec2(winSize.x/2-20,winSize.y-20));
		m_pinned = true;
		m_scale.y = 0.5;
		m_scale.x = winSize.x / (Wrapper::WRAPPER_SIZE_X * m_scale.x);

		m_position.x = 0;
		m_position.y = winSize.y - (Wrapper::WRAPPER_SIZE_Y*m_scale.y);
		return true;
	}
	else 
	{
		return false;
	}
	
}

bool GuiWrapper::Exit(glm::vec2 mousePos)
{
	if (mousePos.x >= GetCoords().z - 30 && mousePos.x <= GetCoords().z
		&& mousePos.y <= GetCoords().w + 30 && mousePos.y >= GetCoords().w)
	{
		
		return true;
	}
	else
	{
		return false;
	}
}


bool GuiWrapper::MouseHoover(glm::vec2 mousePos)
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

void GuiWrapper::AddButton(func_ptr func, const std::string & name)
{
	m_user_callbacks.emplace(m_user_callbacks.begin(), func);
	m_widgets.emplace(m_widgets.begin(), new GuiButton(name));
	num_buttons++;

}

void GuiWrapper::AddArrowButton(func_ptr func, const std::string & name)
{
	m_user_callbacks.emplace(m_user_callbacks.begin() + num_buttons, func);
	m_widgets.emplace(m_widgets.begin() + num_buttons, new GuiArrowButton(name));
	num_arrow_btns++;
	
}

void GuiWrapper::AddCheckBox(func_ptr func, const std::string & name)
{
	m_user_callbacks.emplace(m_user_callbacks.begin() + num_buttons + num_arrow_btns, func);
	m_widgets.emplace(m_widgets.begin() + num_buttons + num_arrow_btns, new GuiCheckBox(name));
	num_checkboxes++;
	
}

void GuiWrapper::AddSlider(const std::string & name)
{
	m_widgets.emplace(m_widgets.begin() + num_buttons + num_checkboxes + num_arrow_btns, new GuiSlider(name));
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
