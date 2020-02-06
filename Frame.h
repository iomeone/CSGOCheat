#pragma once

#include "Tab.h"
#include <vector>

class MenuFrame 
{
public:
	MenuFrame(std::string title_name, int x, int y, int w, int h, int tbar_h);
	std::vector<MenuTab*> GetTabs(); // Get the tab list
	
	void SetWidth(int w);  // Modify the width of the frame
	void SetHeight(int h); // Modify the height of the frame
	void SetPosition(int x, int y); // Modify the position of the frame
	void SetTitleName(std::string name); // Modify the title of the frame
	void UpdatePositions(); // Modify the position of the frame
	void AddTab(MenuTab* group); // Add a tab to the group list
	void SetMovingBoolean(bool b); // Change the moving boolean of the frame
	void SetLastMouseXDiff(int dx); // Modify the last mouse x difference of the frame
	void SetLastMouseYDiff(int dy); // Modify the last mouse x difference of the frame
	void IncreaseTabsAnimationValue(); // Increase the tabs animation value of the frame
	void DecreaseTabsAnimationValue(); // Decrease the tabs animation value of the frame
	void SetCurrentTab(int t_id); // Set the current selected tab of the frame

	bool IsMoving(); // Is the menu changing positions

	int GetCurrentTab(); // Get the current selected tab of the frame
	int GetX(); // Get the x position from the frame
	int GetY(); // Get the y position from the frame
	int GetLastMouseXDiff(); // Get the last mouse x difference from the frame
	int GetLastMouseYDiff(); // Get the last mouse y difference from the frame
	int GetWidth(); // Get the width from the frame
	int GetHeight(); // Get the height from the frame
	int GetTitleBarHeight(); // Get the title bar height from the frame
	int GetTabsAnimationValue(); // Get the tabs animation value of the frame

	std::string GetTitleName(); // Get the title name from the frame
private:
	bool moving, animation_going_to_right;
	int x, y, w, h, tbar_h, last_mouse_x_diff, last_mouse_y_diff, tab_animation_value, current_tab;
	std::string title_name;
	std::vector<MenuTab*> tabs;
};