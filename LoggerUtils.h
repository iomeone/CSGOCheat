#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include "Color.h"

struct CNotifyText
{
	Color clr;
	float liferemaining;
	std::string	text;
};

namespace Logger
{
	extern std::vector<CNotifyText> m_NotifyText;
	void Render();
	void Message(std::string s, Color color);

	void SaveToLog(std::string s);
}