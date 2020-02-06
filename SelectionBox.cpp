#include "SelectionBox.h"

MenuSelectionBox::MenuSelectionBox(std::string display_name, std::string control_name, int w, int h, Color color)
{
	this->SetDisplayName(display_name);
	this->SetControlName(control_name);

	this->SetWidth(w);
	this->SetHeight(h);

	this->SetColor(color);

	this->SetControlType(Control_SelectionBox);

	this->per_item_h = h;
	this->selected_ids.clear();
	this->open = false;
}

bool MenuSelectionBox::IsSelectedItem(int id)
{
	bool in_list = false;
	for (int i : this->selected_ids)
	{
		if (id == i)
		{
			in_list = true;
		}
	}
	return in_list;
}

bool MenuSelectionBox::MouseInArea(int x, int y, int w, int h)
{
	return MenuUtils::IsMouseInArea(x, y, w, h);
}

void MenuSelectionBox::RemoveSelectedItem(int id)
{
	std::vector<int> new_list;
	for (int i : this->selected_ids)
	{
		if (id != i)
		{
			new_list.push_back(i);
		}
	}
	this->selected_ids = new_list;
}

void MenuSelectionBox::RemoveSelectedItems()
{
	this->selected_ids.clear();
}

void MenuSelectionBox::SetSelectedItem(int id)
{
	bool already_in = false;
	for (int i : this->selected_ids)
	{
		if (id == i)
		{
			already_in = true;
		}
	}

	if (!already_in)
	{
		this->selected_ids.push_back(id);
	}
	else {
		RemoveSelectedItem(id);
	}
}

int MenuSelectionBox::GetHeightPerItem()
{
	return this->per_item_h;
}

std::vector<std::string> MenuSelectionBox::GetItems()
{
	return this->items;
}

bool MenuSelectionBox::IsOpen()
{
	return this->open;
}

void MenuSelectionBox::AddItem(std::string s)
{
	this->items.push_back(s);
}

std::vector<int> MenuSelectionBox::GetSelectedItems()
{
	return this->selected_ids;
}