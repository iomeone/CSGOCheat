#pragma once

#include "Color.h"
#include <string>

enum ControlType
{
	Control_CheckBox,
	Control_ComboBox,
	Control_SelectionBox,
	Control_KeyBox,
	Control_InputBox,
	Control_Slider
};

class MenuControl
{
public:
	void SetColor(Color c); // Modify the color of the control
	void SetWidth(int w);  // Modify the width of the control
	void SetHeight(int h); // Modify the height of the control
	void SetPosition(int x, int y); // Modify the position of the control
	void SetControlName(std::string name); // Modify the name of the control
	void SetDisplayName(std::string name); // Modify the display name of the control
	void SetControlType(ControlType type); // Modify the control type of the control

	ControlType GetControlType(); // Get the control type from the control

	Color GetColor(); // Get the color from the control

	int GetX(); // Get the x position from the control
	int GetY(); // Get the y position from the control
	int GetWidth(); // Get the width from the control
	int GetHeight(); // Get the height from the control

	std::string GetDisplayName(); // Get the display name from the control
	std::string GetControlName(); // Get the control name from the control

	bool IsMouseOver(int menu_x, int menu_y); // Is the mouse over the control

	virtual void LeftReleaseEvent() {} // When not clicking anymore [event]
	virtual void LeftPressEvent() {} // When clicking it [event]
	virtual void Render(int menu_x, int menu_y) {}// We render the control
	virtual void RenderAdvanced(int menu_x, int menu_y, int menu_width, int menu_height) {}// We render the control with more info
private:
	bool pressed;
	int x, y, w, h;
	std::string display_name, control_name;
	Color color;
	ControlType control_type;
};