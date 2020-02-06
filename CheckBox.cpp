#include "CheckBox.h"

MenuCheckBox::MenuCheckBox(std::string display_name, std::string control_name, int w, int h, Color color, Color default_color)
{
	this->SetDisplayName(display_name);
	this->SetControlName(control_name);

	this->SetWidth(w);
	this->SetHeight(h);

	this->SetColor(color);
	this->custom_color = default_color;
	this->uses_color = true;
	this->color_box_open = false;

	this->state = false;

	this->SetControlType(Control_CheckBox);
}

MenuCheckBox::MenuCheckBox(std::string display_name, std::string control_name, int w, int h, Color color)
{
	this->SetDisplayName(display_name);
	this->SetControlName(control_name);

	this->SetWidth(w);
	this->SetHeight(h);

	this->SetColor(color);
	this->uses_color = false;
	this->color_box_open = false;

	this->state = false;

	this->SetControlType(Control_CheckBox);
}

bool MenuCheckBox::GetState()
{
	return this->state;
}

void MenuCheckBox::OutsideLeftPressEvent(int menu_x, int menu_y, int menu_w, int menu_h)
{
	if (this->IsUsingColor())
	{
		float x = menu_x + this->GetX() + menu_w - 30;
		float y = menu_y + this->GetY() + 3;
		float w = 20;
		float h = this->GetHeight() - 6;

		if (MenuUtils::IsMouseInArea(x, y, w, h))
		{
			this->color_box_open = true;
		}
		else {
			if (this->color_box_open)
			{
				float color_pick_x = menu_x + this->GetX() + menu_w - 30;
				float color_pick_y = menu_y + this->GetY() + this->GetHeight() + 5;
				float color_pick_w = 201;
				float color_pick_h = 201;
				if (MenuUtils::IsMouseInArea(color_pick_x, color_pick_y, color_pick_w, color_pick_h))
				{
					HDC dc = GetDC(NULL);
					COLORREF color = GetPixel(dc, MenuUtils::GetMousePosition().x, MenuUtils::GetMousePosition().y);
					ReleaseDC(NULL, dc);

					this->custom_color = Color(GetRValue(color), GetGValue(color), GetBValue(color));
				}
				else {
					this->color_box_open = false;
				}
			}
		}
	}
}

bool MenuCheckBox::IsColorBoxOpen()
{
	return this->color_box_open;
}

bool MenuCheckBox::IsUsingColor()
{
	return this->uses_color;
}

void MenuCheckBox::SetCustomColor(Color c)
{
	this->custom_color = c;
}

Color MenuCheckBox::GetCustomColor()
{
	return this->custom_color;
}