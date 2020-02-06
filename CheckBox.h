#pragma once

#include "Menu.h"
#include "RenderUtils.h"
#include "Control.h"

class MenuCheckBox : public MenuControl
{
public:
	MenuCheckBox(std::string display_name, std::string control_name, int w, int h, Color color);
	MenuCheckBox(std::string display_name, std::string control_name, int w, int h, Color color, Color default_color);

	bool GetState(); // Get current boolean state
	bool IsUsingColor(); // If the control uses the color picker
	bool IsColorBoxOpen(); // If the color picker is open
	Color GetCustomColor(); // Get current color
	void SetCustomColor(Color c); // Get current color

	void OutsideLeftPressEvent(int menu_x, int menu_y, int menu_w, int menu_h); // When clicking but not the checkbox [event]

	virtual void LeftReleaseEvent() {} // When not clicking anymore [event]
	virtual void LeftPressEvent() // When clicking it [event]
	{
		this->color_box_open = false;
		this->state = !this->state;
	}

	virtual void RenderAdvanced(int menu_x, int menu_y, int menu_w, int menu_h) override // We render
	{
		float MiddleY = (this->GetY() + (this->GetHeight() / 2)) - (RenderUtils::GetTextHeight(this->GetDisplayName(), RenderUtils::Normal_font) / 2);
		RenderUtils::RenderText(menu_x + this->GetX() + this->GetWidth() + 5, menu_y + MiddleY, Color::White(), this->GetDisplayName().c_str());
		RenderUtils::RenderRect(menu_x + this->GetX(), menu_y + this->GetY(), this->GetWidth(), this->GetHeight(), this->GetState() ? this->GetColor() : Color(50, 50, 50, 255));
		RenderUtils::RenderOutlineRect(menu_x + this->GetX(), menu_y + this->GetY(), this->GetWidth(), this->GetHeight(), Color(0, 0, 0, 255));

		if (this->IsUsingColor())
		{
			RenderUtils::RenderRect(menu_x + this->GetX() + menu_w - 30, menu_y + this->GetY() + 3, 20, this->GetHeight() - 6, this->GetCustomColor());
			RenderUtils::RenderOutlineRect(menu_x + this->GetX() + menu_w - 30, menu_y + this->GetY() + 3, 20, this->GetHeight() - 6, Color(0, 0, 0, 255));

			if (/*this->color_box_open*/ false) // BROKEN
			{
				float color_pick_x = menu_x + this->GetX() + menu_w - 30;
				float color_pick_y = menu_y + this->GetY() + this->GetHeight() + 5;
				RenderUtils::RenderColorPicker(color_pick_x, color_pick_y, 201, 201);
				RenderUtils::RenderRect(color_pick_x, color_pick_y, 201, 201, Color::Red());
				RenderUtils::RenderOutlineRect(color_pick_x - 1, color_pick_y - 1, 201, 201, Color(0, 0, 0, 255));
			}
		}
	}

private:
	Color custom_color;
	bool state, uses_color, color_box_open;
};