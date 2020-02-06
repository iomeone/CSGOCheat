#pragma once

#include "Menu.h"
#include "RenderUtils.h"

enum InputType
{
	InputType_FULL,
	InputType_ONLY_NUMBERS,
	InputType_ONLY_CHARACTERS
};

class MenuInputBox : public MenuControl
{
public:
	MenuInputBox(std::string display_name, std::string control_name, int w, int h, Color color, InputType type);

	std::string GetInput(); // Get the current input
	void AddKey(int k); // Add the key to the input
	void UpdateVisualInputString(); // Update the visual string
	bool WaitingForInput(); // Get current boolean state
	void Set(std::string s);

	virtual void LeftReleaseEvent() {} // When not clicking anymore [event]
	virtual void LeftPressEvent() // When clicking it [event]
	{
		waiting_for_input = !waiting_for_input;
	}

	virtual void Render(int menu_x, int menu_y) override // We render
	{
		float Middle_Y = (this->GetY() + (this->GetHeight() / 2)) - (RenderUtils::GetTextHeight(this->GetDisplayName(), RenderUtils::Normal_font) / 2);

		RenderUtils::RenderText(menu_x + this->GetX() + this->GetWidth() + 5, menu_y + Middle_Y, Color::White(), this->GetDisplayName().c_str());

		RenderUtils::RenderRect(menu_x + this->GetX(), menu_y + this->GetY(), this->GetWidth(), this->GetHeight(), Color(50, 50, 50, 255));
		RenderUtils::RenderOutlineRect(menu_x + this->GetX(), menu_y + this->GetY(), this->GetWidth(), this->GetHeight(), waiting_for_input ? this->GetColor() : Color(0, 0, 0, 255));

		if (visual_input_string.length() < 1)
		{
			std::string string = "_";
			float MiddleY = (this->GetY() + (this->GetHeight() / 2)) - (RenderUtils::GetTextHeight(string, RenderUtils::Normal_font) / 2);
			float MiddleX = (this->GetX() + (this->GetWidth() / 2)) - (RenderUtils::GetTextLenght(string, RenderUtils::Normal_font) / 2);
			RenderUtils::RenderText(menu_x + MiddleX, menu_y + MiddleY, Color::White(), string.c_str());
		}
		else {
			float MiddleY = (this->GetY() + (this->GetHeight() / 2)) - (RenderUtils::GetTextHeight(this->visual_input_string, RenderUtils::Normal_font) / 2);
			float MiddleX = (this->GetX() + (this->GetWidth() / 2)) - (RenderUtils::GetTextLenght(this->visual_input_string, RenderUtils::Normal_font) / 2);
			RenderUtils::RenderText(menu_x + MiddleX, menu_y + MiddleY, Color::White(), visual_input_string.c_str());
		}
	}

private:
	bool waiting_for_input;
	std::string input_string;
	std::string visual_input_string;
	InputType inputbox_type;
};