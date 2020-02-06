#include "Tab.h"
#include "Menu.h"
#include "RenderUtils.h"

void MenuTab::Render(int frame_x, int frame_y, int animation_value, bool selected)
{
	int calc_x = frame_x - animation_value;
	int calc_w = (int)((float)animation_value / 100.f * (float)this->w);

	RenderUtils::RenderRect(calc_x, frame_y, calc_w, this->h, selected ? Color(100, 100, 100, 255) : Color(70, 70, 70, 255));
	RenderUtils::RenderOutlineRect(calc_x, frame_y, calc_w, this->h, Color(0, 0, 0, 255));

	float text_x = calc_x + ((float)calc_w / 2.f) - ((float)RenderUtils::GetTextLenght(this->tab_name, RenderUtils::Normal_font) / 2.f);
	float text_y = frame_y + ((float)this->h / 2.f) - ((float)RenderUtils::GetTextHeight(this->tab_name, RenderUtils::Normal_font) / 2.f);

	float alpha = ((float)animation_value / 100.f * 255.f);
	if (calc_w > RenderUtils::GetTextLenght(this->tab_name, RenderUtils::Normal_font))
	{
		RenderUtils::RenderText(text_x, text_y, Color(255,255,255, alpha), this->tab_name.c_str());
	}
}
//
//void MenuTab::UpdateGroupBoxes(int frame_h, int h_per_group, int w_per_group)
//{
//	int separation_y = 15;
//
//	int next_y = separation_y;
//	int next_x = 5;
//
//	h_per_group -= separation_y;
//	w_per_group -= 5;
//
//	for (MenuGroup* group : this->groups)
//	{
//		int next = next_y + (h_per_group * 2) + separation_y;
//		if (next > frame_h)
//		{
//			next_y = 15;
//			next_x += w_per_group + 5;
//			w_per_group -= 5;
//		}
//		else {
//			next_y += h_per_group + separation_y + 5;
//			h_per_group -= separation_y + 5;
//		}
//	}
//
//	next_y = separation_y;
//	next_x = 5;
//
//	for (MenuGroup* group : this->groups)
//	{
//		group->SetPositionAndSize(next_x, next_y, w_per_group, h_per_group);
//		
//		int next = next_y + (h_per_group * 2) + separation_y;
//		if (next > frame_h)
//		{
//			next_y = 15;
//			next_x += w_per_group + 5;
//		}
//		else {
//			next_y += h_per_group + separation_y + 5;
//		}
//	}
//}

int MenuTab::GetHeight()
{
	return this->h;
}

int MenuTab::GetWidth()
{
	return this->w;
}

std::string MenuTab::GetTabName()
{
	return this->tab_name;
}

MenuTab::MenuTab(std::string tab_name, int height)
{
	this->w = 100;
	this->tab_name = tab_name;
	this->h = height;
}

void MenuTab::AddGroup(MenuGroup* control)
{
	this->groups.push_back(control);
}

std::vector<MenuGroup*> MenuTab::GetGroups()
{
	return this->groups;
}