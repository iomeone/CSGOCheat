#include "KeyBox.h"

MenuKeyBox::MenuKeyBox(std::string display_name, std::string control_name, int w, int h, Color color, KeyType type)
{
	this->SetDisplayName(display_name);
	this->SetControlName(control_name);

	this->SetWidth(w);
	this->SetHeight(h);

	this->SetColor(color);
	
	this->waiting_for_key = false;
	this->keybox_type = type;
	this->key = 999;

	this->SetControlType(Control_KeyBox);
}

void MenuKeyBox::SetKey(int k)
{
	if (k == VK_END)
	{
		this->waiting_for_key = false;
	}
	else {

		std::string key_name = "";
		switch (keybox_type)
		{
		case KeyType::KeyType_FULL:
			key_name = MenuUtils::VirtualToKey(k);
			break;
		case KeyType::KeyType_ONLY_NUMBERS:
			key_name = MenuUtils::VirtualToNumber(k);
			break;
		case KeyType::KeyType_ONLY_CHARACTERS:
			key_name = MenuUtils::VirtualToCharacter(k);
			break;
		default:
			break;
		}

		if (key_name.length() > 0)
		{
			this->key_string = key_name;
			this->key = k;
		}
		else {
			this->key = 9999;
			this->key_string = "";
		}
		this->waiting_for_key = false;
	}
}

int MenuKeyBox::GetKey()
{
	return this->key;
}

bool MenuKeyBox::WaitingForKey()
{
	return this->waiting_for_key;
}