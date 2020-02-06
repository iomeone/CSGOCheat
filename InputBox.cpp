#include "InputBox.h"

MenuInputBox::MenuInputBox(std::string display_name, std::string control_name, int w, int h, Color color, InputType type)
{
	this->SetDisplayName(display_name);
	this->SetControlName(control_name);

	this->SetWidth(w);
	this->SetHeight(h);

	this->SetColor(color);

	this->waiting_for_input = false;
	this->inputbox_type = type;
	this->Set("");

	this->SetControlType(Control_InputBox);
}

void MenuInputBox::UpdateVisualInputString()
{
	if (RenderUtils::GetTextLenght(input_string, RenderUtils::Normal_font) >= this->GetWidth())
	{
		float width_per_character = (float)RenderUtils::GetTextLenght(input_string, RenderUtils::Normal_font) / (float)input_string.length();
		int max_characters = (roundf((float)this->GetWidth() / (float)width_per_character));
		input_string = input_string.substr(0, max_characters - 3) + "...";
	}
}

void MenuInputBox::AddKey(int k)
{
	if (k == VK_END)
	{
		this->waiting_for_input = false;
	}
	else {
		if (k == VK_SPACE)
		{
			this->input_string += " ";
		}
		else {
			if (k == VK_BACK)
			{
				if (this->input_string.length() > 0)
				{
					this->input_string.pop_back();
				}
			}
			else {
				std::string key_name = "";
				switch (inputbox_type)
				{
				case InputType::InputType_FULL:
					key_name = MenuUtils::VirtualToKey(k);
					break;
				case InputType::InputType_ONLY_NUMBERS:
					key_name = MenuUtils::VirtualToNumber(k);
					break;
				case InputType::InputType_ONLY_CHARACTERS:
					key_name = MenuUtils::VirtualToCharacter(k);
					break;
				default:
					break;
				}

				if (key_name.length() > 0)
				{
					this->input_string += key_name;
				}
			}
		}
	}
	this->visual_input_string = this->input_string;
	UpdateVisualInputString();
}

std::string MenuInputBox::GetInput()
{
	return this->input_string;
}

void MenuInputBox::Set(std::string s)
{
	this->visual_input_string = s;
	this->input_string = s;
	UpdateVisualInputString();
}

bool MenuInputBox::WaitingForInput()
{
	return this->waiting_for_input;
}