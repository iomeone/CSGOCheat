#include "Menu.h"

void Menu::ControlAreaUpdate(MenuControl* control, int frame_x, int frame_y, int group_x, int group_y)
{
	MenuSlider* slider = (MenuSlider*)control;
	MenuComboBox* combo_box = (MenuComboBox*)control;
	MenuSelectionBox* selection_box = (MenuSelectionBox*)control;
	MenuKeyBox* key_box = (MenuKeyBox*)control;
	MenuInputBox* input_box = (MenuInputBox*)control;

	switch (control->GetControlType())
	{
	case Control_Slider:
		if (!MenuValues::LeftMousePressed || slider->IsSliderMoving())
		{
			int delta = MenuUtils::GetMousePosition().x - (group_x + slider->GetX());
			if (delta < 0)
				delta = 0;

			slider->SetMouseDelta(delta);
			slider->LeftPressEvent();
			slider->SetSliderBoolean(true);
		}
		break;
	case Control_ComboBox:
		if (!MenuValues::LeftMousePressed)
		{
			if (combo_box->IsOpen())
			{
				int y = 0;
				int id = 0;
				for (std::string i : combo_box->GetItems())
				{
					bool over_item = MenuUtils::IsMouseInArea(group_x + combo_box->GetX(), group_y + combo_box->GetY() + y, combo_box->GetWidth(), combo_box->GetHeightPerItem());
					if (over_item)
					{
						combo_box->SetSelectedItem(id);
					}

					id += 1;
					y += combo_box->GetHeightPerItem();
				}
				combo_box->LeftPressEvent();
			}
			else {
				combo_box->LeftPressEvent();
			}
		}
		break;
	case Control_SelectionBox:
		if (!MenuValues::LeftMousePressed)
		{
			if (!selection_box->IsOpen())
			{
				control->LeftPressEvent();
			}
			else {
				int y = 0;
				int id = 0;
				for (std::string i : selection_box->GetItems())
				{
					bool over_item = MenuUtils::IsMouseInArea(group_x + selection_box->GetX(), group_y + selection_box->GetY() + y, selection_box->GetWidth(), selection_box->GetHeightPerItem());
					if (over_item)
					{
						selection_box->SetSelectedItem(id);
					}

					id += 1;
					y += selection_box->GetHeightPerItem();
				}
			}
		}
		break;
	case Control_CheckBox:
		if (!MenuValues::LeftMousePressed)
		{
			control->LeftPressEvent();
		}
		break;
	case Control_KeyBox:
		if (!MenuValues::LeftMousePressed)
		{
			if (!key_box->WaitingForKey())
			{
				control->LeftPressEvent();
			}
		}
	case Control_InputBox:
		if (!MenuValues::LeftMousePressed)
		{
			if (!input_box->WaitingForInput())
			{
				control->LeftPressEvent();
			}
		}
		break;
	}
}

void Menu::ControlOutAreaUpdate(MenuControl* control, int frame_x, int frame_y, int group_x, int group_y, int group_w, int group_h)
{
	MenuCheckBox* check_box = (MenuCheckBox*)control;
	MenuSlider* slider = (MenuSlider*)control;
	MenuComboBox* combo_box = (MenuComboBox*)control;
	MenuSelectionBox* selection_box = (MenuSelectionBox*)control;
	MenuKeyBox* key_box = (MenuKeyBox*)control;
	MenuInputBox* input_box = (MenuInputBox*)control;

	switch (control->GetControlType())
	{
	case Control_CheckBox:
		if (!MenuValues::LeftMousePressed)
		{
			check_box->OutsideLeftPressEvent(group_x, group_y, group_w, group_h);
		}
		break;
	case Control_Slider:
		if (slider->IsSliderMoving())
		{
			int delta = MenuUtils::GetMousePosition().x - (group_x + slider->GetX());
			if (delta < 0)
				delta = 0;

			slider->SetMouseDelta(delta);
			slider->LeftPressEvent();
		}
		break;
	case Control_ComboBox:
		if (!MenuValues::LeftMousePressed)
		{
			if (combo_box->IsOpen())
			{
				int y = 0;
				int id = 0;
				for (std::string i : combo_box->GetItems())
				{
					bool over_item = MenuUtils::IsMouseInArea(group_x + combo_box->GetX(), group_y + combo_box->GetY() + y, combo_box->GetWidth(), combo_box->GetHeightPerItem());
					if (over_item)
					{
						combo_box->SetSelectedItem(id);
					}

					id += 1;
					y += combo_box->GetHeightPerItem();
				}

				combo_box->LeftPressEvent();
			}
		}
		break;
	case Control_SelectionBox:
		if (!MenuValues::LeftMousePressed)
		{
			if (selection_box->IsOpen())
			{
				bool changed_value = false;

				int y = 0;
				int id = 0;
				for (std::string i : selection_box->GetItems())
				{
					bool over_item = MenuUtils::IsMouseInArea(group_x + selection_box->GetX(), group_y + selection_box->GetY() + y, selection_box->GetWidth(), selection_box->GetHeightPerItem());
					if (over_item)
					{
						changed_value = true;
						selection_box->SetSelectedItem(id);
					}

					id += 1;
					y += selection_box->GetHeightPerItem();
				}


				if(!changed_value)
				{
					selection_box->LeftPressEvent();
				}
			}
		}
		break;
	case Control_KeyBox:
		if (!MenuValues::LeftMousePressed)
		{
			if (key_box->WaitingForKey())
			{
				control->LeftPressEvent();
			}
		}
		break;
	case Control_InputBox:
		if (!MenuValues::LeftMousePressed)
		{
			if (input_box->WaitingForInput())
			{
				control->LeftPressEvent();
			}
		}
		break;
	default:
		break;
	}
}

void Menu::HandleLeftMouse()
{
	for (MenuFrame* frame : MenuValues::frames)
	{
		float frame_x = frame->GetX();
		float frame_y = frame->GetY() + frame->GetTitleBarHeight();

		bool in_titlebar_area = MenuUtils::IsMouseInArea(frame->GetX(), frame->GetY(), frame->GetWidth(), frame->GetTitleBarHeight());
		bool pressing_leftmouse_button = GetAsyncKeyState(VK_LBUTTON);

		if (!pressing_leftmouse_button)
		{
			int calc_x = frame->GetWidth() - ((frame->GetX() + frame->GetWidth()) - MenuUtils::GetMousePosition().x);
			int calc_y = frame->GetY() - MenuUtils::GetMousePosition().y;
			frame->SetLastMouseXDiff(calc_x);
			frame->SetLastMouseYDiff(calc_y);
			frame->SetMovingBoolean(false);
		}

		if (!in_titlebar_area && !frame->IsMoving())
		{
			int tab_id = 0;
			int tab_y = 0;
			for (MenuTab* tab : frame->GetTabs())
			{
				bool in_tabs_area = MenuUtils::IsMouseInArea(frame->GetX() - tab->GetWidth(), frame->GetY(), tab->GetWidth(), frame->GetHeight());
				if (in_tabs_area)
				{
					int tab_y_ = frame_y + tab_y;
					int tab_x = frame->GetX() - tab->GetWidth();
					int tab_w = tab->GetWidth();
					if (MenuUtils::IsMouseInArea(tab_x, tab_y_, tab_w, tab->GetHeight()) && pressing_leftmouse_button && !MenuValues::LeftMousePressed)
					{
						frame->SetCurrentTab(tab_id);
					}
				}
				else {
					if (tab_id == frame->GetCurrentTab())
					{

						bool cancel_click_for_some_controls = false;
						for (MenuGroup* group : tab->GetGroups())
						{
							for (MenuControl* control : group->GetControls())
							{
								if (control->GetControlType() == Control_ComboBox)
								{
									MenuComboBox* combo_box = (MenuComboBox*)control;
									if (combo_box->IsOpen())
									{
										cancel_click_for_some_controls = true;
									}
								}
								if (control->GetControlType() == Control_SelectionBox)
								{
									MenuSelectionBox* selection_box = (MenuSelectionBox*)control;
									if (selection_box->IsOpen())
									{
										cancel_click_for_some_controls = true;
									}
								}
								if (control->GetControlType() == Control_CheckBox)
								{
									MenuCheckBox* check_box = (MenuCheckBox*)control;
									if (check_box->IsColorBoxOpen() && check_box->IsUsingColor())
									{
										cancel_click_for_some_controls = true;
									}
								}
							}
						}


						for (MenuGroup* group : tab->GetGroups())
						{
							int group_x = frame_x + group->GetX();
							int group_y = frame_y + group->GetY();
						
							if (group->GetTabs().size() > 0)
							{
								group_y += 12;
							
								float width_per_tab = ((float)group->GetWidth() / (float)group->GetTabs().size());

								int next_x = 0;
								int tab_height = 20;

								for (MenuGroupTab group_tab : group->GetTabs())
								{
									float MiddleX = group_x + next_x + (width_per_tab / 2) - (RenderUtils::GetTextLenght(group_tab.name, RenderUtils::Normal_Bold_font) / 2);
									float MiddleY = group_y + (tab_height / 2) - (RenderUtils::GetTextHeight(group_tab.name, RenderUtils::Normal_Bold_font) / 2);

									bool hovering_tab = MenuUtils::IsMouseInArea(group_x + next_x, group_y, width_per_tab, tab_height);
									if (hovering_tab)
									{
										if (pressing_leftmouse_button)
										{
											group->SetCurrentTab(group_tab.id);
										}
									}

									next_x += width_per_tab;
								}
								group_y += 15;
							}

							group_y += 15;

							if (pressing_leftmouse_button)
							{
								int next_y = 0;
								for (MenuControl* control : group->GetControls())
								{
									MenuCheckBox* check_box = (MenuCheckBox*)control;
									MenuComboBox* combo_box = (MenuComboBox*)control;
									MenuSelectionBox* selection_box = (MenuSelectionBox*)control;


									if (!cancel_click_for_some_controls
									|| cancel_click_for_some_controls && control->GetControlType() == Control_ComboBox && combo_box->IsOpen()
									|| cancel_click_for_some_controls && control->GetControlType() == Control_SelectionBox && selection_box->IsOpen()
									|| cancel_click_for_some_controls && control->GetControlType() == Control_CheckBox && check_box->IsUsingColor() && check_box->IsColorBoxOpen())
									{
										if (control->IsMouseOver(group_x, group_y))
										{
											// Over the control
											ControlAreaUpdate(control, frame_x, frame_y, group_x, group_y);
										}
										else {
											// Not over the control
											ControlOutAreaUpdate(control, frame_x, frame_y, group_x, group_y, group->GetWidth(), group->GetHeight());
										}
									}
									next_y += (control->GetHeight() + 5);
								}
							}
							else {
								for (MenuControl* control : group->GetControls())
								{
									control->LeftReleaseEvent();
								}
							}
						}
					}
				}
				tab_id += 1;
				tab_y += tab->GetHeight() + 5;
			}
		}
		else {
			if (pressing_leftmouse_button)
			{
				if (!MenuValues::LeftMousePressed || frame->IsMoving())
				{
					frame->SetMovingBoolean(true);
					frame->UpdatePositions();
				}
			}
		}
	}
}

void Menu::HandleKeyInput(int key)
{
	for (MenuFrame* frame : MenuValues::frames)
	{
		int tab_id = 0;
		for (MenuTab* tab : frame->GetTabs())
		{
			if (tab_id == frame->GetCurrentTab())
			{
				for (MenuGroup* group : tab->GetGroups())
				{
					for (MenuControl* control : group->GetControls())
					{
						MenuKeyBox* key_box = (MenuKeyBox*)control;
						MenuInputBox* input_box = (MenuInputBox*)control;

						switch (control->GetControlType())
						{
						case Control_KeyBox:
							if (key_box->WaitingForKey())
							{
								key_box->SetKey(key);
							}
							break;
						case Control_InputBox:
							if (input_box->WaitingForInput())
							{
								input_box->AddKey(key);
							}
							break;
						default:
							break;
						}
					}
				}
			}
			tab_id += 1;
		}
	}
}

void Menu::UpdateAnimations()
{
	for (MenuFrame* frame : MenuValues::frames)
	{
		int tab_size = 0;
		for (MenuTab* tab : frame->GetTabs())
		{
			tab_size = tab->GetWidth();
			break;
		}
		bool in_tabs_area = MenuUtils::IsMouseInArea(frame->GetX() - tab_size, frame->GetY(), tab_size, frame->GetHeight());
		if (in_tabs_area)
		{
			frame->IncreaseTabsAnimationValue();
		}
		else { frame->DecreaseTabsAnimationValue(); }
	}
}