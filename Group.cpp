#include "Group.h"

#include "RenderUtils.h"
#include "Menu.h"

MenuGroup::MenuGroup(std::string title_name, int x, int y, int w, int h, Color color, std::vector<MenuGroupTab> tabs)
{
	this->title_name = title_name;
	this->current_tab = 0;
	this->controls.clear();
	this->color = color;
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->tabs = tabs;
}

MenuGroup::MenuGroup(std::string title_name, int x, int y, int w, int h, Color color)
{
	this->title_name = title_name;
	this->current_tab = 0;
	this->controls.clear();
	this->tabs.clear();
	this->color = color;
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

std::vector<MenuGroupTab> MenuGroup::GetTabs()
{
	return this->tabs;
}

std::vector<MenuControl*> MenuGroup::GetControls()
{
	if (tabs.size() > 0)
	{
		for (MenuGroupTab t : tabs)
		{
			if (t.id == this->current_tab)
			{
				return t.controls;
			}
		}
	}
	else {
		return this->controls;
	}
}

void MenuGroup::SetCurrentTab(int id)
{
	this->current_tab = id;
}

void MenuGroup::SetPositionAndSize(int x, int y, int w, int h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

int MenuGroup::GetXPerControl(MenuControl* control)
{
	int x = 0;

	switch (control->GetControlType())
	{
	case Control_KeyBox:
	case Control_CheckBox:
	case Control_InputBox:
	case Control_Slider:
	case Control_ComboBox:
	case Control_SelectionBox:
		x = 5;
		break;
	default:
		break;
	}

	return x;
}

void MenuGroup::OrganizeControls()
{
	int next_y = 0;
	for (MenuControl* control : this->GetControls())
	{
		control->SetPosition(GetXPerControl(control), next_y);
		next_y += (control->GetHeight() + 5);
	}
}

void MenuGroup::AddControl(MenuControl* control)
{
	this->controls.push_back(control);
}

int MenuGroup::GetHeight()
{
	return this->h;
}

int MenuGroup::GetWidth()
{
	return this->w;
}

int MenuGroup::GetX()
{
	return this->x;
}

int MenuGroup::GetY()
{
	return this->y;
}

Color MenuGroup::GetColor()
{
	return this->color;
}

void MenuGroup::Render(int frame_x, int frame_y)
{	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// We draw under all the title and the background

	int group_x = frame_x + this->x;
	int group_y = frame_y + this->y;

	RenderUtils::RenderRect(group_x, group_y, this->w, this->h, Color(70, 70, 70, 255));  // We draw under the controls (group background)
	RenderUtils::RenderOutlineRect(group_x, group_y, this->w, this->h, Color(0, 0, 0, 255));  // We draw under the controls (group background)

	RenderUtils::RenderCheatTitle(group_x, group_y, this->w, 12);
	RenderUtils::RenderOutlineRect(group_x, group_y, this->w, 12, Color(0, 0, 0, 255));

	RenderUtils::RenderText(group_x + ((float)this->w / 2.f) - (RenderUtils::GetTextLenght(this->title_name, RenderUtils::Normal_font) / 2), group_y + 6 - (RenderUtils::GetTextHeight(this->title_name, RenderUtils::Normal_font) / 2), Color::White(), this->title_name.c_str());

	if (tabs.size() > 0)
	{
		group_y += 12;

		float width_per_tab = ((float)this->w / (float)tabs.size());
		float end_of_tab = group_x + this->w;

		int tab_id = 0;
		int next_x = 1;
		int tab_height = 20;

		for (MenuGroupTab &group_tab : tabs)
		{
			float MiddleX = group_x + next_x + (width_per_tab / 2) - (RenderUtils::GetTextLenght(group_tab.name, RenderUtils::Normal_Bold_font) / 2);
			float MiddleY = group_y + (tab_height / 2) - (RenderUtils::GetTextHeight(group_tab.name, RenderUtils::Normal_Bold_font) / 2);

			RenderUtils::RenderRect(group_x + next_x, group_y, width_per_tab, 20, group_tab.id == this->current_tab ? Color(100, 100, 100, 255) : Color(70, 70, 70, 255));
			RenderUtils::RenderText(MiddleX, MiddleY, Color::White(), group_tab.name.c_str());

			if (tab_id < tabs.size() && tab_id > 0)
			{
				RenderUtils::RenderLine(group_x + next_x, group_y, group_x + next_x, group_y + tab_height, Color(0, 0, 0, 255));
			}

			tab_id += 1;
			next_x += width_per_tab;
		}

		RenderUtils::RenderLine(group_x, group_y, end_of_tab, group_y, Color(0, 0, 0, 255));
		RenderUtils::RenderLine(group_x, group_y + tab_height, end_of_tab, group_y + tab_height, Color(0, 0, 0, 255));

		group_y += 15;
	}

	group_y += 15;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// We draw the controls
	int next_y = 0;
	for (MenuControl* control : this->GetControls())
	{
		control->SetPosition(GetXPerControl(control), next_y);
		control->Render(group_x, group_y);
		control->RenderAdvanced(group_x, group_y, this->w, this->h);
		next_y += (control->GetHeight() + 5);
	}

	// We draw over the other controls
	for (MenuControl* control : this->GetControls())
	{
		if (control->GetControlType() == Control_ComboBox)
		{
			MenuComboBox* combo_box = (MenuComboBox*)control;
			if (combo_box->IsOpen())
			{
				control->Render(group_x, group_y);
				control->RenderAdvanced(group_x, group_y, this->w, this->h);
			}
		}
		if (control->GetControlType() == Control_SelectionBox)
		{
			MenuSelectionBox* selection_box = (MenuSelectionBox*)control;
			if (selection_box->IsOpen())
			{
				control->Render(group_x, group_y);
				control->RenderAdvanced(group_x, group_y, this->w, this->h);
			}
		}
		if (control->GetControlType() == Control_CheckBox)
		{
			MenuCheckBox* check_box = (MenuCheckBox*)control;
			if (check_box->IsColorBoxOpen() && check_box->IsUsingColor())
			{
				control->Render(group_x, group_y);
				control->RenderAdvanced(group_x, group_y, this->w, this->h);
			}
		}
	}
}