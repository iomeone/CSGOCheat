#pragma once

#include "Menu.h"
#include "RenderUtils.h"
#include "Control.h"

class MenuSlider : public MenuControl
{
public:
	MenuSlider(std::string display_name, std::string control_name, int w, int h, int default_v, int min, int max, Color color);

	int GetValue(); // Get the value of the slider
	void SetValue(int i); // Set the value of the slider

	bool IsSliderMoving(); // Is slider moving
	
	void SetMouseDelta(int m); // Set mouse delta of the slider
	void SetSliderBoolean(bool b); // Set the value of the boolean

	virtual void LeftReleaseEvent() // When not clicking anymore [event]
	{
		this->slider_changing = false;
	} 
	virtual void LeftPressEvent() // When clicking it [event]
	{
		float w_per_step = (float)this->GetWidth() / (float)(this->max - this->min);
		int result = (int)((float)this->mouse_delta / w_per_step);
		result += this->min;

		if (result > max)
			result = max;
		
		if (result < min)
			result = min;
		
		this->value = result;
	}

	virtual void Render(int menu_x, int menu_y) override // We render
	{
		float MiddleY = this->GetY() + (this->GetHeight() / 2) - (RenderUtils::GetTextHeight(this->GetDisplayName(), RenderUtils::Normal_font) / 2);
		
		float MiddleY_value = this->GetY() + (this->GetHeight() / 2) - (RenderUtils::GetTextHeight(std::to_string(this->GetValue()), RenderUtils::Normal_font) / 2);
		float MiddleX_value = this->GetX() + (this->GetWidth() / 2) - (RenderUtils::GetTextLenght(std::to_string(this->GetValue()), RenderUtils::Normal_font) / 2);

		RenderUtils::RenderText(menu_x + this->GetX() + this->GetWidth() + 5, menu_y + MiddleY, Color::White(), this->GetDisplayName().c_str());

		RenderUtils::RenderRect(menu_x + this->GetX(), menu_y + this->GetY(), this->GetWidth(), this->GetHeight(), Color(50, 50, 50, 255));
		RenderUtils::RenderOutlineRect(menu_x + this->GetX(), menu_y + this->GetY(), this->GetWidth(), this->GetHeight(), Color(0, 0, 0, 255));
	
		float w_per_step = (float)this->GetWidth() / (float)(this->max - this->min);
		float final_calc_width = ((float)(this->GetValue() - this->min) * w_per_step);
		RenderUtils::RenderRect(menu_x + this->GetX(), menu_y + this->GetY(), final_calc_width, this->GetHeight(), this->GetColor());
		RenderUtils::RenderOutlineRect(menu_x + this->GetX(), menu_y + this->GetY(), final_calc_width, this->GetHeight(), Color(0,0,0,255));
	
		RenderUtils::RenderText(menu_x + MiddleX_value, menu_y + MiddleY_value, Color::White(), std::to_string(this->GetValue()).c_str());
	}

private:
	int value, min, max, mouse_delta;
	bool slider_changing;
};