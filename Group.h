#pragma once

#include "Control.h"
#include <vector>

struct MenuGroupTab
{
	MenuGroupTab(std::string name, int id)
	{
		this->controls.clear();
		this->name = name;
		this->id = id;
	}
	int id;
	std::string name;
	std::vector<MenuControl*> controls;

	void AddControl(MenuControl* c)
	{
		this->controls.push_back(c);
	}
};

class MenuGroup
{
public:
	MenuGroup(std::string title_name, int x, int y, int w, int h, Color color);
	MenuGroup(std::string title_name, int x, int y, int w, int h, Color color, std::vector<MenuGroupTab> tabs);

	std::vector<MenuControl*> GetControls(); // Get the control list
	std::vector<MenuGroupTab> GetTabs(); // Get the tab list

	Color GetColor(); // Returns the color 

	int GetY(); // Returns the value of the y position
	int GetX(); // Returns the value of the x position
	int GetHeight(); // Returns the value of the height
	int GetWidth(); // Returns the value of the width
	int GetXPerControl(MenuControl* control); // Returns the x for the control

	void SetCurrentTab(int id); // Change the current tab 
	void SetPositionAndSize(int x, int y, int w, int h); // Change the position and size
	void OrganizeControls(); // Organize all the controls
	void AddControl(MenuControl* control); // Add a control to the control list
	void Render(int frame_x, int frame_y); // Group render
private:
	int x, y, w, h, current_tab;
	Color color;
	std::vector<MenuControl*> controls;
	std::vector<MenuGroupTab> tabs;
	std::string title_name;
};