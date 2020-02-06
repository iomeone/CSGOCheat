#pragma once

#include <string>

namespace SecurityUtils
{
	extern std::string username;
	void check_guard();
	void SetData();
	std::string get_session_id();
	std::string get_web_response(std::string domain, std::string web_get);
}