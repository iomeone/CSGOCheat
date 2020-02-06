#include "LoggerUtils.h"
#include "RenderUtils.h"
#include "InterfaceManager.h"

std::vector<CNotifyText> Logger::m_NotifyText;

void Logger::Message(std::string s, Color color)
{
	static ConVar* con_notifytime = game_interfaces::CVar->FindVar("con_notifytime");

	CNotifyText text;
	text.clr = color;
	text.liferemaining = con_notifytime->GetFloat();
	text.text = s;
	Logger::m_NotifyText.push_back(text);
	game_interfaces::CVar->ConsoleColorPrintf(color,s.c_str());
}



void Logger::Render()
{
	if (!RenderUtils::directx_device || !RenderUtils::Normal_font)
		return;

	int x = 8;
	int y = 5;

	int i = 0;
	for (CNotifyText &notify : m_NotifyText)
	{
		if (notify.liferemaining >= 0.1f && i < 10)
		{
			float alpha = 255.f;
			float fontTall = RenderUtils::GetTextHeight(notify.text.c_str(), RenderUtils::Normal_font) + 1;

			if (i == 0 && notify.liferemaining < 0.5 && notify.liferemaining >= 0.f)
			{
				float scale = notify.liferemaining / 0.5f;
				alpha = (255.f * scale);

				y -= (fontTall - (scale * fontTall));
			}

			if (notify.text.length() > 0)
			{
				try {
					wchar_t buffer[500];
					if (MultiByteToWideChar(CP_UTF8, 0, notify.text.c_str(), -1, buffer, 500) > 0)
					{
						RenderUtils::RenderText(x, y, Color(notify.clr.r(), notify.clr.g(), notify.clr.b(), alpha), buffer);
						y += fontTall;
					}
				}
				catch (int Error) {

				}
			}

			notify.liferemaining -= 0.01f;
			i += 1;
		}
	}

	if (m_NotifyText.size() > 0)
	{
		bool cancel = false;
		for (CNotifyText &notify : m_NotifyText)
		{
			if (notify.liferemaining > 0.f)
			{
				cancel = true;
			}
		}
		if (!cancel)
		{
			m_NotifyText.clear();
		}
	}
}

#include <windows.h>
#include <iostream>
#include <shlobj.h>
#include <fstream>  
#pragma comment(lib, "shell32.lib")

void Logger::SaveToLog(std::string s)
{
	char * userpath = getenv("USERPROFILE");
	std::string path;
	if (userpath != nullptr)
			path = std::string(userpath) + "\\Documents\\dominance_logfile.txt";

	std::ofstream log;
	log.open(path, std::ofstream::app);

	log << s.c_str() << "\n" << std::endl;

	log.close();
}