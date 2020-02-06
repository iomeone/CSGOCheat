#include "Menu.h"

#include "RenderUtils.h"
#include "LoggerUtils.h"

std::vector<MenuFrame*> MenuValues::frames;
HWND MenuValues::hwnd;
bool MenuValues::LeftMousePressed;
bool MenuValues::MenuOpen;
//--------------------------------------------//


MenuTab* aimbot_tab = new MenuTab("Aimbot", 40);
MenuTab* visuals_tab = new MenuTab("Visuals", 40);
MenuTab* other_tab = new MenuTab("Other", 40);

MenuFrame* new_frame = new MenuFrame("Dominance Beta", 50, 50, 600, 500, 30);

void Menu::Initialize(HWND hwnd)
{
	MenuValues::hwnd = hwnd;
	MenuValues::MenuOpen = false;

	//--------------------------------------------//
	MenuControls::a_master_switch = new MenuCheckBox("Enable aimbot", "aimbot_master_switch", 15, 15, Color(30, 60, 100));
	MenuControls::a_hitboxes = new MenuSelectionBox("Hitboxes", "aimbot_hitboxes", 100, 15, Color(30, 60, 100));
	MenuControls::a_silent = new MenuCheckBox("Silent", "aimbot_silent", 15, 15, Color(30, 60, 100));
	MenuControls::a_shoot = new MenuCheckBox("Shoot", "aimbot_shoot", 15, 15, Color(30, 60, 100));
	MenuControls::a_ignoreteams = new MenuCheckBox("Ignore Teams", "aimbot_ignoreteams", 15, 15, Color(30, 60, 100));
	MenuControls::a_autoscope = new MenuCheckBox("AutoScope", "aimbot_autoscope", 15, 15, Color(30, 60, 100));
	MenuControls::a_autowall = new MenuCheckBox("AutoWall", "aimbot_autowall", 15, 15, Color(30, 60, 100));
	MenuControls::a_backtrack = new MenuCheckBox("BackTrack", "aimbot_backtrack", 15, 15, Color(30, 60, 100));
	MenuControls::a_multipoint = new MenuSlider("MultiPoint/Hitbox", "aimbot_multipoint", 100, 15, 1, 0, 10, Color(30, 60, 100));
	MenuControls::a_pointscale = new MenuSlider("PointScale", "aimbot_pointscale", 100, 15, 50, 0, 100, Color(30, 60, 100));
	MenuControls::a_mindamage = new MenuSlider("Minimum Damage", "aimbot_mindamage", 100, 15, 5, 1, 100, Color(30, 60, 100));
	MenuControls::a_hitchance = new MenuSlider("HitChance", "aimbot_hitchance", 100, 15, 0, 0, 100, Color(30, 60, 100));
	MenuControls::a_hitscan_settings = new MenuSelectionBox("HitScan Priorities", "aimbot_hitscan_settings", 100, 15, Color(30, 60, 100));

	MenuControls::a_antiaim = new MenuCheckBox("Enable AntiAim", "aimbot_antiaim", 15, 15, Color(30, 60, 100));

	MenuControls::a_antiaim_stand_pitch = new MenuComboBox("AntiAim Pitch", "aimbot_antiaim_stand_pitch", 100, 15, Color(30, 60, 100));
	MenuControls::a_antiaim_stand_yaw = new MenuComboBox("AntiAim Real Yaw", "aimbot_antiaim_stand_yaw", 100, 15, Color(30, 60, 100));
	//MenuControls::a_antiaim_stand_yaw_fake = new MenuComboBox("AntiAim Fake Yaw", "aimbot_antiaim_stand_fake_yaw", 100, 15, Color(30, 60, 100));
	MenuControls::a_antiaim_stand_yaw_addition = new MenuSlider("Real Yaw Add", "aimbot_antiaim_stand_yaw_addition", 100, 15, 0, -179, 179, Color(30, 60, 100));
	//MenuControls::a_antiaim_stand_yaw_fake_addition = new MenuSlider("Fake Yaw Add", "aimbot_antiaim_stand_fake_yaw_addition", 100, 15, 0, -179, 179, Color(30, 60, 100));

	MenuControls::a_antiaim_ground_pitch = new MenuComboBox("AntiAim Pitch", "aimbot_antiaim_ground_pitch", 100, 15, Color(30, 60, 100));
	MenuControls::a_antiaim_ground_yaw = new MenuComboBox("AntiAim Real Yaw", "aimbot_antiaim_ground_yaw", 100, 15, Color(30, 60, 100));
	//MenuControls::a_antiaim_ground_yaw_fake = new MenuComboBox("AntiAim Fake Yaw", "aimbot_antiaim_ground_fake_yaw", 100, 15, Color(30, 60, 100));
	MenuControls::a_antiaim_ground_yaw_addition = new MenuSlider("Real Yaw Add", "aimbot_antiaim_ground_yaw_addition", 100, 15, 0, -179, 179, Color(30, 60, 100));
	//MenuControls::a_antiaim_ground_yaw_fake_addition = new MenuSlider("Fake Yaw Add", "aimbot_antiaim_ground_fake_yaw_addition", 100, 15, 0, -179, 179, Color(30, 60, 100));

	MenuControls::a_antiaim_air_pitch = new MenuComboBox("AntiAim Pitch", "aimbot_antiaim_air_pitch", 100, 15, Color(30, 60, 100));
	MenuControls::a_antiaim_air_yaw = new MenuComboBox("AntiAim Real Yaw", "aimbot_antiaim_air_yaw", 100, 15, Color(30, 60, 100));
	//MenuControls::a_antiaim_air_yaw_fake = new MenuComboBox("AntiAim Fake Yaw", "aimbot_antiaim_air_fake_yaw", 100, 15, Color(30, 60, 100));
	MenuControls::a_antiaim_air_yaw_addition = new MenuSlider("Real Yaw Add", "aimbot_antiaim_air_yaw_addition", 100, 15, 0, -179, 179, Color(30, 60, 100));
	//MenuControls::a_antiaim_air_yaw_fake_addition = new MenuSlider("Fake Yaw Add", "aimbot_antiaim_air_fake_yaw_addition", 100, 15, 0, -179, 179, Color(30, 60, 100));

	MenuControls::a_antiaim_stand_freestanding = new MenuCheckBox("FreeStanding", "aimbot_antiaim_free", 15, 15, Color(30, 60, 100));
	MenuControls::a_antiaim_desync = new MenuCheckBox("Desync", "aimbot_antiaim_stand_desync", 15, 15, Color(30, 60, 100));
	MenuControls::a_antiaim_manual = new MenuCheckBox("Manual", "aimbot_antiaim_manual", 15, 15, Color(30, 60, 100));
	MenuControls::a_antiaim_stand_break_value = new MenuSlider("Break Amount", "misc_antiaim_break_value", 100, 15, 150, 100, 179, Color(30, 60, 100));
	MenuControls::a_antiaim_stand_break_mode = new MenuComboBox("Breaker Mode", "aimbot_antiaim_breaker_mode", 100, 15, Color(30, 60, 100));
	MenuControls::a_antiaim_at_target = new MenuCheckBox("At Target", "aimbot_antiaim_at_target", 15, 15, Color(30, 60, 100));
	MenuControls::a_antiaim_anti_lby_air = new MenuCheckBox("No Ground", "aimbot_antiaim_no_ground", 15, 15, Color(30, 60, 100));

	MenuControls::a_norecoil = new MenuCheckBox("NoRecoil", "aimbot_norecoil", 15, 15, Color(30, 60, 100));
	MenuControls::a_nospread = new MenuCheckBox("NoSpread", "aimbot_nospread", 15, 15, Color(30, 60, 100));
	MenuControls::a_autostop = new MenuCheckBox("AutoStop", "aimbot_autostop", 15, 15, Color(30, 60, 100));
	MenuControls::a_autorevolver = new MenuCheckBox("AutoRevolver", "aimbot_autorevolver", 15, 15, Color(30, 60, 100));

	MenuControls::a_resolver = new MenuCheckBox("Enable Resolver", "aimbot_resolver", 15, 15, Color(30, 60, 100));
	MenuControls::a_resolver_resolve_all = new MenuCheckBox("Resolve All", "aimbot_resolver_resolve_all", 15, 15, Color(30, 60, 100));
	MenuControls::a_resolver_overwrite_pitch = new MenuComboBox("OverWrite Pitch", "aimbot_resolver_overwrite_pitch", 100, 15, Color(30, 60, 100));
	MenuControls::a_resolver_mode = new MenuComboBox("Resolver Mode", "aimbot_resolver_mode", 100, 15, Color(30, 60, 100));
	MenuControls::a_resolver_bruteforce_angle_step = new MenuSlider("Bruteforce Angle Step", "aimbot_resolver_bruteforce_angle_step", 100, 15, 45, 5, 180, Color(30, 60, 100));
	MenuControls::a_resolver_bodyaim_after_shots = new MenuSlider("Body After X Shots", "aimbot_resolver_bodyaim_after_shots", 100, 15, 0, 0, 5, Color(30, 60, 100));
	MenuControls::a_resolver_bodyaim_if = new MenuSelectionBox("BodyAim If", "aimbot_resolver_bodyaim_if", 100, 15, Color(30, 60, 100));
	//--------------------------------------------//
	MenuControls::v_players_esp_enable = new MenuCheckBox("Enable Visuals", "visual_esp_enable", 15, 15, Color(30, 60, 100));
	MenuControls::v_players_esp_dormant = new MenuCheckBox("Dormants", "visual_esp_dormant", 15, 15, Color(30, 60, 100));
	MenuControls::v_players_esp_health = new MenuComboBox("Health Bar", "visual_esp_healthbar", 100, 15, Color(30, 60, 100));
	MenuControls::v_players_esp_weapon = new MenuCheckBox("Weapon", "visual_esp_weapon", 15, 15, Color(30, 60, 100));
	MenuControls::v_players_esp_name = new MenuCheckBox("Name", "visual_esp_name", 15, 15, Color(30, 60, 100));
	MenuControls::v_players_esp_sound = new MenuCheckBox("Sound", "visual_esp_sound", 15, 15, Color(30, 60, 100));
	MenuControls::v_players_esp_info = new MenuCheckBox("Info", "visual_esp_info", 15, 15, Color(30, 60, 100));

	MenuControls::v_players_esp_ally_enable = new MenuCheckBox("Ally Enable", "visual_esp_ally_enable", 15, 15, Color(30, 60, 100));
	MenuControls::v_players_esp_ally_chams = new MenuCheckBox("Ally Chams", "visual_esp_ally_chams", 15, 15, Color(30, 60, 100), Color(255, 255, 255));
	MenuControls::v_players_esp_ally_box = new MenuCheckBox("Ally Box", "visual_esp_ally_box", 15, 15, Color(30, 60, 100), Color(255, 255, 255));
	MenuControls::v_players_esp_ally_glow = new MenuCheckBox("Ally Glow", "visual_esp_ally_glow", 15, 15, Color(30, 60, 100), Color(255, 255, 255));

	MenuControls::v_players_esp_enemy_enable = new MenuCheckBox("Enemy Enable", "visual_esp_enemy_enable", 15, 15, Color(30, 60, 100));
	MenuControls::v_players_esp_enemy_chams = new MenuCheckBox("Enemy Chams", "visual_esp_enemy_chams", 15, 15, Color(30, 60, 100), Color(255, 255, 255));
	MenuControls::v_players_esp_enemy_box = new MenuCheckBox("Enemy Box", "visual_esp_enemy_box", 15, 15, Color(30, 60, 100), Color(255, 255, 255));
	MenuControls::v_players_esp_enemy_glow = new MenuCheckBox("Enemy Glow", "visual_esp_enemy_glow", 15, 15, Color(30, 60, 100), Color(255, 255, 255));

	MenuControls::v_players_esp_chams_mode = new MenuComboBox("Chams Mode", "visual_esp_chams_mode", 100, 15, Color(30, 60, 100));
	MenuControls::v_players_esp_glow_mode = new MenuComboBox("Glow Mode", "visual_esp_glow_mode", 100, 15, Color(30, 60, 100));
	MenuControls::v_players_esp_glow_bloom = new MenuSlider("Glow Thickness", "visual_esp_glow_thickness", 100, 15, 50, 0, 100, Color(30, 60, 100));
	MenuControls::v_players_esp_offscreen = new MenuCheckBox("OffScreen", "visual_esp_offscreen", 15, 15, Color(30, 60, 100), Color(255, 255, 255));
	MenuControls::v_players_esp_offscreen_distance = new MenuSlider("OffScreen distance", "visual_esp_offscreen_distance", 100, 15, 200, 10, 500, Color(30, 60, 100));
	MenuControls::v_players_esp_drawbacktrack = new MenuComboBox("Draw BackTrack", "visual_esp_drawbacktrack", 100, 15, Color(30, 60, 100));
	//--------------------------------------------//
	MenuControls::v_other_hitmarker = new MenuCheckBox("HitMarker", "visual_other_hitmarker", 15, 15, Color(30, 60, 100), Color(255, 255, 255));
	MenuControls::v_other_crosshair = new MenuCheckBox("CrossHair", "visual_other_crosshair", 15, 15, Color(30, 60, 100), Color(255, 255, 255));
	MenuControls::v_other_noscope = new MenuCheckBox("Remove Scope", "visual_other_noscope", 15, 15, Color(30, 60, 100));
	MenuControls::v_other_postprocess = new MenuCheckBox("Remove PostProcess", "visual_other_prostprocess", 15, 15, Color(30, 60, 100));
	MenuControls::v_other_projectiles = new MenuCheckBox("Bullet Projectiles", "visual_other_projectiles", 15, 15, Color(30, 60, 100));
	MenuControls::v_other_nightmode = new MenuCheckBox("Nightmode", "visual_other_nightmode", 15, 15, Color(30, 60, 100));
	MenuControls::v_other_nightmode_value = new MenuSlider("Nightmode Value", "visual_other_nightmode_value", 100, 15, 15, 5, 95, Color(30, 60, 100));
	MenuControls::v_other_antiaimghost_fake = new MenuCheckBox("Fake Ghost", "visual_other_antiaimghost_fake", 15, 15, Color(30, 60, 100), Color(255,255,255));
	MenuControls::v_other_antiaimghost_lby = new MenuCheckBox("LowerBodyYaw Ghost", "visual_other_antiaimghost_lby", 15, 15, Color(30, 60, 100), Color(255, 255, 255));
	MenuControls::v_other_fakelag_ghost = new MenuCheckBox("FakeLag Ghost", "visual_other_fakelag_ghost", 15, 15, Color(30, 60, 100), Color(255, 255, 255));
	MenuControls::v_other_grenade_prediction = new MenuCheckBox("Grenade Prediction", "visual_other_grenade_prediction", 15, 15, Color(30, 60, 100));
	MenuControls::v_other_nosmoke = new MenuComboBox("No Smoke", "visual_other_nosmoke", 100, 15, Color(30, 60, 100));
	//--------------------------------------------//
	MenuControls::v_skins_enable = new MenuCheckBox("Enable SkinChanger", "visual_skins_enable", 15, 15, Color(30, 60, 100));
	MenuControls::v_skins_knife_model = new MenuComboBox("Knife Model", "visual_skins_knife_model", 100, 15, Color(30, 60, 100));
	MenuControls::v_skins_paint_id = new MenuInputBox("Paintkit", "visual_skins_paintkit", 100, 15, Color(30, 60, 100), InputType::InputType_ONLY_NUMBERS);
	MenuControls::v_skins_seed = new MenuInputBox("Seed", "visual_skins_seed", 100, 15, Color(30, 60, 100), InputType::InputType_ONLY_NUMBERS);
	//--------------------------------------------//
	MenuControls::m_debug = new MenuCheckBox("Debug Mode", "misc_debug", 15, 15, Color(30, 60, 100));
	MenuControls::m_clantag = new MenuCheckBox("ClanTag", "misc_clantag", 15, 15, Color(30, 60, 100));
	MenuControls::m_bunnyhop = new MenuCheckBox("BunnyHop", "misc_bunnyhop", 15, 15, Color(30, 60, 100));
	MenuControls::m_bunnyhop_delay = new MenuSlider("BunnyHop Delay", "misc_bunnyhop_delay", 100, 15, 0, 0, 50, Color(30, 60, 100));
	MenuControls::m_fakewalk = new MenuCheckBox("FakeWalk", "misc_fakewalk", 15, 15, Color(30, 60, 100));
	MenuControls::m_thirdperson_bind = new MenuKeyBox("ThirdPerson Key", "misc_thirdperson_key", 100, 15, Color(30, 60, 100), KeyType::KeyType_ONLY_CHARACTERS);
	MenuControls::m_circlestrafer_bind = new MenuKeyBox("CircleStrafer Key", "misc_circlestrafer_key", 100, 15, Color(30, 60, 100), KeyType::KeyType_ONLY_CHARACTERS);
	MenuControls::m_fakelatency = new MenuSlider("Fake Latency", "misc_fakelatency", 100, 15, 0, 0, 800, Color(30, 60, 100));
	MenuControls::m_fakelag_mode = new MenuComboBox("FakeLag Mode", "misc_fakelag_mode", 100, 15, Color(30, 60, 100));
	MenuControls::m_fakelag_max = new MenuSlider("FakeLag Max", "misc_fakelag_max", 100, 15, 0, 0, 15, Color(30, 60, 100));
	//--------------------------------------------//
	MenuControls::m_configs = new MenuComboBox("Config Selector", "misc_main_config_selector", 100, 15, Color(30, 60, 100));
	MenuControls::m_config_save = new MenuCheckBox("Save Config", "misc_config_save", 15, 15, Color(30, 60, 100));
	MenuControls::m_config_load = new MenuCheckBox("Load Config", "misc_config_load", 15, 15, Color(30, 60, 100));
	//--------------------------------------------//

	/////////////////////////////////////////////////////////////////////////////////////////////////

	// Aimbot Tab

	MenuGroup* aimbot_general_group = new MenuGroup("General", 5, 5, 290, 460, Color(30, 60, 100));
	MenuGroup* aimbot_resolver_group = new MenuGroup("Resolver", 305, 5, 290, 155, Color(30, 60, 100));
	MenuGroup* aimbot_misc_group = new MenuGroup("Other", 305, 165, 290, 55, Color(30, 60, 100));

	aimbot_general_group->AddControl(MenuControls::a_master_switch);
	MenuControls::a_hitboxes->AddItem("Head");
	MenuControls::a_hitboxes->AddItem("Chest");
	MenuControls::a_hitboxes->AddItem("Arms");
	MenuControls::a_hitboxes->AddItem("Legs");
	aimbot_general_group->AddControl(MenuControls::a_hitboxes);
	aimbot_general_group->AddControl(MenuControls::a_silent);
	aimbot_general_group->AddControl(MenuControls::a_shoot);
	aimbot_general_group->AddControl(MenuControls::a_ignoreteams);
	aimbot_general_group->AddControl(MenuControls::a_norecoil);
	//aimbot_general_group->AddControl(a_nospread);
	aimbot_general_group->AddControl(MenuControls::a_autoscope);
	aimbot_general_group->AddControl(MenuControls::a_autowall);
	aimbot_general_group->AddControl(MenuControls::a_backtrack);
	aimbot_general_group->AddControl(MenuControls::a_multipoint);
	aimbot_general_group->AddControl(MenuControls::a_pointscale);
	aimbot_general_group->AddControl(MenuControls::a_mindamage);
	aimbot_general_group->AddControl(MenuControls::a_hitchance);
	MenuControls::a_hitscan_settings->AddItem("BackTrack");
	MenuControls::a_hitscan_settings->AddItem("Visible Hitboxes");
	MenuControls::a_hitscan_settings->AddItem("Basic HitBoxes");
	MenuControls::a_hitscan_settings->AddItem("Fast HitScan");
	MenuControls::a_hitscan_settings->AddItem("Max 3 Hitboxes");
	aimbot_general_group->AddControl(MenuControls::a_hitscan_settings);

	aimbot_resolver_group->AddControl(MenuControls::a_resolver);
	aimbot_resolver_group->AddControl(MenuControls::a_resolver_resolve_all);
	MenuControls::a_resolver_mode->AddItem("Experimental");
	MenuControls::a_resolver_mode->AddItem("Ground");
	MenuControls::a_resolver_mode->AddItem("Brute");
	MenuControls::a_resolver_mode->AddItem("Step");
	MenuControls::a_resolver_mode->AddItem("LBY");
	aimbot_resolver_group->AddControl(MenuControls::a_resolver_mode);
	MenuControls::a_resolver_overwrite_pitch->AddItem("Disabled");
	MenuControls::a_resolver_overwrite_pitch->AddItem("Down");
	MenuControls::a_resolver_overwrite_pitch->AddItem("Zero");
	MenuControls::a_resolver_overwrite_pitch->AddItem("Mix");
	MenuControls::a_resolver_overwrite_pitch->AddItem("Up");
	aimbot_resolver_group->AddControl(MenuControls::a_resolver_overwrite_pitch);
	MenuControls::a_resolver_bodyaim_if->AddItem("In Air");
	MenuControls::a_resolver_bodyaim_if->AddItem("Unresolved");
	aimbot_resolver_group->AddControl(MenuControls::a_resolver_bodyaim_if);
	aimbot_resolver_group->AddControl(MenuControls::a_resolver_bodyaim_after_shots);
	aimbot_resolver_group->AddControl(MenuControls::a_resolver_bruteforce_angle_step);

	MenuGroupTab general_aa("General", 0);
	general_aa.AddControl(MenuControls::a_antiaim);
	general_aa.AddControl(MenuControls::a_antiaim_anti_lby_air);
	general_aa.AddControl(MenuControls::a_antiaim_at_target);
	general_aa.AddControl(MenuControls::a_antiaim_manual);
	general_aa.AddControl(MenuControls::a_antiaim_desync);
	general_aa.AddControl(MenuControls::a_antiaim_stand_freestanding);
	MenuGroupTab standing_aa("Stand", 1);
	MenuControls::a_antiaim_stand_pitch->AddItem("Disabled");
	MenuControls::a_antiaim_stand_pitch->AddItem("Down");
	MenuControls::a_antiaim_stand_pitch->AddItem("Zero");
	MenuControls::a_antiaim_stand_pitch->AddItem("Up");
	MenuControls::a_antiaim_stand_yaw->AddItem("Disabled");
	MenuControls::a_antiaim_stand_yaw->AddItem("Jitter");
	MenuControls::a_antiaim_stand_yaw->AddItem("Spin");
	MenuControls::a_antiaim_stand_yaw->AddItem("Sideways");
	MenuControls::a_antiaim_stand_yaw->AddItem("Random");
	MenuControls::a_antiaim_stand_yaw->AddItem("Static");
	MenuControls::a_antiaim_stand_yaw->AddItem("LowerBody");
	MenuControls::a_antiaim_stand_yaw->AddItem("BackWards");
	MenuControls::a_antiaim_stand_yaw->AddItem("Progressive");
	//MenuControls::a_antiaim_stand_yaw_fake->AddItem("Disabled");
	//MenuControls::a_antiaim_stand_yaw_fake->AddItem("Jitter");
	//MenuControls::a_antiaim_stand_yaw_fake->AddItem("Spin");
	//MenuControls::a_antiaim_stand_yaw_fake->AddItem("Sideways");
	//MenuControls::a_antiaim_stand_yaw_fake->AddItem("Random");
	//MenuControls::a_antiaim_stand_yaw_fake->AddItem("Static");
	//MenuControls::a_antiaim_stand_yaw_fake->AddItem("LowerBody");
	//MenuControls::a_antiaim_stand_yaw_fake->AddItem("BackWards");
	//MenuControls::a_antiaim_stand_yaw_fake->AddItem("Progressive");
	MenuControls::a_antiaim_stand_break_mode->AddItem("Disabled");
	MenuControls::a_antiaim_stand_break_mode->AddItem("Real Anim");
	MenuControls::a_antiaim_stand_break_mode->AddItem("Fake Anim");
	standing_aa.AddControl(MenuControls::a_antiaim_stand_pitch);
	standing_aa.AddControl(MenuControls::a_antiaim_stand_yaw);
	standing_aa.AddControl(MenuControls::a_antiaim_stand_yaw_addition);
	//standing_aa.AddControl(MenuControls::a_antiaim_stand_yaw_fake);
	//standing_aa.AddControl(MenuControls::a_antiaim_stand_yaw_fake_addition);
	standing_aa.AddControl(MenuControls::a_antiaim_stand_break_mode);
	standing_aa.AddControl(MenuControls::a_antiaim_stand_break_value);
	MenuGroupTab walking_aa("Ground", 2);
	MenuControls::a_antiaim_ground_pitch->AddItem("Disabled");
	MenuControls::a_antiaim_ground_pitch->AddItem("Down");
	MenuControls::a_antiaim_ground_pitch->AddItem("Zero");
	MenuControls::a_antiaim_ground_pitch->AddItem("Up");
	MenuControls::a_antiaim_ground_yaw->AddItem("Disabled");
	MenuControls::a_antiaim_ground_yaw->AddItem("Jitter");
	MenuControls::a_antiaim_ground_yaw->AddItem("Spin");
	MenuControls::a_antiaim_ground_yaw->AddItem("Sideways");
	MenuControls::a_antiaim_ground_yaw->AddItem("Random");
	MenuControls::a_antiaim_ground_yaw->AddItem("Static");
	MenuControls::a_antiaim_ground_yaw->AddItem("LowerBody");
	MenuControls::a_antiaim_ground_yaw->AddItem("BackWards");
	MenuControls::a_antiaim_ground_yaw->AddItem("Progressive");
	//MenuControls::a_antiaim_ground_yaw_fake->AddItem("Disabled");
	//MenuControls::a_antiaim_ground_yaw_fake->AddItem("Jitter");
	//MenuControls::a_antiaim_ground_yaw_fake->AddItem("Spin");
	//MenuControls::a_antiaim_ground_yaw_fake->AddItem("Sideways");
	//MenuControls::a_antiaim_ground_yaw_fake->AddItem("Random");
	//MenuControls::a_antiaim_ground_yaw_fake->AddItem("Static");
	//MenuControls::a_antiaim_ground_yaw_fake->AddItem("LowerBody");
	//MenuControls::a_antiaim_ground_yaw_fake->AddItem("BackWards");
	//MenuControls::a_antiaim_ground_yaw_fake->AddItem("Progressive");
	walking_aa.AddControl(MenuControls::a_antiaim_ground_pitch);
	walking_aa.AddControl(MenuControls::a_antiaim_ground_yaw);
	walking_aa.AddControl(MenuControls::a_antiaim_ground_yaw_addition);
	//walking_aa.AddControl(MenuControls::a_antiaim_ground_yaw_fake);
	//walking_aa.AddControl(MenuControls::a_antiaim_ground_yaw_fake_addition);
	MenuGroupTab in_air_aa("Air", 3);
	MenuControls::a_antiaim_air_pitch->AddItem("Disabled");
	MenuControls::a_antiaim_air_pitch->AddItem("Down");
	MenuControls::a_antiaim_air_pitch->AddItem("Zero");
	MenuControls::a_antiaim_air_pitch->AddItem("Up");
	MenuControls::a_antiaim_air_yaw->AddItem("Disabled");
	MenuControls::a_antiaim_air_yaw->AddItem("Jitter");
	MenuControls::a_antiaim_air_yaw->AddItem("Spin");
	MenuControls::a_antiaim_air_yaw->AddItem("Sideways");
	MenuControls::a_antiaim_air_yaw->AddItem("Random");
	MenuControls::a_antiaim_air_yaw->AddItem("Static");
	MenuControls::a_antiaim_air_yaw->AddItem("LowerBody");
	MenuControls::a_antiaim_air_yaw->AddItem("BackWards");
	MenuControls::a_antiaim_air_yaw->AddItem("Progressive");
	//MenuControls::a_antiaim_air_yaw_fake->AddItem("Disabled");
	//MenuControls::a_antiaim_air_yaw_fake->AddItem("Jitter");
	//MenuControls::a_antiaim_air_yaw_fake->AddItem("Spin");
	//MenuControls::a_antiaim_air_yaw_fake->AddItem("Sideways");
	//MenuControls::a_antiaim_air_yaw_fake->AddItem("Random");
	//MenuControls::a_antiaim_air_yaw_fake->AddItem("Static");
	//MenuControls::a_antiaim_air_yaw_fake->AddItem("LowerBody");
	//MenuControls::a_antiaim_air_yaw_fake->AddItem("BackWards");
	//MenuControls::a_antiaim_air_yaw_fake->AddItem("Progressive");
	in_air_aa.AddControl(MenuControls::a_antiaim_air_pitch);
	in_air_aa.AddControl(MenuControls::a_antiaim_air_yaw);
	in_air_aa.AddControl(MenuControls::a_antiaim_air_yaw_addition);
	//in_air_aa.AddControl(MenuControls::a_antiaim_air_yaw_fake);
	//in_air_aa.AddControl(MenuControls::a_antiaim_air_yaw_fake_addition);
	std::vector<MenuGroupTab> antiaim_tabs = { general_aa , standing_aa, walking_aa, in_air_aa };
	MenuGroup* aimbot_antiaim_group = new MenuGroup("Anti-aimbot", 305, 225, 290, 240, Color(30, 60, 100), antiaim_tabs);

	aimbot_misc_group->AddControl(MenuControls::a_autostop);
	aimbot_misc_group->AddControl(MenuControls::a_autorevolver);

	aimbot_general_group->OrganizeControls();
	aimbot_resolver_group->OrganizeControls();
	aimbot_misc_group->OrganizeControls();
	aimbot_antiaim_group->OrganizeControls();

	aimbot_tab->AddGroup(aimbot_general_group);
	aimbot_tab->AddGroup(aimbot_resolver_group);
	aimbot_tab->AddGroup(aimbot_misc_group);
	aimbot_tab->AddGroup(aimbot_antiaim_group);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Visuals Tab

	MenuGroup* visuals_skins_group = new MenuGroup("SkinChanger", 305, 5, 290, 150, Color(30, 60, 100));
	MenuGroup* visuals_other_group = new MenuGroup("Other", 305, 160, 290, 305, Color(30, 60, 100));

	MenuGroupTab general_visuals("General", 0);
	general_visuals.AddControl(MenuControls::v_players_esp_enable);
	general_visuals.AddControl(MenuControls::v_players_esp_offscreen);
	general_visuals.AddControl(MenuControls::v_players_esp_offscreen_distance);

	MenuControls::v_players_esp_glow_mode->AddItem("Normal");
	MenuControls::v_players_esp_glow_mode->AddItem("FullBloom");
	general_visuals.AddControl(MenuControls::v_players_esp_glow_mode);
	general_visuals.AddControl(MenuControls::v_players_esp_glow_bloom);
	general_visuals.AddControl(MenuControls::v_players_esp_dormant);
	MenuControls::v_players_esp_health->AddItem("Disabled");
	MenuControls::v_players_esp_health->AddItem("Normal");
	MenuControls::v_players_esp_health->AddItem("Separated");
	MenuControls::v_players_esp_chams_mode->AddItem("Normal");
	MenuControls::v_players_esp_chams_mode->AddItem("Flat");
	MenuControls::v_players_esp_chams_mode->AddItem("Wireframe");
	MenuControls::v_players_esp_chams_mode->AddItem("Transparent");
	general_visuals.AddControl(MenuControls::v_players_esp_chams_mode);
	general_visuals.AddControl(MenuControls::v_players_esp_health);
	general_visuals.AddControl(MenuControls::v_players_esp_weapon);
	general_visuals.AddControl(MenuControls::v_players_esp_sound);
	general_visuals.AddControl(MenuControls::v_players_esp_name);
	general_visuals.AddControl(MenuControls::v_players_esp_info);

	MenuGroupTab allies_visuals("Allies", 1);
	allies_visuals.AddControl(MenuControls::v_players_esp_ally_enable);
	allies_visuals.AddControl(MenuControls::v_players_esp_ally_box);
	allies_visuals.AddControl(MenuControls::v_players_esp_ally_chams);
	allies_visuals.AddControl(MenuControls::v_players_esp_ally_glow);

	MenuGroupTab enemies_visuals("Enemies", 2);
	enemies_visuals.AddControl(MenuControls::v_players_esp_enemy_enable);
	enemies_visuals.AddControl(MenuControls::v_players_esp_enemy_box);
	enemies_visuals.AddControl(MenuControls::v_players_esp_enemy_chams);
	enemies_visuals.AddControl(MenuControls::v_players_esp_enemy_glow);
	MenuControls::v_players_esp_drawbacktrack->AddItem("Disabled");
	MenuControls::v_players_esp_drawbacktrack->AddItem("Square");
	MenuControls::v_players_esp_drawbacktrack->AddItem("Chams");
	enemies_visuals.AddControl(MenuControls::v_players_esp_drawbacktrack);

	visuals_skins_group->AddControl(MenuControls::v_skins_enable);
	MenuControls::v_skins_knife_model->AddItem("Default");
	MenuControls::v_skins_knife_model->AddItem("Bayonet");
	MenuControls::v_skins_knife_model->AddItem("Flip Knife");
	MenuControls::v_skins_knife_model->AddItem("Gut Knife");
	MenuControls::v_skins_knife_model->AddItem("Karambit");
	MenuControls::v_skins_knife_model->AddItem("M9 Bayonet");
	MenuControls::v_skins_knife_model->AddItem("Huntsman Knife");
	MenuControls::v_skins_knife_model->AddItem("Bowie Knife");
	MenuControls::v_skins_knife_model->AddItem("Butterfly Knife");
	MenuControls::v_skins_knife_model->AddItem("Shadow Daggers");
	MenuControls::v_skins_knife_model->AddItem("Falchion Knife");
	visuals_skins_group->AddControl(MenuControls::v_skins_knife_model);
	visuals_skins_group->AddControl(MenuControls::v_skins_seed);
	visuals_skins_group->AddControl(MenuControls::v_skins_paint_id);

	visuals_other_group->AddControl(MenuControls::v_other_hitmarker);
	visuals_other_group->AddControl(MenuControls::v_other_crosshair);
	visuals_other_group->AddControl(MenuControls::v_other_noscope);
	visuals_other_group->AddControl(MenuControls::v_other_postprocess);
	visuals_other_group->AddControl(MenuControls::v_other_projectiles);
	visuals_other_group->AddControl(MenuControls::v_other_grenade_prediction);
	visuals_other_group->AddControl(MenuControls::v_other_nightmode);
	visuals_other_group->AddControl(MenuControls::v_other_nightmode_value);
	visuals_other_group->AddControl(MenuControls::v_other_antiaimghost_fake);
	visuals_other_group->AddControl(MenuControls::v_other_antiaimghost_lby);
	visuals_other_group->AddControl(MenuControls::v_other_fakelag_ghost);
	MenuControls::v_other_nosmoke->AddItem("Disabled");
	MenuControls::v_other_nosmoke->AddItem("Wireframe");
	MenuControls::v_other_nosmoke->AddItem("Full");
	visuals_other_group->AddControl(MenuControls::v_other_nosmoke);

	std::vector<MenuGroupTab> visual_tabs = { general_visuals ,allies_visuals, enemies_visuals };
	MenuGroup* visuals_general_group = new MenuGroup("General", 5, 5, 290, 460, Color(30, 60, 100), visual_tabs);

	visuals_general_group->OrganizeControls();
	visuals_skins_group->OrganizeControls();
	visuals_other_group->OrganizeControls();

	visuals_tab->AddGroup(visuals_general_group);
	visuals_tab->AddGroup(visuals_skins_group);
	visuals_tab->AddGroup(visuals_other_group);

	/////////////////////////////////////////////////////////////////////////////////////////////////

	MenuGroup* other_miscellaneous_group = new MenuGroup("Miscellaneous", 5, 5, 290, 460, Color(30, 60, 100));
	MenuGroup* other_configs_group = new MenuGroup("Configs", 305, 5, 290, 460, Color(30, 60, 100));

	other_miscellaneous_group->AddControl(MenuControls::m_debug);
	other_miscellaneous_group->AddControl(MenuControls::m_clantag);
	other_miscellaneous_group->AddControl(MenuControls::m_bunnyhop);
	other_miscellaneous_group->AddControl(MenuControls::m_bunnyhop_delay);
	other_miscellaneous_group->AddControl(MenuControls::m_fakewalk);
	other_miscellaneous_group->AddControl(MenuControls::m_thirdperson_bind);
	other_miscellaneous_group->AddControl(MenuControls::m_circlestrafer_bind);
	other_miscellaneous_group->AddControl(MenuControls::m_fakelatency);
	MenuControls::m_fakelag_mode->AddItem("Disabled");
	MenuControls::m_fakelag_mode->AddItem("Adaptive");
	MenuControls::m_fakelag_mode->AddItem("Random");
	MenuControls::m_fakelag_mode->AddItem("Max");
	other_miscellaneous_group->AddControl(MenuControls::m_fakelag_mode);
	other_miscellaneous_group->AddControl(MenuControls::m_fakelag_max);

	MenuControls::m_configs->AddItem("Preset 1");
	MenuControls::m_configs->AddItem("Preset 2");
	MenuControls::m_configs->AddItem("Preset 3");
	MenuControls::m_configs->AddItem("Preset 4");
	MenuControls::m_configs->AddItem("Preset 5");
	other_configs_group->AddControl(MenuControls::m_configs);
	other_configs_group->AddControl(MenuControls::m_config_save);
	other_configs_group->AddControl(MenuControls::m_config_load);

	other_miscellaneous_group->OrganizeControls();
	other_configs_group->OrganizeControls();
	
	other_tab->AddGroup(other_miscellaneous_group);
	other_tab->AddGroup(other_configs_group);

	/////////////////////////////////////////////////////////////////////////////////////////////////

	new_frame->AddTab(aimbot_tab);
	new_frame->AddTab(visuals_tab);
	new_frame->AddTab(other_tab);

	MenuValues::frames.push_back(new_frame);

	Logger::SaveToLog("Menu loaded");
}

void Menu::UpdateConfigs()
{
	if (MenuControls::m_config_save->GetState())
	{
		MenuControls::m_config_save->LeftPressEvent();
		MenuUtils::SaveConfig(MenuControls::m_configs->GetSelectedItem());
	}
	if (MenuControls::m_config_load->GetState())
	{
		MenuControls::m_config_load->LeftPressEvent();
		MenuUtils::LoadConfig(MenuControls::m_configs->GetSelectedItem());
	}
}

void Menu::Update()
{
	if (MenuValues::MenuOpen != GetKeyState(VK_INSERT))
	{
		if (!GetKeyState(VK_INSERT)) // We close all the controls
		{
			for (MenuFrame* frame : MenuValues::frames)
			{
				for (MenuTab* tab : frame->GetTabs())
				{
					for (MenuGroup* group : tab->GetGroups())
					{
						for (MenuControl* control : group->GetControls())
						{
							MenuComboBox* combobox = (MenuComboBox*)control;
							MenuSelectionBox* selectionbox = (MenuSelectionBox*)control;
							MenuKeyBox* keybox = (MenuKeyBox*)control;
							MenuInputBox* inputbox = (MenuInputBox*)control;

							switch (control->GetControlType())
							{
							case Control_ComboBox:
								if (combobox->IsOpen())
								{
									combobox->LeftPressEvent();
								}
								break;
							case Control_SelectionBox:
								if (selectionbox->IsOpen())
								{
									selectionbox->LeftPressEvent();
								}
								break;
							case Control_KeyBox:
								if (keybox->WaitingForKey())
								{
									keybox->LeftPressEvent();
								}
								break;
							case Control_InputBox:
								if (inputbox->WaitingForInput())
								{
									inputbox->LeftPressEvent();
								}
								break;
							default:
								break;
							}
						}
					}
				}
			}
		}

		if (MenuValues::MenuOpen != GetKeyState(VK_INSERT))
		{
			MenuValues::MenuOpen = GetKeyState(VK_INSERT);

			if (MenuValues::MenuOpen)
			{
				game_interfaces::Surface->UnLockCursor();
			}

		}

	}

	if (MenuValues::MenuOpen)
	{
		UpdateConfigs();
		UpdateAnimations();
		HandleLeftMouse();
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			MenuValues::LeftMousePressed = true;
		}
		else {
			MenuValues::LeftMousePressed = false;
		}
		//
	}
}

void Menu::Render()
{
	if (MenuValues::MenuOpen)
	{
		for (MenuFrame* frame : MenuValues::frames)
		{
			float frame_x = frame->GetX();
			float frame_y = frame->GetY() + frame->GetTitleBarHeight();
			float frame_x_title_middle = (frame->GetX() + (frame->GetWidth() / 2)) - (RenderUtils::GetTextLenght(frame->GetTitleName(), RenderUtils::Menu_Title_font) / 2);
			float frame_y_title_middle = (frame->GetY() + (frame->GetTitleBarHeight() / 2)) - (RenderUtils::GetTextHeight(frame->GetTitleName(), RenderUtils::Menu_Title_font) / 2);
			float frame_title_width = (RenderUtils::GetTextLenght(frame->GetTitleName(), RenderUtils::Menu_Title_font));

			RenderUtils::RenderRect(frame->GetX(), frame->GetY(), frame->GetWidth(), frame->GetHeight(), Color(50, 50, 50, 255));
			RenderUtils::RenderRect(frame->GetX(), frame->GetY(), frame->GetWidth(), frame->GetTitleBarHeight(), Color(25, 25, 25, 255));
			RenderUtils::RenderCheatTitle(frame->GetX(), frame->GetY(), frame->GetWidth(), frame->GetTitleBarHeight());
			RenderUtils::RenderText(frame_x_title_middle, frame_y_title_middle, Color::White(), frame->GetTitleName().c_str(), RenderUtils::Menu_Title_font);
			RenderUtils::RenderLine(frame_x_title_middle + 1, frame->GetY() + frame->GetTitleBarHeight() - 7, frame_x_title_middle + 1 + (frame_title_width), frame->GetY() + frame->GetTitleBarHeight() - 7, Color(0,0,0,255)); // Shadow
			RenderUtils::RenderLine(frame_x_title_middle, frame->GetY() + frame->GetTitleBarHeight() - 8, frame_x_title_middle + (frame_title_width), frame->GetY() + frame->GetTitleBarHeight() - 8, Color::White()); // Front
			RenderUtils::RenderOutlineRect(frame->GetX(), frame->GetY(), frame->GetWidth(), frame->GetHeight(), Color(0, 0, 0, 255));
			RenderUtils::RenderLine(frame->GetX(), frame->GetY() + frame->GetTitleBarHeight(), frame->GetX() + frame->GetWidth(), frame->GetY() + frame->GetTitleBarHeight(), Color(0, 0, 0, 255));

			int tab_width = 0;
			for (MenuTab* tab : frame->GetTabs())
			{
				tab_width = tab->GetWidth();
				break;
			}
			int tab_w = (int)((float)frame->GetTabsAnimationValue() / 100.f * (float)tab_width);
			RenderUtils::RenderRect(frame->GetX() - tab_w, frame->GetY(), tab_w, frame->GetHeight(), Color(50, 50, 50, 255));
			RenderUtils::RenderCheatTitle(frame->GetX() - tab_w, frame->GetY(), tab_w, frame->GetTitleBarHeight());

			int tab_y = 0;
			int tab_id = 0;
			for (MenuTab* tab : frame->GetTabs())
			{
				if (tab_w > RenderUtils::GetTextLenght("Tabs", RenderUtils::Normal_font))
				{
					float text_alpha = ((float)frame->GetTabsAnimationValue() / 100.f * 255.f);
					int text_x = (frame->GetX() - tab_w) + ((float)tab_w / 2.f) - ((float)RenderUtils::GetTextLenght("Tabs", RenderUtils::Normal_font) / 2.f);
					int text_y = frame->GetY() + ((float)frame->GetTitleBarHeight() / 2.f) - ((float)RenderUtils::GetTextHeight("Tabs", RenderUtils::Normal_font) / 2.f);
					RenderUtils::RenderText(text_x, text_y, Color(255, 255, 255, text_alpha), "Tabs");
				}

				tab->Render(frame_x, frame_y + tab_y, frame->GetTabsAnimationValue(), tab_id == frame->GetCurrentTab());
				RenderUtils::RenderOutlineRect(frame->GetX() - tab_w, frame->GetY(), tab_w, frame->GetHeight(), Color(0, 0, 0, 255));

				if (tab_id == frame->GetCurrentTab())
				{
					for (MenuGroup* group : tab->GetGroups())
					{
						group->Render(frame_x, frame_y);
					}

					// We draw over the other group boxes

					for (MenuGroup* group : tab->GetGroups())
					{
						bool has_priority = false;

						for (MenuControl* control : group->GetControls())
						{
							if (control->GetControlType() == Control_ComboBox)
							{
								MenuComboBox* combo_box = (MenuComboBox*)control;
								if (combo_box->IsOpen())
								{
									has_priority = true;
								}
							}
							if (control->GetControlType() == Control_SelectionBox)
							{
								MenuSelectionBox* selection_box = (MenuSelectionBox*)control;
								if (selection_box->IsOpen())
								{
									has_priority = true;
								}
							}
							if (control->GetControlType() == Control_CheckBox)
							{
								MenuCheckBox* check_box = (MenuCheckBox*)control;
								if (check_box->IsColorBoxOpen() && check_box->IsUsingColor())
								{
									has_priority = true;
								}
							}
						}
						if (has_priority)
						{
							group->Render(frame_x, frame_y);
						}
					}
				}

				tab_id += 1;
				tab_y += tab->GetHeight() + 5;
			}
		}

		// Mouse Pointer Drawing
		POINT mouse = MenuUtils::GetMousePosition();
		RenderUtils::TriangleFilled(Vector2D(mouse.x + 12, mouse.y + 25), Vector2D(mouse.x + 1, mouse.y + 1), Vector2D(mouse.x + 26, mouse.y + 10), Color(0,125,255,255));
	//	RenderUtils::Triangle(Vector2D(mouse.x + 12, mouse.y + 25), Vector2D(mouse.x + 2, mouse.y + 2), Vector2D(mouse.x + 24, mouse.y + 11), Color(0, 0, 0));
	}
}

namespace MenuControls
{
	//--------------------------------------------//
	MenuCheckBox* a_master_switch;
	MenuSelectionBox* a_hitboxes;
	MenuCheckBox* a_silent;
	MenuCheckBox* a_shoot;
	MenuCheckBox* a_autoscope;
	MenuCheckBox* a_autowall;
	MenuCheckBox* a_backtrack;
	MenuCheckBox* a_ignoreteams;
	MenuSlider*	a_pointscale;
	MenuSlider*   a_multipoint;
	MenuSlider*   a_mindamage;
	MenuSlider*   a_hitchance;
	MenuSelectionBox* a_hitscan_settings;

	MenuCheckBox* a_antiaim;

	MenuComboBox* a_antiaim_stand_pitch;
	MenuComboBox* a_antiaim_stand_yaw;
	//MenuComboBox* a_antiaim_stand_yaw_fake;
	MenuSlider*   a_antiaim_stand_yaw_addition;
	//MenuSlider*   a_antiaim_stand_yaw_fake_addition;

	MenuComboBox* a_antiaim_ground_pitch;
	MenuComboBox* a_antiaim_ground_yaw;
	//MenuComboBox* a_antiaim_ground_yaw_fake;
	MenuSlider*   a_antiaim_ground_yaw_addition;
	//MenuSlider*   a_antiaim_ground_yaw_fake_addition;

	MenuComboBox* a_antiaim_air_pitch;
	MenuComboBox* a_antiaim_air_yaw;
	//MenuComboBox* a_antiaim_air_yaw_fake;
	MenuSlider*   a_antiaim_air_yaw_addition;
	//MenuSlider*   a_antiaim_air_yaw_fake_addition;

	MenuCheckBox* a_antiaim_desync;
	MenuCheckBox* a_antiaim_stand_freestanding;
	MenuCheckBox* a_antiaim_manual;
	MenuSlider*	a_antiaim_stand_break_value;
	MenuComboBox* a_antiaim_stand_break_mode;
	MenuCheckBox* a_antiaim_at_target;
	MenuCheckBox* a_antiaim_anti_lby_air;

	MenuCheckBox* a_norecoil;
	MenuCheckBox* a_nospread;
	MenuCheckBox* a_autostop;
	MenuCheckBox* a_autorevolver;

	MenuCheckBox* a_resolver;
	MenuCheckBox* a_resolver_resolve_all;
	MenuComboBox* a_resolver_overwrite_pitch;
	MenuComboBox* a_resolver_mode;
	MenuSlider* a_resolver_bruteforce_angle_step;
	MenuSlider* a_resolver_bodyaim_after_shots;
	MenuSelectionBox* a_resolver_bodyaim_if;

	//--------------------------------------------//
	MenuCheckBox* v_players_esp_enable;
	MenuCheckBox* v_players_esp_dormant;
	MenuComboBox* v_players_esp_health;
	MenuCheckBox* v_players_esp_weapon;
	MenuCheckBox* v_players_esp_name;
	MenuCheckBox* v_players_esp_sound;

	MenuCheckBox* v_players_esp_ally_enable;
	MenuCheckBox* v_players_esp_ally_chams;
	MenuCheckBox* v_players_esp_ally_box;
	MenuCheckBox* v_players_esp_ally_glow;

	MenuCheckBox* v_players_esp_enemy_enable;
	MenuCheckBox* v_players_esp_enemy_chams;
	MenuCheckBox* v_players_esp_enemy_box;
	MenuCheckBox* v_players_esp_enemy_glow;

	MenuComboBox* v_players_esp_chams_mode;

	MenuComboBox* v_players_esp_glow_mode;
	MenuSlider* v_players_esp_glow_bloom;
	
	MenuCheckBox* v_players_esp_info;
	MenuCheckBox* v_players_esp_offscreen;
	MenuSlider* v_players_esp_offscreen_distance;
	MenuComboBox* v_players_esp_drawbacktrack;
	//--------------------------------------------//
	MenuCheckBox* v_other_hitmarker;
	MenuCheckBox* v_other_crosshair;
	MenuCheckBox* v_other_noscope;
	MenuCheckBox* v_other_postprocess;
	MenuCheckBox* v_other_projectiles;
	MenuCheckBox* v_other_nightmode;
	MenuSlider* v_other_nightmode_value;
	MenuCheckBox* v_other_antiaimghost_fake;
	MenuCheckBox* v_other_antiaimghost_lby;
	MenuCheckBox* v_other_fakelag_ghost;
	MenuCheckBox* v_other_grenade_prediction;
	MenuComboBox* v_other_nosmoke;
	//--------------------------------------------//
	MenuCheckBox* v_skins_enable;
	MenuComboBox* v_skins_knife_model;
	MenuInputBox* v_skins_paint_id;
	MenuInputBox* v_skins_seed;
	//--------------------------------------------//
	MenuCheckBox* m_debug;
	MenuCheckBox* m_clantag;
	MenuCheckBox* m_bunnyhop;
	MenuSlider* m_bunnyhop_delay;
	MenuCheckBox* m_fakewalk;
	MenuKeyBox* m_thirdperson_bind;
	MenuKeyBox* m_circlestrafer_bind;
	MenuSlider*	m_fakelatency;
	MenuComboBox* m_fakelag_mode;
	MenuSlider*	m_fakelag_max;
	//--------------------------------------------//
	MenuComboBox* m_configs;
	MenuCheckBox* m_config_save;
	MenuCheckBox* m_config_load;
	//--------------------------------------------//
}
