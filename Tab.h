#pragma once

#include "Group.h"
#include <vector>

class MenuTab
{
public:
	MenuTab(std::string tab_name, int height);

	void AddGroup(MenuGroup* control); // Add a group to the group list
	void Render(int frame_x, int frame_y, int animation_value, bool selected); // Render the tab
	//void UpdateGroupBoxes(int frame_h, int h_per_group, int w_per_group); // We change the positions and the sizes of the group boxes

	int GetHeight(); // Get the height of the tab
	int GetWidth(); // Get the width of the tab

	std::string GetTabName(); // Get the name of the tab

	std::vector<MenuGroup*> GetGroups(); // Get the group list
private:
	int h, w;
	std::string tab_name;
	std::vector<MenuGroup*> groups;
};