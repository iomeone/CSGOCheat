#include "Menu.h"

POINT MenuUtils::GetMousePosition()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(MenuValues::hwnd, &p);
	return p;
}

bool MenuUtils::IsMouseInArea(int x, int y, int w, int h)
{
	POINT p = GetMousePosition();

	if (p.x >= x && p.x <= (x + w) && p.y >= y && p.y <= (y + h))
	{
		return true;
	}
	return false;
}

#include <fstream>
#include <istream>
#include <string>

std::vector<std::string> split(const std::string& s, const std::string& delimiter, const bool& removeEmptyEntries = false)
{
	std::vector<std::string> tokens;

	for (size_t start = 0, end; start < s.length(); start = end + delimiter.length())
	{
		size_t position = s.find(delimiter, start);
		end = position != std::string::npos ? position : s.length();

		std::string token = s.substr(start, end - start);
		if (!removeEmptyEntries || !token.empty())
		{
			tokens.push_back(token);
		}
	}

	if (!removeEmptyEntries &&
		(s.empty() || s.size() >= delimiter.size() && s.substr(s.size() - delimiter.size()) == delimiter))
	{
		tokens.push_back("");
	}

	return tokens;
}

#include "SkinChanger.h"

void MenuUtils::LoadConfig(int num)
{
	num += 1;

	std::string path = game_interfaces::Engine->GetGameDirectory();
	path += "\\Preset " + std::to_string(num) + ".dominance_skincfg";
	std::ifstream file_stream(path);

	if (file_stream.good())
	{
		for (std::string line; getline(file_stream, line); )
		{
			std::vector<std::string> Split = split(line, "-");

			if (Split[0].length() > 0 || Split[1].length() > 0)
			{
				SkinChanger::SetSkin(std::atoi(Split[0].c_str()), std::atoi(Split[1].c_str()), std::atoi(Split[2].c_str()));
			}
		}

		if (game_interfaces::Engine->InGame())
		{
			entity* local = game_interfaces::EntityList->GetClientEntity(game_interfaces::Engine->GetLocalPlayer());
			if (local)
			{
				if (local->m_iHealth() > 0 && !local->IsDormant())
				{
					baseweapon* curr_weapon = local->GetWeapon();
					if (curr_weapon)
					{
						for (SkinWeapon sw : SkinChanger::SkinWeapons)
						{
							if (sw.WeaponID == curr_weapon->m_iItemDefinitionIndex())
							{
								MenuControls::v_skins_seed->Set(std::to_string(sw.Seed));
								MenuControls::v_skins_paint_id->Set(std::to_string(sw.PaintKit));

								SkinChanger::Update(curr_weapon);
							}
						}
					}
				}
			}
		}
	}

	

	path = game_interfaces::Engine->GetGameDirectory();
	path += "\\Preset " + std::to_string(num) + ".dominance_cfg";
	file_stream = std::ifstream(path);

	if (file_stream.good()) 
	{
		for (std::string line; getline(file_stream, line); )
		{
			std::vector<std::string> Split = split(line, "-");

			if (line.length() > 0 && Split.size() > 1)
			{
				if (Split[0].length() > 0 && Split[1].length() > 0)
				{
					for (MenuFrame* frame : MenuValues::frames)
					{
						for (MenuTab* tab : frame->GetTabs())
						{
							for (MenuGroup* group : tab->GetGroups())
							{
								if (group->GetTabs().size() > 0)
								{
									for (MenuGroupTab& group_tab : group->GetTabs())
									{
										for (MenuControl* control : group_tab.controls)
										{
											if (!strcmp(control->GetControlName().c_str(), Split[0].c_str()))
											{
												MenuCheckBox* checkbox = (MenuCheckBox*)control;
												MenuComboBox* combobox = (MenuComboBox*)control;
												MenuSelectionBox* selectionbox = (MenuSelectionBox*)control;
												MenuSlider* slider = (MenuSlider*)control;
												MenuInputBox* inputbox = (MenuInputBox*)control;
												MenuKeyBox* keybox = (MenuKeyBox*)control;

												switch (control->GetControlType())
												{
												case Control_CheckBox:
													if (checkbox->GetState() != (std::stoi(Split[1]) == 1 ? true : false))
													{
														checkbox->LeftPressEvent();
													}
													if (checkbox->IsUsingColor())
													{
														checkbox->SetCustomColor(Color(std::stoi(Split[2]), std::stoi(Split[3]), std::stoi(Split[4])));
													}
													break;
												case Control_ComboBox:
													combobox->SetSelectedItem(std::stoi(Split[1]));
													break;
												case Control_SelectionBox:
													selectionbox->RemoveSelectedItems();
													for (std::string a : Split)
													{
														if (!(!strcmp(control->GetControlName().c_str(), a.c_str())))
														{
															int v = std::stoi(a);
															if (!selectionbox->IsSelectedItem(v))
															{
																selectionbox->SetSelectedItem(v);
															}
														}
													}
													break;
												case Control_Slider:
													slider->SetValue(std::stoi(Split[1]));
													break;
												case Control_InputBox:
													inputbox->Set(Split[1]);
													break;
												case Control_KeyBox:
													keybox->SetKey(std::atoi(Split[1].c_str()));
													break;
												default:
													break;
												}
											}
										}
									}
								}
								else {
									for (MenuControl* control : group->GetControls())
									{
										if (!strcmp(control->GetControlName().c_str(), Split[0].c_str()))
										{
											MenuCheckBox* checkbox = (MenuCheckBox*)control;
											MenuComboBox* combobox = (MenuComboBox*)control;
											MenuSelectionBox* selectionbox = (MenuSelectionBox*)control;
											MenuSlider* slider = (MenuSlider*)control;
											MenuInputBox* inputbox = (MenuInputBox*)control;
											MenuKeyBox* keybox = (MenuKeyBox*)control;

											switch (control->GetControlType())
											{
											case Control_CheckBox:
												if (checkbox->GetState() != (std::stoi(Split[1]) == 1 ? true : false))
												{
													checkbox->LeftPressEvent();
												}
												if (checkbox->IsUsingColor() && Split.size() > 3)
												{
													checkbox->SetCustomColor(Color(std::stoi(Split[2]), std::stoi(Split[3]), std::stoi(Split[4])));
												}
												break;
											case Control_ComboBox:
												combobox->SetSelectedItem(std::stoi(Split[1]));
												break;
											case Control_SelectionBox:
												selectionbox->RemoveSelectedItems();
												for (std::string a : Split)
												{
													if (!(!strcmp(control->GetControlName().c_str(), a.c_str())))
													{
														int v = std::stoi(a);
														if (!selectionbox->IsSelectedItem(v))
														{
															selectionbox->SetSelectedItem(v);
														}
													}
												}
												break;
											case Control_Slider:
												slider->SetValue(std::stoi(Split[1]));
												break;
											case Control_InputBox:
												inputbox->Set(Split[1]);
												break;
											case Control_KeyBox:
												keybox->SetKey(std::atoi(Split[1].c_str()));
												break;
											default:
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	file_stream.close();
}

void MenuUtils::SaveConfig(int num)
{
	num += 1;

	std::string path = game_interfaces::Engine->GetGameDirectory();
	path += "\\Preset " + std::to_string(num) + ".dominance_skincfg";
	std::ofstream file_stream(path);

	if (file_stream.good())
	{
		remove(path.c_str()); 
	}

	for (SkinWeapon sw : SkinChanger::SkinWeapons)
	{
		std::string result;
		result += std::to_string(sw.WeaponID);
		result += "-";
		result += std::to_string(sw.PaintKit);
		result += "-";
		result += std::to_string(sw.Seed);
		file_stream << result << std::endl;
	}

	path = game_interfaces::Engine->GetGameDirectory();
	path += "\\Preset " + std::to_string(num) + ".dominance_cfg";
	file_stream = std::ofstream(path);

	if (file_stream.good())
	{
		remove(path.c_str());
	}

	for (MenuFrame* frame : MenuValues::frames)
	{
		for (MenuTab* tab : frame->GetTabs())
		{
			for (MenuGroup* group : tab->GetGroups())
			{
				if (group->GetTabs().size() > 0)
				{
					for (MenuGroupTab& group_tab : group->GetTabs())
					{
						for (MenuControl* control : group_tab.controls)
						{
							MenuCheckBox* checkbox = (MenuCheckBox*)control;
							MenuComboBox* combobox = (MenuComboBox*)control;
							MenuSelectionBox* selectionbox = (MenuSelectionBox*)control;
							MenuSlider* slider = (MenuSlider*)control;
							MenuInputBox* inputbox = (MenuInputBox*)control;
							MenuKeyBox* keybox = (MenuKeyBox*)control;

							std::string result = "";

							switch (control->GetControlType())
							{
							case Control_CheckBox:
								result += checkbox->GetControlName();
								result += "-";
								result += (checkbox->GetState() ? "1" : "0");
								if (checkbox->IsUsingColor())
								{
									result += "-";
									result += std::to_string(checkbox->GetCustomColor().r());
									result += "-";
									result += std::to_string(checkbox->GetCustomColor().g());
									result += "-";
									result += std::to_string(checkbox->GetCustomColor().b());
								}
								break;
							case Control_ComboBox:
								result += combobox->GetControlName();
								result += "-";
								result += std::to_string(combobox->GetSelectedItem());
								break;
							case Control_SelectionBox:
								result += selectionbox->GetControlName();
								for (int a : selectionbox->GetSelectedItems())
								{
									result += "-";
									result += std::to_string(a);
								}
								break;
							case Control_Slider:
								result += slider->GetControlName();
								result += "-";
								result += std::to_string(slider->GetValue());
								break;
							case Control_InputBox:
								result += inputbox->GetControlName();
								result += "-";
								result += inputbox->GetInput();
								break;
							case Control_KeyBox:
								result += keybox->GetControlName();
								result += "-";
								result += std::to_string(keybox->GetKey());
								break;
							default:
								break;
							}

							if (result.length() > 0)
							{
								file_stream << result << std::endl;
							}
						}
					}
				}
				else {
					for (MenuControl* control : group->GetControls())
					{
						MenuCheckBox* checkbox = (MenuCheckBox*)control;
						MenuComboBox* combobox = (MenuComboBox*)control;
						MenuSelectionBox* selectionbox = (MenuSelectionBox*)control;
						MenuSlider* slider = (MenuSlider*)control;
						MenuInputBox* inputbox = (MenuInputBox*)control;
						MenuKeyBox* keybox = (MenuKeyBox*)control;

						std::string result = "";

						switch (control->GetControlType())
						{
						case Control_CheckBox:
							result += checkbox->GetControlName();
							result += "-";
							result += (checkbox->GetState() ? "1" : "0");
							if (checkbox->IsUsingColor())
							{
								result += "-";
								result += std::to_string(checkbox->GetCustomColor().r());
								result += "-";
								result += std::to_string(checkbox->GetCustomColor().g());
								result += "-";
								result += std::to_string(checkbox->GetCustomColor().b());
							}
							break;
						case Control_ComboBox:
							result += combobox->GetControlName();
							result += "-";
							result += std::to_string(combobox->GetSelectedItem());
							break;
						case Control_SelectionBox:
							result += selectionbox->GetControlName();
							for (int a : selectionbox->GetSelectedItems())
							{
								result += "-";
								result += std::to_string(a);
							}
							break;
						case Control_Slider:
							result += slider->GetControlName();
							result += "-";
							result += std::to_string(slider->GetValue());
							break;
						case Control_InputBox:
							result += inputbox->GetControlName();
							result += "-";
							result += inputbox->GetInput();
							break;
						case Control_KeyBox:
							result += keybox->GetControlName();
							result += "-";
							result += std::to_string(keybox->GetKey());
							break;
						default:
							break;
						}

						if (result.length() > 0)
						{
							file_stream << result << std::endl;
						}
					}
				}
			}
		}
	}
	file_stream.close();
}






std::string MenuUtils::VirtualToNumber(int key)
{
	switch (key)
	{
	case 0x30:
		return "0";
		break;
	case 0x31:
		return "1";
		break;
	case 0x32:
		return "2";
		break;
	case 0x33:
		return "3";
		break;
	case 0x34:
		return "4";
		break;
	case 0x35:
		return "5";
		break;
	case 0x36:
		return "6";
		break;
	case 0x37:
		return "7";
		break;
	case 0x38:
		return "8";
		break;
	case 0x39:
		return "9";
		break;
	default:
		return "";
		break;
	}
	return "";
}

std::string MenuUtils::VirtualToCharacter(int key)
{
	switch (key)
	{
	case 0x41:
		return "A";
		break;
	case 0x42:
		return "B";
		break;
	case 0x43:
		return "C";
		break;
	case 0x44:
		return "D";
		break;
	case 0x45:
		return "E";
		break;
	case 0x46:
		return "F";
		break;
	case 0x47:
		return "G";
		break;
	case 0x48:
		return "H";
		break;
	case 0x49:
		return "I";
		break;
	case 0x4A:
		return "J";
		break;
	case 0x4B:
		return "K";
		break;
	case 0x4C:
		return "L";
		break;
	case 0x4D:
		return "M";
		break;
	case 0x4E:
		return "N";
		break;
	case 0x4F:
		return "O";
		break;
	case 0x50:
		return "P";
		break;
	case 0x51:
		return "Q";
		break;
	case 0x52:
		return "R";
		break;
	case 0x53:
		return "S";
		break;
	case 0x54:
		return "T";
		break;
	case 0x55:
		return "U";
		break;
	case 0x56:
		return "V";
		break;
	case 0x57:
		return "W";
		break;
	case 0x58:
		return "X";
		break;
	case 0x59:
		return "Y";
		break;
	case 0x5A:
		return "Z";
		break;
	case VK_XBUTTON1:
		return "X1";
		break;
	case VK_XBUTTON2:
		return "X2";
		break;
	default:
		return "";
		break;
	}
	return "";
}

std::string MenuUtils::VirtualToKey(int key)
{
	switch (key)
	{
	case 0x30:
		return "0";
		break;
	case 0x31:
		return "1";
		break;
	case 0x32:
		return "2";
		break;
	case 0x33:
		return "3";
		break;
	case 0x34:
		return "4";
		break;
	case 0x35:
		return "5";
		break;
	case 0x36:
		return "6";
		break;
	case 0x37:
		return "7";
		break;
	case 0x38:
		return "8";
		break;
	case 0x39:
		return "9";
		break;
	case 0x41:
		return "A";
		break;
	case 0x42:
		return "B";
		break;
	case 0x43:
		return "C";
		break;
	case 0x44:
		return "D";
		break;
	case 0x45:
		return "E";
		break;
	case 0x46:
		return "F";
		break;
	case 0x47:
		return "G";
		break;
	case 0x48:
		return "H";
		break;
	case 0x49:
		return "I";
		break;
	case 0x4A:
		return "J";
		break;
	case 0x4B:
		return "K";
		break;
	case 0x4C:
		return "L";
		break;
	case 0x4D:
		return "M";
		break;
	case 0x4E:
		return "N";
		break;
	case 0x4F:
		return "O";
		break;
	case 0x50:
		return "P";
		break;
	case 0x51:
		return "Q";
		break;
	case 0x52:
		return "R";
		break;
	case 0x53:
		return "S";
		break;
	case 0x54:
		return "T";
		break;
	case 0x55:
		return "U";
		break;
	case 0x56:
		return "V";
		break;
	case 0x57:
		return "W";
		break;
	case 0x58:
		return "X";
		break;
	case 0x59:
		return "Y";
		break;
	case 0x5A:
		return "Z";
		break;
	case VK_XBUTTON1:
		return "X1";
		break;
	case VK_XBUTTON2:
		return "X2";
		break;
	default:
		return "";
		break;
	}
	return "";
}
