#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "global_input.hpp"

namespace gui {
	void group_box(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, bool show_label);
	void tab(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string, std::int32_t& tab, std::int32_t count);
	void check_box(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value, int click_area_id = 2);
	void check_box(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value, color& setting_color, bool& toggle_color);
	void slider(std::int32_t x, std::int32_t y, std::int32_t slider_pos_x, std::int32_t slider_len, unsigned long font, const std::string string, float& value, float min_value, float max_value);
	void menu_movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h);
	void button(std::int32_t x, std::int32_t y, std::int32_t butt_pos, unsigned long font, const std::string label, void(*callback)());
	bool button_bool(std::int32_t x, std::int32_t y, std::int32_t butt_pos, unsigned long font, const std::string label);
	void id_changer(std::int32_t x, std::int32_t y, std::int32_t right_position, int val_cont_w, unsigned long font, const std::string label, int& target, int min, int max);
	inline bool should_drag = false;
	inline bool should_move = false; 
}

namespace spectator_framework {
	void spec_list_movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h);
	inline bool should_drag = false;
	inline bool should_move = false;
}

struct color_popup_info {
	std::int32_t x;
	std::int32_t y;
	color& target;
	bool& toggle_color;
};

namespace popup_system {
	/* ------------ Variables ------------ */
	const int win_padding = 10;
	const int slider_w = 127, slider_h = 15;		// w has to be divisible by 6 in order for the fade to be clean
	const int win_w = slider_w + win_padding * 2;
	const int win_h = slider_h * 2 + win_padding * 3;	// +1 slider and margin for alpha slider

	/* ------------ Functions ------------ */
	void render_popups();				// Will call each check_*_popups()
	bool mouse_in_popup(int x, int y);	// Will check if x:y is in a popup

	// Menu buttons will store here information about the popup that will be rendered
	inline std::vector<color_popup_info> active_color_popups;
	void check_color_popups();									// Will check for popups in the active_color_popups vector
	void color_picker_popup(color_popup_info color_popup);		// Will render the actual popup
}
