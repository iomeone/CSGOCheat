#pragma once

#include "Menu.h"
#include "RenderUtils.h"

enum KeyType
{
	KeyType_FULL,
	KeyType_ONLY_NUMBERS,
	KeyType_ONLY_CHARACTERS
};

class MenuKeyBox : public MenuControl
{
public:
	MenuKeyBox(std::string display_name, std::string control_name, int w, int h, Color color, KeyType type);

	int GetKey(); // Get the current key
	void SetKey(int k); // Set the current key
	bool WaitingForKey(); // Get current boolean state

	virtual void LeftReleaseEvent() {} // When not clicking anymore [event]
	virtual void LeftPressEvent() // When clicking it [event]
	{
		waiting_for_key = !waiting_for_key;
	}

	virtual void Render(int menu_x, int menu_y) override // We render
	{
		float Middle_Y = (this->GetY() + (this->GetHeight() / 2)) - (RenderUtils::GetTextHeight(this->GetDisplayName(), RenderUtils::Normal_font) / 2);
	
		RenderUtils::RenderText(menu_x + this->GetX() + this->GetWidth() + 5, menu_y + Middle_Y, Color::White(), this->GetDisplayName().c_str());
		
		RenderUtils::RenderRect(menu_x + this->GetX(), menu_y + this->GetY(), this->GetWidth(), this->GetHeight(), Color(50, 50, 50, 255));
		RenderUtils::RenderOutlineRect(menu_x + this->GetX(), menu_y + this->GetY(), this->GetWidth(), this->GetHeight(), waiting_for_key ? this->GetColor() : Color(0, 0, 0, 255));
		
		if (key_string.length() < 1)
		{
			std::string string = "_";
			float MiddleY = (this->GetY() + (this->GetHeight() / 2)) - (RenderUtils::GetTextHeight(string, RenderUtils::Normal_font) / 2);
			float MiddleX = (this->GetX() + (this->GetWidth() / 2)) - (RenderUtils::GetTextLenght(string, RenderUtils::Normal_font) / 2);
			RenderUtils::RenderText(menu_x + MiddleX, menu_y + MiddleY, Color::White(), string.c_str());
		}
		else {
			float MiddleY = (this->GetY() + (this->GetHeight() / 2)) - (RenderUtils::GetTextHeight(this->key_string, RenderUtils::Normal_font) / 2);
			float MiddleX = (this->GetX() + (this->GetWidth() / 2)) - (RenderUtils::GetTextLenght(this->key_string, RenderUtils::Normal_font) / 2);
			RenderUtils::RenderText(menu_x + MiddleX, menu_y + MiddleY, Color::White(), key_string.c_str());
		}
	}

private:
	int key;
	bool waiting_for_key;
	std::string key_string;
	KeyType keybox_type;
};