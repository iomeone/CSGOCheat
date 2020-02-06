#include "Frame.h"
#include "Menu.h"
#include <Windows.h>

MenuFrame::MenuFrame(std::string title_name, int x, int y, int w, int h, int tbar_h)
{
	this->title_name = title_name;
	this->tabs.clear();
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->tbar_h = tbar_h;
	this->tab_animation_value = 10;
	this->current_tab = 0;
}

void MenuFrame::IncreaseTabsAnimationValue()
{
	int max_size = 0;
	for (MenuTab* tab : this->tabs)
	{
		max_size = tab->GetWidth();
		break;
	}

	if (this->tab_animation_value < max_size)
	{
		this->tab_animation_value+=1;
	}
}

void MenuFrame::DecreaseTabsAnimationValue()
{
	if (this->tab_animation_value > 10)
	{
		this->tab_animation_value-=1;
	}
}

int MenuFrame::GetTabsAnimationValue()
{
	return this->tab_animation_value;
}

void MenuFrame::SetMovingBoolean(bool b)
{
	this->moving = b;
}

bool MenuFrame::IsMoving()
{
	return this->moving;
}

int MenuFrame::GetLastMouseXDiff()
{
	return this->last_mouse_x_diff;
}
int MenuFrame::GetLastMouseYDiff()
{
	return this->last_mouse_y_diff;
}

void MenuFrame::SetLastMouseXDiff(int dx)
{
	this->last_mouse_x_diff = dx;
}

void MenuFrame::SetLastMouseYDiff(int dy)
{
	this->last_mouse_y_diff = dy;
}

void MenuFrame::UpdatePositions()
{
	POINT mouse_pos = MenuUtils::GetMousePosition();

	int next_menu_x = mouse_pos.x - this->GetLastMouseXDiff();
	int next_menu_y = mouse_pos.y + this->GetLastMouseYDiff();

	if ((next_menu_x + this->GetWidth()) > RenderUtils::Viewport().right)
		next_menu_x = RenderUtils::Viewport().right - this->GetWidth();

	if ((next_menu_y + this->GetHeight()) > RenderUtils::Viewport().bottom)
		next_menu_y = RenderUtils::Viewport().bottom - this->GetHeight();

	if (next_menu_x < 0)
		next_menu_x = 0;
	if (next_menu_y < 0)
		next_menu_y = 0;

	this->SetPosition(next_menu_x, next_menu_y);
}

int MenuFrame::GetX()
{
	return this->x;
}

int MenuFrame::GetY()
{
	return this->y;
}

int MenuFrame::GetWidth()
{
	return this->w;
}

int MenuFrame::GetHeight()
{
	return this->h;
}

int MenuFrame::GetTitleBarHeight()
{
	return this->tbar_h;
}

std::string MenuFrame::GetTitleName()
{
	return this->title_name;
}

void MenuFrame::SetWidth(int w)
{
	this->w = w;
}

void MenuFrame::SetHeight(int h)
{
	this->h = h;
}

void MenuFrame::SetPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}

int MenuFrame::GetCurrentTab()
{
	return this->current_tab;
}

void MenuFrame::SetCurrentTab(int t_id)
{
	this->current_tab = t_id;
}

void MenuFrame::SetTitleName(std::string name)
{
	this->title_name = name;
}

void MenuFrame::AddTab(MenuTab* control)
{
	this->tabs.push_back(control);
}

std::vector<MenuTab*> MenuFrame::GetTabs()
{
	return this->tabs;
}