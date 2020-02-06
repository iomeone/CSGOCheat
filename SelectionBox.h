#pragma once

#include "Menu.h"
#include "RenderUtils.h"
#include "InterfaceManager.h"
#include "Control.h"

class MenuSelectionBox : public MenuControl
{
public:
	MenuSelectionBox(std::string display_name, std::string control_name, int w, int h, Color color);

	std::vector<int> GetSelectedItems(); // Get the selected item id
	int GetHeightPerItem(); // Get the height per item in the item list

	bool IsOpen(); // Is the item list open
	bool MouseInArea(int x, int y, int w, int h); // To evade the error C3861
	bool IsSelectedItem(int id); // If the item is selected

	void RemoveSelectedItems(); // Remove the selected items
	void AddItem(std::string s); // Add item to the item list
	void SetSelectedItem(int id); // Set the selected item id
	void RemoveSelectedItem(int id); // Remove the selected item id

	std::vector<std::string> GetItems(); // Get the item list

	virtual void LeftReleaseEvent() {} // When not clicking anymore [event]
	virtual void LeftPressEvent() // When clicking it [event]
	{
		open = !open;
	}

	virtual void Render(int menu_x, int menu_y) override // We render
	{
		float MiddleY = (this->GetY() + (this->GetHeight() / 2)) - (RenderUtils::GetTextHeight(this->GetDisplayName(), RenderUtils::Normal_font) / 2);
		RenderUtils::RenderText(menu_x + this->GetX() + this->GetWidth() + 5, menu_y + MiddleY, Color::White(), this->GetDisplayName().c_str());
	
		if (this->open)
		{
			int height = this->GetHeightPerItem() * this->items.size();
			RenderUtils::RenderRect(menu_x + this->GetX(), menu_y + this->GetY(), this->GetWidth(), height, Color(50, 50, 50, 255));
		}
		else {
			RenderUtils::RenderRect(menu_x + this->GetX(), menu_y + this->GetY(), this->GetWidth(), this->GetHeight(), Color(50, 50, 50, 255));
		}
	
		if (this->open)
		{
			int y = 0;
			int id = 0;
			for (std::string &i : items)
			{
				bool over_item = this->MouseInArea(menu_x + this->GetX(), menu_y + this->GetY() + y, this->GetWidth(), this->per_item_h);

				int Text_Y = (menu_y + this->GetY() + y) + ((float)per_item_h / 2.f) - ((float)RenderUtils::GetTextHeight(i, RenderUtils::Normal_font) / 2.f);
				RenderUtils::RenderText(menu_x + this->GetX() + (over_item ? 10 : 5), Text_Y, this->IsSelectedItem(id) ? this->GetColor() : Color::White(), i.c_str(), RenderUtils::Normal_font);

				id += 1;
				y += per_item_h;
			}
		}
		else {
			std::string selected_items = "";
			bool first_item_to_add = true;

			int id = 0;
			for (std::string &i : items)
			{
				if (IsSelectedItem(id))
				{
					selected_items += (first_item_to_add ? i : "," + i);
					first_item_to_add = false;
				}
				id += 1;
			}

			if (RenderUtils::GetTextLenght(selected_items, RenderUtils::Normal_font) >= this->GetWidth())
			{
				float width_per_character = (float)RenderUtils::GetTextLenght(selected_items, RenderUtils::Normal_font) / (float)selected_items.length();
				int max_characters = (roundf((float)this->GetWidth() / (float)width_per_character));
				selected_items = selected_items.substr(0,max_characters - 3) + "...";
			}

			if (selected_items.length() < 1)
			{
				selected_items = "_";
			}

			int Text_X = (menu_x + this->GetX()) + ((float)this->GetWidth() / 2.f) - ((float)RenderUtils::GetTextLenght(selected_items, RenderUtils::Normal_font) / 2.f);
			int Text_Y = (menu_y + this->GetY()) + ((float)this->per_item_h / 2.f) - ((float)RenderUtils::GetTextHeight(selected_items, RenderUtils::Normal_font) / 2.f);
		
			RenderUtils::RenderText(Text_X, Text_Y, Color::White(), selected_items.c_str());

		}

		RenderUtils::TriangleFilled(Vector2D(menu_x + this->GetX() + this->GetWidth() - 15, menu_y + this->GetY() + this->GetHeight() - 5),
			Vector2D(menu_x + this->GetX() + this->GetWidth() - 10, menu_y + this->GetY() + 5),
			Vector2D(menu_x + this->GetX() + this->GetWidth() - 5, menu_y + this->GetY() + this->GetHeight() - 5), Color::White());

		if (this->open)
		{
			int height = this->GetHeightPerItem() * this->items.size();
			RenderUtils::RenderOutlineRect(menu_x + this->GetX(), menu_y + this->GetY(), this->GetWidth(), height, Color(0, 0, 0, 255));
		}
		else {
			RenderUtils::RenderOutlineRect(menu_x + this->GetX(), menu_y + this->GetY(), this->GetWidth(), this->GetHeight(), Color(0, 0, 0, 255));
		}
	}

private:
	bool open;
	std::vector<int> selected_ids;
	int per_item_h;
	std::vector<std::string> items;
};