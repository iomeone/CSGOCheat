#include "Control.h"
#include "Menu.h"
#include <Windows.h>

ControlType MenuControl::GetControlType()
{
	return this->control_type;
}

void MenuControl::SetControlType(ControlType type)
{
	this->control_type = type;
}

Color MenuControl::GetColor()
{
	return this->color;
}

int MenuControl::GetX()
{
	return this->x;
}

int MenuControl::GetY()
{
	return this->y;
}

int MenuControl::GetWidth()
{
	return this->w;
}

int MenuControl::GetHeight()
{
	return this->h;
}

std::string MenuControl::GetDisplayName()
{
	return this->display_name;
}

std::string MenuControl::GetControlName()
{
	return this->control_name;
}

bool MenuControl::IsMouseOver(int menu_x, int menu_y)
{
	POINT p = MenuUtils::GetMousePosition();
	
	if (p.x >= (menu_x + this->x) && p.x <= (menu_x + this->x + this->w) && p.y >= (menu_y + this->y) && p.y <= (menu_y + this->y + this->h))
	{
		return true;
	}
	return false;
}

void MenuControl::SetColor(Color c)
{
	this->color = c;
}

void MenuControl::SetWidth(int w)
{
	this->w = w;
}

void MenuControl::SetHeight(int h) 
{
	this->h = h;
}

void MenuControl::SetPosition(int x, int y) 
{
	this->x = x;
	this->y = y;
}

void MenuControl::SetControlName(std::string name) 
{
	this->control_name = name;
}

void MenuControl::SetDisplayName(std::string name) 
{
	this->display_name = name;
}
