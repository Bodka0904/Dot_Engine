#pragma once
#include "events/GuiEvent.h"
#include "widgets/GuiWidget.h"
#include "data/GuiData.h"
#include "graphics/GuiShader.h"
#include "graphics/GuiTransform.h"
#include "graphics/GuiArrayBuffer.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <functional>
#include <vector>


class GuiWrapper
{
public:
	typedef std::function<void()> func_ptr;

	GuiWrapper(const std::string& name);
	~GuiWrapper();

	void Draw(GuiShader& shader, GuiTransform& transform);

	void DrawButtons(GuiShader& shader, GuiTransform& transform);
	void DrawArrButtons(GuiShader& shader, GuiTransform& transform);
	void DrawCheckBoxes(GuiShader& shader, GuiTransform& transform);
	void DrawSliders(GuiShader& shader, GuiTransform& transform);
	void DrawTexts(GuiTransform& transform);


	void HandleButtonCallbacks();
	void HandleCheckBoxCallbacks();
	void HandleSliders(float xpos);
	void HandleReleaseButton(GuiEvent &event,float xpos, float ypos);

	void UpdateData(GuiTransform& transform);

	void SetData(glm::vec2& pos, glm::vec2& scale = glm::vec2(1, 1));
	void SetWidgetsFollow();
	void SetWidget(int index,glm::vec2 position);
	void SetWidgetsNextTo();
	void SetSize(glm::vec2& scale) { m_scale = scale; };

	bool PinToSide(glm::vec2& winSize);
	

	bool &GetPinned() { return m_pinned; }
	bool &GetVisible() {return m_visible;}
	GuiText* GetText() { return m_text; }
	bool MouseHoover(glm::vec2& mousePos);

	glm::vec2 GetCenter() const;
	glm::vec2 GetPosition() const;
	glm::vec2 GetScale() const { return m_scale; }

	std::vector<std::shared_ptr<GuiWidget>>GetWidgets() { return m_widgets; }
	std::vector<func_ptr>GetCallbacks() { return m_user_callbacks; }

	void AddButton(func_ptr func, const std::string& name,const glm::vec2& size = glm::vec2(60,40));
	void AddArrowButton(func_ptr func, const std::string& name, const glm::vec2& size = glm::vec2(60, 40));
	void AddCheckBox(func_ptr func, const std::string& name, const glm::vec2& size = glm::vec2(40, 40));
	void AddSlider(const std::string& name, const glm::vec2& size = glm::vec2(150, 20));
	
	unsigned int GetNumButtons() const { return num_buttons; }
	unsigned int GetNumArrButtons() const { return num_arrow_btns; }
	unsigned int GetNumCheckBoxes() const { return num_checkboxes; }
	unsigned int GetNumSliders() const { return  num_sliders; }

	glm::vec2 GetButtonsIndexed() const;
	glm::vec2 GetArrButtonsIndexed() const;
	glm::vec2 GetCheckBoxesIndexed() const;
	glm::vec2 GetSlidersIndexed() const;

	GuiWidget& GetButton(int index);
	GuiWidget& GetArrowButton(int index);
	GuiWidget& GetCheckBox(int index);
	GuiWidget& GetSlider(int index);

private:
	glm::vec4 GetCoords();	
	

private:
	std::shared_ptr<GuiArrayBuffer>m_VAO;

	glm::vec2 m_position = glm::vec2(0, 0);
	glm::vec2 m_scale;

	bool m_pinned = false;
	bool m_visible = true;
	float m_color = 0;


	unsigned int m_width;
	unsigned int m_height;

private:
	GuiText* m_text;

	
	std::vector <std::shared_ptr<GuiWidget>> m_widgets;
	std::vector<func_ptr> m_user_callbacks;

	unsigned int num_buttons = 0;
	unsigned int num_checkboxes = 0;
	unsigned int num_sliders = 0;
	unsigned int num_arrow_btns = 0;

};

