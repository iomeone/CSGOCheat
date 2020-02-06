#include "ComboBox.h"

MenuComboBox::MenuComboBox(std::string display_name, std::string control_name, int w, int h, Color color)
{
	this->SetDisplayName(display_name);
	this->SetControlName(control_name);

	this->SetWidth(w);
	this->SetHeight(h);

	this->SetColor(color);

	this->SetControlType(Control_ComboBox);

	this->per_item_h = h;
	this->selected_id = 0;
	this->open = false;
}

bool MenuComboBox::MouseInArea(int x, int y, int w, int h)
{
	return MenuUtils::IsMouseInArea(x, y, w, h);
}

void MenuComboBox::SetSelectedItem(int id)
{
	this->selected_id = id;
}

int MenuComboBox::GetHeightPerItem()
{
	return this->per_item_h;
}

std::vector<std::string> MenuComboBox::GetItems()
{
	return this->items;
}

bool MenuComboBox::IsOpen()
{
	return this->open;
}

void MenuComboBox::AddItem(std::string s)
{
	this->items.push_back(s);
}

int MenuComboBox::GetSelectedItem()
{
	return this->selected_id;
}