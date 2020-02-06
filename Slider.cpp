#include "Slider.h"

MenuSlider::MenuSlider(std::string display_name, std::string control_name, int w, int h, int default_v, int min, int max, Color color)
{
	this->SetDisplayName(display_name);
	this->SetControlName(control_name);

	this->SetWidth(w);
	this->SetHeight(h);
	this->min = min;
	this->max = max;
	this->value = default_v;

	this->SetColor(color);

	this->SetControlType(Control_Slider);
}

void MenuSlider::SetSliderBoolean(bool b)
{
	this->slider_changing = b;
}

void MenuSlider::SetMouseDelta(int m)
{
	this->mouse_delta = m;
}

bool MenuSlider::IsSliderMoving()
{
	return this->slider_changing;
}

void MenuSlider::SetValue(int i)
{
	this->value = i;
}

int MenuSlider::GetValue()
{
	return this->value;
}