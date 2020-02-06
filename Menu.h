#pragma once

#include "Control.h"
#include "CheckBox.h"
#include "SelectionBox.h"
#include "Slider.h"
#include "ComboBox.h"
#include "InputBox.h"
#include "KeyBox.h"

class MenuCheckBox;
class MenuSelectionBox;
class MenuSlider;
class MenuComboBox;
class MenuInputBox;
class MenuKeyBox;

namespace MenuControls
{
	//--------------------------------------------//
	extern MenuCheckBox* a_master_switch;
	extern MenuSelectionBox* a_hitboxes;
	extern MenuCheckBox* a_silent;
	extern MenuCheckBox* a_shoot;
	extern MenuCheckBox* a_autoscope;
	extern MenuCheckBox* a_autowall;
	extern MenuCheckBox* a_backtrack;
	extern MenuCheckBox* a_ignoreteams;
	extern 	MenuSlider*	a_pointscale;
	extern MenuSlider*   a_multipoint;
	extern MenuSlider*   a_mindamage;
	extern MenuSlider*   a_hitchance;
	extern MenuSelectionBox* a_hitscan_settings;

	extern MenuCheckBox* a_antiaim;
	extern MenuComboBox* a_antiaim_stand_pitch;
	extern MenuComboBox* a_antiaim_stand_yaw;
	extern MenuSlider*   a_antiaim_stand_yaw_addition;

	extern MenuComboBox* a_antiaim_ground_pitch;
	extern MenuComboBox* a_antiaim_ground_yaw;
	extern MenuSlider*   a_antiaim_ground_yaw_addition;

	extern MenuComboBox* a_antiaim_air_pitch;
	extern MenuComboBox* a_antiaim_air_yaw;
	extern MenuSlider*   a_antiaim_air_yaw_addition;

	extern MenuCheckBox* a_antiaim_stand_freestanding;
	extern MenuCheckBox* a_antiaim_manual;
	extern MenuSlider*	a_antiaim_stand_break_value;
	extern MenuComboBox* a_antiaim_stand_break_mode;
	extern MenuCheckBox* a_antiaim_at_target;
	extern MenuCheckBox* a_antiaim_anti_lby_air;
	extern MenuCheckBox* a_antiaim_desync;

	extern MenuCheckBox* a_norecoil;
	extern MenuCheckBox* a_nospread;
	extern MenuCheckBox* a_autostop;
	extern MenuCheckBox* a_autorevolver;

	extern MenuCheckBox* a_resolver;
	extern 	MenuCheckBox* a_resolver_resolve_all;
	extern MenuComboBox* a_resolver_overwrite_pitch;
	extern MenuComboBox* a_resolver_mode;
	extern MenuSlider* a_resolver_bruteforce_angle_step;
	extern MenuSlider* a_resolver_bodyaim_after_shots;
	extern MenuSelectionBox* a_resolver_bodyaim_if;

	//--------------------------------------------//
	extern MenuCheckBox* v_players_esp_enable;
	extern MenuCheckBox* v_players_esp_dormant;
	extern MenuComboBox* v_players_esp_health;
	extern MenuCheckBox* v_players_esp_weapon;
	extern MenuCheckBox* v_players_esp_name;
	extern MenuCheckBox* v_players_esp_sound;

	extern MenuCheckBox* v_players_esp_ally_enable;
	extern MenuCheckBox* v_players_esp_ally_chams;
	extern MenuCheckBox* v_players_esp_ally_box;
	extern MenuCheckBox* v_players_esp_ally_glow;

	extern MenuCheckBox* v_players_esp_enemy_enable;
	extern MenuCheckBox* v_players_esp_enemy_chams;
	extern MenuCheckBox* v_players_esp_enemy_box;
	extern MenuCheckBox* v_players_esp_enemy_glow;

	extern MenuComboBox* v_players_esp_chams_mode;
	extern MenuComboBox* v_players_esp_glow_mode;
	extern MenuSlider*   v_players_esp_glow_bloom;
	extern MenuCheckBox* v_players_esp_info;
	extern MenuCheckBox* v_players_esp_offscreen;
	extern MenuSlider*   v_players_esp_offscreen_distance;
	extern MenuComboBox* v_players_esp_drawbacktrack;
	//--------------------------------------------//
	extern MenuCheckBox* v_other_hitmarker;
	extern MenuCheckBox* v_other_crosshair;
	extern MenuCheckBox* v_other_noscope;
	extern MenuCheckBox* v_other_postprocess;
	extern MenuCheckBox* v_other_projectiles;
	extern 	MenuCheckBox* v_other_nightmode;
	extern 	MenuSlider* v_other_nightmode_value;
	extern 	MenuCheckBox* v_other_antiaimghost_fake;
	extern 	MenuCheckBox* v_other_antiaimghost_lby;
	extern 	MenuCheckBox* v_other_fakelag_ghost;
	extern MenuCheckBox* v_other_grenade_prediction;
	extern 	MenuComboBox* v_other_nosmoke;
	//--------------------------------------------//
	extern MenuCheckBox* v_skins_enable;
	extern MenuComboBox* v_skins_knife_model;
	extern MenuInputBox* v_skins_paint_id;
	extern MenuInputBox* v_skins_seed;
	//--------------------------------------------//
	extern MenuCheckBox* m_debug;
	extern 	MenuCheckBox* m_clantag;
	extern 	MenuCheckBox* m_bunnyhop;
	extern MenuSlider* m_bunnyhop_delay;
	extern 	MenuCheckBox* m_fakewalk;
	extern 	MenuKeyBox* m_thirdperson_bind;
	extern 	MenuKeyBox* m_circlestrafer_bind;
	extern 	MenuSlider*	m_fakelatency;
	extern 	MenuComboBox* m_fakelag_mode;
	extern 	MenuSlider*	m_fakelag_max;
	//--------------------------------------------//
	extern MenuComboBox* m_configs;
	extern 	MenuCheckBox* m_config_save;
	extern 	MenuCheckBox* m_config_load;
	//--------------------------------------------//
};

// External includes
#include <vector>
#include <Windows.h>

// Menu includes
#include "Frame.h"
#include "Group.h"
#include "Tab.h"

namespace MenuValues
{
	extern std::vector<MenuFrame*> frames;
	extern HWND hwnd;
	extern bool LeftMousePressed;
	extern bool MenuOpen;
}

namespace MenuUtils
{
	POINT GetMousePosition();
	std::string VirtualToKey(int key);
	std::string VirtualToNumber(int key);
	std::string VirtualToCharacter(int key);
	bool IsMouseInArea(int x, int y, int w, int h);

	void LoadConfig(int num);
	void SaveConfig(int num);
}

namespace Menu
{
	void Initialize(HWND hwnd);

	void HandleLeftMouse();
	void HandleKeyInput(int key);

	void ControlAreaUpdate(MenuControl* control, int frame_x, int frame_y, int group_x, int group_y);
	void ControlOutAreaUpdate(MenuControl* control, int frame_x, int frame_y, int group_x, int group_y, int group_w, int group_h);
	
	void UpdateAnimations();
	void UpdateConfigs();
	void Render();
	void Update();
}