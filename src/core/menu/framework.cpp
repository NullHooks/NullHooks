#include "core/menu/framework.hpp"
#include "core/menu/variables.hpp"
#include "core/config/config.hpp"

typedef struct CursorCoords {
	int x;
	int y;
} cursor_coords;

cursor_coords cursor;
cursor_coords cursor_corrected;

#pragma region GUI EXTERNS
int gui::vars::column_number = 0;			// Current column. Will change when calling add_column()

// o_* variables are the original ones with 0 columns (all menu width)
int gui::vars::o_container_left_pos = variables::ui::menu::x + container_margin;		// Will change when adding more columns
int gui::vars::o_container_width = variables::ui::menu::w - container_margin * 2;	// Will get divided when adding more columns
int gui::vars::o_item_left_pos = o_container_left_pos + container_padding;			// Base top left pos for all items (label text position)

int gui::vars::o_item_combo_pos = variables::ui::menu::x + o_container_width - container_margin;		// Max right pos
int gui::vars::o_item_checkbox_pos = o_item_combo_pos - item_checkbox_length;
int gui::vars::o_item_slider_pos = o_item_combo_pos - item_slider_length;				// Top left corner of the actual slider
int gui::vars::o_item_hotkey_w = o_container_width - container_padding * 2;

// Actual vars for items and containers. Updated in init_tab() and add_column()
int gui::vars::container_left_pos = o_container_left_pos;
int gui::vars::container_width = o_container_width;
int gui::vars::item_left_pos = o_item_left_pos;
int gui::vars::item_combo_pos = o_item_combo_pos;
int gui::vars::item_checkbox_pos = o_item_checkbox_pos;
int gui::vars::item_slider_pos = o_item_slider_pos;
int gui::vars::item_hotkey_w = o_item_hotkey_w;

// Vars for groupbox
int gui::vars::o_cur_part_y = variables::ui::menu::y + vars::top_margin_with_tabs + vars::container_margin;
int gui::vars::o_cur_base_item_y = o_cur_part_y + container_padding;		// Base y position of the items (position of the first item of the groupbox)

int gui::vars::cur_part_items = 0;			// Will be changed when adding groupbox
int gui::vars::cur_part_y = o_cur_part_y;
int gui::vars::cur_base_item_y = o_cur_base_item_y;
int gui::vars::cur_part_h = 0;			// Will update with each item added

int gui::vars::button_part_item = 0;
int gui::vars::button_part_h = 0;			// Need to get h first to subtract it from bottom to get top pos
int gui::vars::button_part_y = variables::ui::menu::y + variables::ui::menu::h - container_margin;
int gui::vars::button_base_item_y = button_part_y + container_padding;
#pragma endregion

#pragma region GUI ITEMS
// Returns true if pressed
bool gui::button_bool(std::int32_t x, std::int32_t y, std::int32_t butt_pos, unsigned long font, const std::string label) {
	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);

	const int w = 30, h = 11;	// Button size
	const color c_default = color(150, 22, 22, 255);
	const color c_hover = color(135, 21, 21, 255);
	bool pressed = false;

	// Checkbox label
	render::draw_text_string(x + 2, y - 1, font, label, false, color::white());

	// Cursor in button and clicked
	if ((cursor.x >= butt_pos) && (cursor.x <= butt_pos + w) && (cursor.y >= y) && (cursor.y <= y + h)) {
		render::draw_filled_rect(butt_pos, y, w, h, color(115, 21, 21, 255));		// Button background (Hover)
		pressed = (!popup_system::mouse_in_popup(cursor.x, cursor.y) && input::gobal_input.IsPressed(VK_LBUTTON));
	} else {
		render::draw_filled_rect(butt_pos, y, w, h, color(150, 22, 22, 255));		// Button background
	}

	return pressed;
}

// Second implementation for button, it pass callback function
void gui::button(std::int32_t x, std::int32_t y, std::int32_t butt_pos, unsigned long font, const std::string label, void(*callback)()) {
	const bool pressed = button_bool(x, y, butt_pos, font, label);		// Call the bool function instead of copying the code again
	if (pressed) callback();
}

void gui::id_changer(std::int32_t x, std::int32_t y, std::int32_t right_position, int val_cont_w, unsigned long font, const std::string label, int& target, int min, int max) {
	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);

	const int button_margins = 2;									// After first button and before second
	const int bw = 11, bh = 11;										// Increase and decrease buttons
	const int br_x = right_position - bw;							// Right button - Increase
	const int val_cont_x = br_x - button_margins - val_cont_w;		// Value container
	const int bl_x = val_cont_x - button_margins - bw;				// Left button - Decrease

	color bl_col = color(150, 22, 22, 255);
	color br_col = color(150, 22, 22, 255);

	if (!popup_system::mouse_in_popup(cursor.x, cursor.y)) {
		if ((cursor.x >= bl_x) && (cursor.x <= bl_x + bw) && (cursor.y >= y) && (cursor.y <= y + bh)) {
			bl_col = color(115, 21, 21, 255);					// Hover
			if (input::gobal_input.IsPressed(VK_LBUTTON))
				target = (target == min) ? max : target - 1;	// Decrease
		}
	
		if ((cursor.x >= br_x) && (cursor.x <= br_x + bw) && (cursor.y >= y) && (cursor.y <= y + bh)) {
			br_col = color(115, 21, 21, 255);					// Hover
			if (input::gobal_input.IsPressed(VK_LBUTTON))
				target = (target == max) ? min : target + 1;	// Increase
		}
	}

	// Increase and decrease buttons
	render::draw_filled_rect(bl_x, y, bw, bh, bl_col);										// Normal color
	render::draw_text_string(bl_x + bw / 2  + 1, y - 1, font, "-", true, color::white());	// Button text
	render::draw_filled_rect(br_x, y, bw, bh, br_col);										// Normal color
	render::draw_text_string(br_x + bw / 2, y - 1, font, "+", true, color::white());		// Button text

	// Value itself
	render::draw_text_string(val_cont_x + val_cont_w / 2, y - 1, font, std::to_string(target), true, color::white());

	// Label
	render::draw_text_string(x + 2, y - 1, font, label, false, color::white());
}

void gui::groupbox(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, bool show_label) {
	// Groupbox background
	render::draw_filled_rect(x, y, w, h, color(25, 25, 25, 255));

	// Groupbox outline
	render::draw_rect(x, y, w, h, color(45, 45, 45, 255));

	// Groupbox label
	if (show_label)
		render::draw_text_string(x + 2, y - 12, font, string, false, color::white());
}

void gui::tab(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, std::int32_t& tab, std::int32_t count) {
	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);

	if ((cursor.x > x) && (cursor.x < x + w) && (cursor.y > y) && (cursor.y < y + h)
		&& !popup_system::mouse_in_popup(cursor.x, cursor.y) && input::gobal_input.IsPressed(VK_LBUTTON))
		tab = count;
	
	// Tab background and line
	if (tab == count) {
		render::draw_filled_rect(x, y, w, h, color(25, 25, 25, 255));
		render::draw_filled_rect(x, y+h-1, w, 2, color(150, 22, 22, 255));
	} else {
		render::draw_filled_rect(x, y, w, h, color(34, 34, 34, 255));
		render::draw_filled_rect(x, y +h, w, 1, color(45, 45, 45, 255));
	}

	// Tab label
	render::draw_text_string(x - render::get_text_size(font, string).x / 2 + w / 2, y + h / 2 - 6, font, string, false, color::white());
}

void gui::check_box(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value, int click_area_id) {
	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);

	const int w = 11, h = 11;

	// The bad thing about mouse_in_popup is that you can only check for popups after they are generated (You pop the items when rendering from the vector)
	if (!popup_system::mouse_in_popup(cursor.x, cursor.y)) {
		switch (click_area_id) {
			default:
			case 0: {	// Only checkbox
				if ((cursor.x >= position) && (cursor.x <= position + w) && (cursor.y >= y) && (cursor.y <= y + h) && input::gobal_input.IsPressed(VK_LBUTTON))
					value = !value;		// If in checkbox and clicked
				break;
			}
			case 1: {	// Name and checkbox, not color
				if (((cursor.x >= position) && (cursor.x <= position + w) && (cursor.y >= y) && (cursor.y <= y + h)		// Checkbox
					|| (cursor.x >= x) && (cursor.x <= position - 55) && (cursor.y >= y) && (cursor.y <= y + h))		// Name and all that. (5 + 20 + 5 + 20 + 5 for the colors)
					&& input::gobal_input.IsPressed(VK_LBUTTON))
					value = !value;		// If in checkbox or text, but not color picker and clicked
				break;
			}
			case 2: {	// All width from name to checkbox
				if ((cursor.x >= x) && (cursor.x <= position + w) && (cursor.y >= y - 1) && (cursor.y <= y + h + 1) && input::gobal_input.IsPressed(VK_LBUTTON)) {
					value = !value;		// If in checkbox or text and clicked
				}
				break;
			}
		}
	}

	// Checkbox itself
	render::draw_filled_rect(position, y, w, h, value ? color(150, 22, 22, 255) : color(36, 36, 36, 255));

	// Checkbox label
	render::draw_text_string(x + 2, y - 1, font, string, false, color::white());
}

// Checkbox with color picker and custom region
void gui::check_box(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value, colorpicker_col_t& col) {
	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);
	const int w = 11, h = 11;							// For checkbox
	const int margin = 5;								// Color "button" margin
	const int col_w = 20, col_h = 11;					// Color "button" size
	const int color_x = position - margin - col_w;		// Color "button" position

	if (!popup_system::mouse_in_popup(cursor.x, cursor.y) && input::gobal_input.IsPressed(VK_LBUTTON)) {		// Check click and all that once so it doesn't freak out
		if (((cursor.x >= position) && (cursor.x <= position + w) && (cursor.y >= y) && (cursor.y <= y + h))	// Checkbox
			|| ((cursor.x >= x) && (cursor.x <= position - 55) && (cursor.y >= y) && (cursor.y <= y + h)))		// Name and all that. (5 + 20 + 5 + 20 + 5 for the 2 colors)
			value = !value;
		
		// Not else if because we want to check if the cursor is in the toggle color button (open popup) or outside (close popup)
		if ((cursor.x >= color_x) && (cursor.x <= color_x + col_w) && (cursor.y >= y) && (cursor.y <= y + col_h))
			col.toggle = !col.toggle;	// Toggle the "active popup" bool
		// We need to check like this instead of using mouse_in_popup because the first check will be on a popup that is not yet in the active_color_popups vector (see bottom of this func)
		else if (!((cursor.x >= color_x) && (cursor.x <= color_x + popup_system::win_w) && (cursor.y >= y + h + margin) && (cursor.y <= y + h + margin + popup_system::win_h)))
			col.toggle = false;			// Close popup if user clicks outside
	}

	render::draw_filled_rect(position, y, w, h, value ? color(150, 22, 22, 255) : color(36, 36, 36, 255));		// Checkbox itself
	render::draw_text_string(x + 2, y - 1, font, string, false, color::white());								// Checkbox text

	render::draw_filled_rect(color_x, y, col_w, col_h, col.col);											// Color itself
	render::draw_rect(color_x - 1, y - 1, col_w + 2, col_h + 2, color(30, 30, 30, 255));						// Color outline

	// Push to vector to render after menu
	if (col.toggle)
		popup_system::active_color_popups.push_back(color_popup_info{ color_x, y + col_h + margin, col });
}

void gui::check_box(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value, colorpicker_col_t& col1, colorpicker_col_t& col2) {
	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);
	const int w = 11, h = 11;							// For checkbox
	const int margin = 5;								// Color "button" margin
	const int col_w = 20, col_h = 11;					// Color "button" size
	const int color_r_x = position - margin - col_w;	// Right color (2) "button" position
	const int color_l_x = color_r_x - margin - col_w;	// Left color (1) "button" position

	// Comments in other function
	if (!popup_system::mouse_in_popup(cursor.x, cursor.y) && input::gobal_input.IsPressed(VK_LBUTTON)) {		// Check click and all that once so it doesn't freak out
		// Checkbox
		if (((cursor.x >= position) && (cursor.x <= position + w) && (cursor.y >= y) && (cursor.y <= y + h))
			|| ((cursor.x >= x) && (cursor.x <= position - 55) && (cursor.y >= y) && (cursor.y <= y + h)))		// Name and all that. (5 + 20 + 5 + 20 + 5 for the 2 colors)
			value = !value;
		
		// Clicked outside popups. We need to check like this so both popups can't be open at the same time
		if (!((cursor.x >= color_l_x) && (cursor.x <= color_r_x + popup_system::win_w) && (cursor.y >= y + h + margin) && (cursor.y <= y + h + margin + popup_system::win_h))) {
			// Check if we clicked left button, then toggle. If not just turn it off
			if ((cursor.x >= color_l_x) && (cursor.x <= color_l_x + col_w) && (cursor.y >= y) && (cursor.y <= y + col_h))
				col1.toggle = !col1.toggle;
			else
				col1.toggle = false;

			// Check if we clicked right button, then toggle. If not just turn it off
			if ((cursor.x >= color_r_x) && (cursor.x <= color_r_x + col_w) && (cursor.y >= y) && (cursor.y <= y + col_h))
				col2.toggle = !col2.toggle;
			else
				col2.toggle = false;
		}
	}

	// Checkbox
	render::draw_filled_rect(position, y, w, h, value ? color(150, 22, 22, 255) : color(36, 36, 36, 255));
	render::draw_text_string(x + 2, y - 1, font, string, false, color::white());

	// Left color
	render::draw_filled_rect(color_l_x, y, col_w, col_h, col1.col);
	render::draw_rect(color_l_x - 1, y - 1, col_w + 2, col_h + 2, color(30, 30, 30, 255));

	// Right color
	render::draw_filled_rect(color_r_x, y, col_w, col_h, col2.col);
	render::draw_rect(color_r_x - 1, y - 1, col_w + 2, col_h + 2, color(30, 30, 30, 255));

	// Push to vector to render after menu
	if (col1.toggle)
		popup_system::active_color_popups.push_back(color_popup_info{ color_l_x, y + col_h + margin, col1 });
	if (col2.toggle)
		popup_system::active_color_popups.push_back(color_popup_info{ color_r_x, y + col_h + margin, col2 });
}

// Thanks to https://github.com/bobloxmonke
float map_slider_constrain(float n, float start1, float stop1, float start2, float stop2) {
	float value = (n - start1) / (stop1 - start1) * (stop2 - start2) + start2;
	return std::clamp(value, start2, stop2);
}

void gui::slider(std::int32_t x, std::int32_t y, std::int32_t slider_pos_x, std::int32_t slider_len, unsigned long font, const std::string string, float& value, float min_value, float max_value) {
	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);
	const int slider_y = y + 1;			// The actual text starts 2px down from the y parameter, so +1 is actually -1 from real text
	const int slider_height = 9;
	
	// Get value from cursor and assign it
	if ((cursor.x >= slider_pos_x) && (cursor.x <= slider_pos_x + slider_len) && (cursor.y >= slider_y) && (cursor.y <= slider_y + slider_height)
		&& !popup_system::mouse_in_popup(cursor.x, cursor.y) && input::gobal_input.IsHeld(VK_LBUTTON))
		value = map_slider_constrain((cursor.x - slider_pos_x), 0.0f, float(slider_len), float(min_value), float(max_value));

	// Slider background and value display
	const float reverse_map = map_slider_constrain(value, float(min_value), float(max_value), 0.0f, float(slider_len));
	render::draw_filled_rect(slider_pos_x, slider_y, slider_len, slider_height, color(36, 36, 36, 255));
	render::draw_filled_rect(slider_pos_x, slider_y, reverse_map, slider_height, color(150, 22, 22, 255));

	// Slider label
	render::draw_text_string(x + 2, y - 1, font, (std::stringstream{ } << string << ": " <<  std::setprecision(3) << value).str(), false, color::white());
}

/*
 * combobox: Will render a combobox with the strings in opt_vec as options, and change &target.idx to the selected item's index in the vector
 * combo_right_pos: top right corner of the selected option indicator.
 * target.toggle: for deciding if this popup is open or not.
 */ 
void gui::combobox(std::int32_t x, std::int32_t y, std::int32_t combo_right_pos, unsigned long font, const std::string label, std::vector<std::string>& opt_vec, combobox_toggle_t& target) {
	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);
	
	const int x_margin = popup_system::combo_win_padding;
	const int h = 11;
	const int arrow_w = 7, arrow_h = 4;
	const int arrow_x = combo_right_pos - x_margin - arrow_w, arrow_y = y + 4;	// h/2 is not reliable
	const int item_w = render::get_text_size(render::fonts::watermark_font_ns, opt_vec.at(target.idx)).x;
	int w = x_margin + item_w + x_margin + arrow_w + x_margin;
	// Stores the px width of the biggest text in the vector if popup is active
	if (target.toggle) {
		for (std::string item : opt_vec) {
			int text_w = render::get_text_size(render::fonts::watermark_font_ns, item).x + x_margin * 2;
			if (text_w > w)
				w = text_w;
		}
	}
	const int position = combo_right_pos - w;		// Get top left corner of current item

	// The bad thing about mouse_in_popup is that you can only check for popups after they are generated (You pop the items when rendering from the vector)
	if (!popup_system::mouse_in_popup(cursor.x, cursor.y) && input::gobal_input.IsPressed(VK_LBUTTON)) {
		if ((cursor.x >= position) && (cursor.x <= position + w) && (cursor.y >= y) && (cursor.y <= y + h))
			target.toggle = !target.toggle;			// If in checkbox and clicked
		// See color picker comment
		else if ( !((cursor.x >= position) && (cursor.x <= position + w) && (cursor.y >= y) && (cursor.y <= y + h + opt_vec.size() * 15)) )
			target.toggle = false;			// Close popup if user clicks outside
	}

	// Combobox "button"
	render::draw_filled_rect(position, y - 1, w, h + 2, color(36, 36, 36, 255));
	render::draw_text_string(position + x_margin, y - 1, render::fonts::watermark_font_ns, opt_vec.at(target.idx), false, color::white());
	
	// Draw arrow
	for (int n = 0; n < arrow_h; n++) {
		render::draw_filled_rect(arrow_x + n, arrow_y + n, arrow_w - n * 2, 1, color::white());
	}

	// Combobox label
	render::draw_text_string(x + 2, y - 1, font, label, false, color::white());

	// Push to vector to render after menu
	if (target.toggle)
		popup_system::active_combo_popups.push_back(combo_popup_info{ position, y + h + 1, w, opt_vec.size() * 15, opt_vec, target.idx, target.toggle });
}

// Same as combobox but with more than one option. Will store selected options as true in a bool vector (target.vector).
void gui::multicombobox(std::int32_t x, std::int32_t y, std::int32_t combo_right_pos, unsigned long font, const std::string label, multicombobox_toggle_t& target) {
	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);

	const int x_margin = popup_system::combo_win_padding;
	const int h = 11;
	const int arrow_w = 7, arrow_h = 4;
	const int arrow_x = combo_right_pos - x_margin - arrow_w, arrow_y = y + 4;	// h/2 is not reliable
	const int max_button_len = 100;
	std::string button_str = "";
	bool has_enabled = false;	// Will be false if all options are disabled
	for (multicombo_opt_t item : target.vector) {
		std::string temp_str = button_str;
		if (item.state) {
			if (has_enabled) temp_str += ", ";		// The check makes it not add a comma before the first item
			temp_str += item.text;
			if (render::get_text_size(render::fonts::watermark_font_ns, temp_str).x < max_button_len) {
				if (has_enabled) button_str += ", ";
				button_str += item.text;
			} else {
				button_str += ", ...";
				break;
			}
			has_enabled = true;		// We found an enabled item
		}
	}
	if (!has_enabled) button_str = "None";
	const int item_w = render::get_text_size(render::fonts::watermark_font_ns, button_str).x;	// Need to get text first (selected options)
	int w = x_margin + item_w + x_margin + arrow_w + x_margin;
	if (target.toggle) {		// Stores the px width of the biggest text in the vector if popup is active
		for (multicombo_opt_t item : target.vector) {
			int text_w = render::get_text_size(render::fonts::watermark_font_ns, item.text).x + x_margin * 2;
			if (text_w > w)
				w = text_w;
		}
	}
	const int position = combo_right_pos - w;		// Get top left corner of current item

	// The bad thing about mouse_in_popup is that you can only check for popups after they are generated (You pop the items when rendering from the vector)
	if (!popup_system::mouse_in_popup(cursor.x, cursor.y) && input::gobal_input.IsPressed(VK_LBUTTON)) {
		if ((cursor.x >= position) && (cursor.x <= position + w) && (cursor.y >= y) && (cursor.y <= y + h))
			target.toggle = !target.toggle;			// If in checkbox and clicked
		// See color picker comment
		else if (!((cursor.x >= position) && (cursor.x <= position + w) && (cursor.y >= y) && (cursor.y <= y + h + target.vector.size() * 15)))
			target.toggle = false;					// Close popup if user clicks outside
	}

	// Combobox "button"
	render::draw_filled_rect(position, y - 1, w, h + 2, color(36, 36, 36, 255));
	render::draw_text_string(position + x_margin, y - 1, render::fonts::watermark_font_ns, button_str, false, color::white());

	// Draw arrow
	for (int n = 0; n < arrow_h; n++) {
		render::draw_filled_rect(arrow_x + n, arrow_y + n, arrow_w - n * 2, 1, color::white());
	}

	// Combobox label
	render::draw_text_string(x + 2, y - 1, font, label, false, color::white());

	// Push to vector to render after menu
	if (target.toggle)
		popup_system::active_multicombo_popups.push_back(multicombo_popup_info{ position, y + h + 2, w, target.vector.size() * 15, target.vector, target.toggle });
}

/*
 * hotkey: Will scan for keys and will change the target_key value to the scanned (latest pressed) virtual key code
 * reading_this_hotkey is needed to check if the hotkey we are changing in input::gobal_input.reading_hotkey is the same as the one in this option
 */
void gui::hotkey(std::int32_t x, std::int32_t y, std::int32_t w, unsigned long font, const std::string string, int& target_key, bool& reading_this_hotkey) {
	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);

	const int h = 11;
	static bool should_skip_frame = false;		// Will ignore hotkey if true. Used to avoid detecting the click itself

	if (!popup_system::mouse_in_popup(cursor.x, cursor.y)) {
		// If in hotkey button or text and clicked (but was not reading a hotkey)
		if ((cursor.x >= x) && (cursor.x <= x + w) && (cursor.y >= y - 1) && (cursor.y <= y + h + 1) && !reading_this_hotkey) {
			if (input::gobal_input.IsPressed(VK_LBUTTON)) {
				input::gobal_input.reading_hotkey = true;
				reading_this_hotkey				  = true;
				should_skip_frame				  = true;
			} else if (input::gobal_input.IsPressed(VK_DELETE)) {		// We can delete the hotkey without "reading it". Just hovering and pressing delete
				target_key = INPUT_KEY_NONE;								// When a hotkey is none, it will apear as pressed all the time
			}
		}
	}

	if (input::gobal_input.reading_hotkey && reading_this_hotkey && !should_skip_frame) {
		const int newkey = input::gobal_input.LatestPressed();
		if (newkey != INPUT_KEY_WAITING) {							// -1 means there is no new keypress
			if (newkey == VK_DELETE) {							// Delte will remove the hotkey
				target_key = INPUT_KEY_NONE;						// When a hotkey is none, it will apear as pressed all the time
			} else if (newkey != VK_ESCAPE) {					// Did not press scape (cancel hotkey).
				target_key = newkey;							// Store key
				input::gobal_input.latest_hotkey = newkey;		// Store to avoid toggling the key when assigning
			}

			input::gobal_input.reading_hotkey = false;		// We are no longer waiting for hotkeys
			reading_this_hotkey               = false;		// And we don't have to worry about wich hotkey are we reading
		}
	} else if (should_skip_frame) {
		should_skip_frame = false;		// We skipped so reset
	}

	// Key text
	std::string key_name    = (reading_this_hotkey) ? input::key_names[INPUT_KEY_WAITING] : input::key_names[target_key];		// Defined in global_input.hpp
	std::string display_key = "[" + key_name + "]";
	int tw, th;		// Text's width and height
	interfaces::surface->get_text_size(font, std::wstring(display_key.begin(), display_key.end()).c_str(), tw, th); // Get w for getting the top left corner of txt
	render::draw_text_string(x + w - tw, y - 1, font, display_key, false, color(210, 210, 210));		// Patoke if you tell me its 1px down I will kill a small animal
	
	// Description (label)
	render::draw_text_string(x + 2, y - 1, font, string, false, color::white());
}

// Same as the other hotkey but using struct
void gui::hotkey(std::int32_t x, std::int32_t y, std::int32_t w, unsigned long font, const std::string string, hotkey_t& hotkey_info) {
	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);

	const int h = 11;
	static bool should_skip_frame = false;		// Will ignore hotkey if true. Used to avoid detecting the click itself

	if (!popup_system::mouse_in_popup(cursor.x, cursor.y)) {
		// If in hotkey button or text and clicked (but was not reading a hotkey)
		if ((cursor.x >= x) && (cursor.x <= x + w) && (cursor.y >= y - 1) && (cursor.y <= y + h + 1) && !hotkey_info.reading_this) {
			if (input::gobal_input.IsPressed(VK_LBUTTON)) {
				input::gobal_input.reading_hotkey = true;
				hotkey_info.reading_this          = true;
				should_skip_frame                 = true;
			} else if (input::gobal_input.IsPressed(VK_DELETE)) {		// We can delete the hotkey without "reading it". Just hovering and pressing delete
				hotkey_info.key = INPUT_KEY_NONE;								// When a hotkey is none, it will apear as pressed all the time
			}
		}
	}

	if (input::gobal_input.reading_hotkey && hotkey_info.reading_this && !should_skip_frame) {
		const int newkey = input::gobal_input.LatestPressed();
		if (newkey != INPUT_KEY_WAITING) {							// -1 means there is no new keypress
			if (newkey == VK_DELETE) {							// Delte will remove the hotkey
				hotkey_info.key = INPUT_KEY_NONE;					// When a hotkey is none, it will apear as pressed all the time
			} else if (newkey != VK_ESCAPE) {					// Did not press scape (cancel hotkey).
				hotkey_info.key = newkey;						// Store key
				input::gobal_input.latest_hotkey = newkey;		// Store to avoid toggling the key when assigning
			}

			input::gobal_input.reading_hotkey = false;		// We are no longer waiting for hotkeys
			hotkey_info.reading_this          = false;		// And we don't have to worry about wich hotkey are we reading
		}
	} else if (should_skip_frame) {
		should_skip_frame = false;		// We skipped so reset
	}

	// Key text
	std::string key_name    = (hotkey_info.reading_this) ? input::key_names[INPUT_KEY_WAITING] : input::key_names[hotkey_info.key];	// Defined in global_input.hpp
	std::string display_key = "[" + key_name + "]";
	int tw, th;		// Text's width and height
	interfaces::surface->get_text_size(font, std::wstring(display_key.begin(), display_key.end()).c_str(), tw, th); // Get w for getting the top left corner of txt
	render::draw_text_string(x + w - tw, y - 1, font, display_key, false, color(210, 210, 210));		// Patoke if you tell me its 1px down I will kill a small animal

	// Description (label)
	render::draw_text_string(x + 2, y - 1, font, string, false, color::white());
}

void gui::textbox(std::int32_t x, std::int32_t y, std::int32_t w, unsigned long font, const std::string placeholder, textbox_t& textbox_info, void(*button_callback)(std::string)) {
	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);

	constexpr int h = 11;
	constexpr int max_txt_len = 255;
	constexpr int button_w = 30;
	constexpr int margin = 5;
	const int text_box_w = w - button_w - margin;
	const int button_x = x + text_box_w + margin;

	if (!popup_system::mouse_in_popup(cursor.x, cursor.y) && input::gobal_input.IsPressed(VK_LBUTTON)) {
		// If in text area and clicked
		if ((cursor.x >= x) && (cursor.x <= x + text_box_w) && (cursor.y >= y - 1) && (cursor.y <= y + h + 1)) {
			if (!input::gobal_input.reading_textbox) {
				input::gobal_input.wndproc_textbox_buffer = textbox_info.text;		// Move current text to buffer cuz we reading
				input::gobal_input.reading_textbox = true;
				textbox_info.reading_this = true;
			}
		} else {
			input::gobal_input.reading_textbox = false;
			textbox_info.reading_this = false;
		}
	}

	if (input::gobal_input.reading_textbox && textbox_info.reading_this) {
		// Delete will remove last char
		if (input::gobal_input.IsPressed(VK_BACK) && !textbox_info.text.empty()) {
			// Ctrl + Del will clear the textbox
			if (input::gobal_input.IsHeld(VK_CONTROL)) {
				input::gobal_input.wndproc_textbox_buffer = "";
				textbox_info.text = "";
			} else {
				input::gobal_input.wndproc_textbox_buffer.pop_back();
				textbox_info.text.pop_back();
			}
		// If esc or enter, unfocus
		} else if (input::gobal_input.IsHeld(VK_ESCAPE) || input::gobal_input.IsHeld(VK_RETURN)) {
			input::gobal_input.reading_textbox = false;
			textbox_info.reading_this = false;
		// If key is valid, add to string. Gotta thank zgui
		} else if (textbox_info.text.length() < max_txt_len) {
			// Only add if we are not overflowing text input
			if (render::get_text_size(render::fonts::watermark_font, input::gobal_input.wndproc_textbox_buffer).x < text_box_w - margin - 2)	// -2 for cursor
				textbox_info.text = input::gobal_input.wndproc_textbox_buffer;
			// If we are overflowing, reset buffer to current text
			else
				input::gobal_input.wndproc_textbox_buffer = textbox_info.text;
		}
	}

	// Backgound and text contents
	render::draw_filled_rect(x, y - 2, text_box_w, 15, color(15, 15, 15, 255));
	if (textbox_info.text.length() > 0) {
		render::draw_text_string(x + margin, y - 1, render::fonts::watermark_font, textbox_info.text, false, (textbox_info.reading_this) ? color::white(255) : color::white(100));
	} else if (!textbox_info.reading_this) {
		render::draw_text_string(x + margin, y - 1, render::fonts::watermark_font, placeholder, false, color::white(100));
	}

	// Cursor
	if (textbox_info.reading_this) {
		const int text_w = render::get_text_size(render::fonts::watermark_font, textbox_info.text).x;
		render::draw_filled_rect(x + margin + text_w, y, 1, 11, color::white());
	}

	// Button
	if ((cursor.x >= button_x) && (cursor.x <= button_x + button_w) && (cursor.y >= y) && (cursor.y <= y + h)) {
		render::draw_filled_rect(button_x, y, button_w, h, color(115, 21, 21, 255));		// Button background (Hover)
		if (!popup_system::mouse_in_popup(cursor.x, cursor.y) && input::gobal_input.IsPressed(VK_LBUTTON)) {
			button_callback(textbox_info.text);
			textbox_info.text = "";
			input::gobal_input.wndproc_textbox_buffer = "";
		}
	} else {
		render::draw_filled_rect(button_x, y, button_w, h, color(150, 22, 22, 255));		// Button background
	}
}

void gui::config_selection(std::int32_t x, std::int32_t y, std::int32_t w, unsigned long font, std::vector<std::string>& config_names) {
	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);

	constexpr int padding = 10;
	const int h = config_names.size() * 15;		// 15px per line
	
	if ((cursor.x >= x) && (cursor.x <= x + w) && (cursor.y >= y) && (cursor.y < y + h) && input::gobal_input.IsHeld(VK_LBUTTON))		// We are holding inside region
		config::selected_config = (cursor.y - y) / 15;		// Get clicked item based on pos

	// No configs
	if (config_names.size() == 0) {
		render::draw_text_string(x + padding, y, render::fonts::watermark_font_ns,
			"No configs found...", false, color::white(100));
		return;
	}

	// Rectangle for selection
	if (config::selected_config != -1)		// Not default
		render::draw_filled_rect(x + 1, y + config::selected_config * 15 - 1, w - 2, 15, color(15, 15, 15, 255));

	// Draw strings after rectangle
	int item_n = 0;
	for (std::string item : config_names) {
		if (item_n > config::max_configs) break;

		// Make sure we dont overflow. *1.5 to leave a bit of margin at the end
		if (render::get_text_size(render::fonts::watermark_font_ns, item).x >= w - padding * 1.5) {
			std::string buffer = "";

			// Keep adding chars until we reach the maximum w
			for (char c : item) {
				buffer.push_back(c);
				if (render::get_text_size(render::fonts::watermark_font_ns, buffer + "...").x >= w - padding * 1.5) {
					buffer.pop_back();		// Remove last char cuz it is too long already
					buffer += "...";		// Add 3 dots to indicate the filename is longer
					break;					// We are done
				}
			}

			item = buffer;					// Set the new string
		}

		render::draw_text_string(x + padding, y + (15 * item_n), render::fonts::watermark_font_ns, item, false, (item_n == config::selected_config) ? color::white() : color::white(100));
		item_n++;
	}
}
#pragma endregion

#pragma region DYNAMIC GUI
/* --------------------------- DYNAMIC GUI --------------------------- */

// Updates item positions with new menu positons and all that. Called before the tab switch statement
void gui::update_positions() {
	vars::o_container_left_pos = variables::ui::menu::x + vars::container_margin;			// Will change when adding more columns
	vars::o_container_width    = variables::ui::menu::w - vars::container_margin * 2;		// Will get divided when adding more columns
	vars::o_item_left_pos      = vars::o_container_left_pos + vars::container_padding;		// Base top left pos for all items (label text position)
	
	vars::o_item_combo_pos     = variables::ui::menu::x + vars::o_container_width - vars::container_margin;		// Max right pos
	vars::o_item_checkbox_pos  = vars::o_item_combo_pos - vars::item_checkbox_length;
	vars::o_item_slider_pos    = vars::o_item_combo_pos - vars::item_slider_length;				// Top left corner of the actual slider
	vars::o_item_hotkey_w      = vars::o_container_width - vars::container_padding * 2;

	// Goupbox vars
	vars::cur_part_items       = 0;		// Will get updated on the add_group_box() calls anyway
	vars::cur_part_h           = 0;
	vars::o_cur_part_y         = variables::ui::menu::y + vars::top_margin_with_tabs;		// No container margin because we will add it in add_groupbox()
	vars::o_cur_base_item_y    = vars::o_cur_part_y + vars::container_padding;

	vars::button_part_item     = 0;
	vars::button_part_h        = 0;			// Need to get h first to subtract it from bottom to get top pos
	vars::button_part_y        = variables::ui::menu::y + variables::ui::menu::h - vars::container_margin;
	vars::button_base_item_y   = vars::button_part_y + vars::container_padding;
}

// Reset values to its original ones
void gui::init_tab() {
	vars::column_number = 0;

	vars::container_left_pos	= vars::o_container_left_pos;
	vars::container_width		= (vars::o_container_width / vars::columns) - (vars::container_margin / vars::columns);

	// We need to assign them again instead of using the originals because the container widht changed
	vars::item_left_pos			= vars::container_left_pos + vars::container_padding;
	vars::item_combo_pos		= variables::ui::menu::x + vars::container_width - vars::container_margin;
	vars::item_checkbox_pos		= vars::item_combo_pos - vars::item_checkbox_length;
	vars::item_slider_pos		= vars::item_combo_pos - vars::item_slider_length;
	vars::item_hotkey_w			= vars::container_width - vars::container_padding * 2;

	vars::cur_part_y            = vars::o_cur_part_y;
	vars::cur_base_item_y       = vars::o_cur_base_item_y;
}

// Adds a column and changes values depending on current column
void gui::add_column() {
	vars::column_number++;

	if (vars::column_number > 0) {		// Only if we are not on the first col
		vars::container_width--;
		vars::cur_part_h            = 0;

		vars::container_left_pos	= vars::container_left_pos + (vars::container_width * vars::column_number) + vars::container_margin;
		vars::item_left_pos			= vars::item_left_pos + (vars::container_width * vars::column_number) + vars::container_margin;
		vars::item_checkbox_pos		= vars::item_checkbox_pos + (vars::container_width * vars::column_number) + vars::container_margin;
		vars::item_slider_pos		= vars::item_slider_pos + (vars::container_width * vars::column_number) + vars::container_margin;
		vars::item_combo_pos		= vars::item_checkbox_pos + vars::item_checkbox_length;

		vars::cur_part_y      = vars::o_cur_part_y;				// We reset the y positions of the goupbox so they start on top of the second column
		vars::cur_base_item_y = vars::o_cur_base_item_y;
	}
}

void gui::add_groupbox(int item_number) {
	vars::cur_part_items  = item_number;
	vars::cur_part_y     += vars::cur_part_h + vars::container_margin;		// cur_part_h is the "previous" part h
	vars::cur_base_item_y = vars::cur_part_y + vars::container_padding;
	vars::cur_part_h      = (15 * vars::cur_part_items) + (vars::container_padding * 2) - 4;		// This for now, but should be increased with the items added
}

void gui::add_groupbox(std::string name, int item_number) {
	vars::cur_part_items = item_number;
	vars::cur_part_y += vars::cur_part_h + vars::container_margin;		// cur_part_h is the "previous" part h
	vars::cur_base_item_y = vars::cur_part_y + vars::container_padding;
	vars::cur_part_h = (15 * vars::cur_part_items) + (vars::container_padding * 2) - 4;		// This for now, but should be increased with the items added

	gui::groupbox(gui::vars::container_left_pos, gui::vars::cur_part_y, gui::vars::container_width, gui::vars::cur_part_h, render::fonts::watermark_font, name, false);
}

void gui::add_bottom_groupbox(int item_number) {
	vars::button_part_item     = item_number;
	vars::button_part_h        = (vars::button_part_item * 15) + (vars::container_padding * 2) - 4;
	vars::button_part_y        = variables::ui::menu::y + variables::ui::menu::h - vars::container_margin - vars::button_part_h;		// Get the top left corner based on the margin pos and the height (start from bottom)
	vars::button_base_item_y   = vars::button_part_y + vars::container_padding;			// Same as other containers
}

void gui::add_checkbox(std::string label, bool& target, unsigned long font) {
	// @todo: Should update cur_part_h with += item_h
	gui::check_box(gui::vars::item_left_pos, gui::vars::cur_base_item_y, gui::vars::item_checkbox_pos, font, label, target);
	gui::vars::cur_base_item_y += 15;		// Add 15 to the variable
}

void gui::add_checkbox(std::string label, bool& target) {
	add_checkbox(label, target, render::fonts::watermark_font);
}

void gui::add_checkbox(std::string label, bool& target, colorpicker_col_t& color, unsigned long font) {
	gui::check_box(gui::vars::item_left_pos, gui::vars::cur_base_item_y, gui::vars::item_checkbox_pos, font, label, target, color);
	gui::vars::cur_base_item_y += 15;
}

void gui::add_checkbox(std::string label, bool& target, colorpicker_col_t& color) {
	add_checkbox(label, target, color, render::fonts::watermark_font);
}

void gui::add_checkbox(std::string label, bool& target, colorpicker_col_t& color1, colorpicker_col_t& color2, unsigned long font) {
	gui::check_box(gui::vars::item_left_pos, gui::vars::cur_base_item_y, gui::vars::item_checkbox_pos, font, label, target, color1, color2);
	gui::vars::cur_base_item_y += 15;
}

void gui::add_checkbox(std::string label, bool& target, colorpicker_col_t& color1, colorpicker_col_t& color2) {
	add_checkbox(label, target, color1, color2, render::fonts::watermark_font);
}

void gui::add_slider(std::string label, float& target, float min_value, float max_value, unsigned long font) {
	gui::slider(gui::vars::item_left_pos, gui::vars::cur_base_item_y, gui::vars::item_slider_pos, gui::vars::item_slider_length, font, label, target, min_value, max_value);
	gui::vars::cur_base_item_y += 15;
}

void gui::add_slider(std::string label, float& target, float min_value, float max_value) {
	add_slider(label, target, min_value, max_value, render::fonts::watermark_font);
}

void gui::add_combobox(std::string label, std::vector<std::string>& option_vector, combobox_toggle_t& target, unsigned long font) {
	gui::combobox(gui::vars::item_left_pos, gui::vars::cur_base_item_y, gui::vars::item_combo_pos, font, label, option_vector, target);
	gui::vars::cur_base_item_y += 15;
}

void gui::add_combobox(std::string label, std::vector<std::string>& option_vector, combobox_toggle_t& target) {
	add_combobox(label, option_vector, target, render::fonts::watermark_font);
}

void gui::add_multicombobox(std::string label, multicombobox_toggle_t& target, unsigned long font) {
	gui::multicombobox(gui::vars::item_left_pos, gui::vars::cur_base_item_y, gui::vars::item_combo_pos, font, label, target);
	gui::vars::cur_base_item_y += 15;
}

void gui::add_multicombobox(std::string label, multicombobox_toggle_t& target) {
	add_multicombobox(label, target, render::fonts::watermark_font);
}

void gui::add_hotkey(const std::string label, int& target_key, int& target, bool& reading_this, unsigned long font) {
	gui::hotkey(gui::vars::item_left_pos, gui::vars::cur_base_item_y, gui::vars::item_hotkey_w, font, label, target, reading_this);
	gui::vars::cur_base_item_y += 15;
}

void gui::add_hotkey(const std::string label, int& target_key, int& target, bool& reading_this) {
	add_hotkey(label, target_key, target, reading_this, render::fonts::watermark_font);
}

void gui::add_hotkey(const std::string label, hotkey_t& hotkey_info, unsigned long font) {
	gui::hotkey(gui::vars::item_left_pos, gui::vars::cur_base_item_y, gui::vars::item_hotkey_w, font, label, hotkey_info);
	gui::vars::cur_base_item_y += 15;
}

void gui::add_hotkey(const std::string label, hotkey_t& hotkey_info) {
	add_hotkey(label, hotkey_info, render::fonts::watermark_font);
}

void gui::add_button(const std::string label, void(*callback)(), unsigned long font) {
	gui::button(gui::vars::item_left_pos, gui::vars::cur_base_item_y, gui::vars::item_combo_pos - vars::item_button_w, font, label, callback);
	gui::vars::cur_base_item_y += 15;
}

void gui::add_button(const std::string label, void(*callback)()) {
	add_button(label, callback, render::fonts::watermark_font);
}

void gui::add_textbox(const std::string placeholder, textbox_t& textbox_info, void(*button_callback)(std::string), unsigned int font) {
	// The -2 and +2 are to move the textbox area a bit, just personal preference
	gui::textbox(gui::vars::item_left_pos - 2, gui::vars::cur_base_item_y, gui::vars::item_hotkey_w + 2, font, placeholder, config::new_config_name, config::create_new_config); 
	gui::vars::cur_base_item_y += 15;
}

void gui::add_textbox(const std::string placeholder, textbox_t& textbox_info, void(*button_callback)(std::string)) {
	add_textbox(placeholder, textbox_info, button_callback, render::fonts::watermark_font);
}
#pragma endregion

#pragma region WINDOW MOVEMENT
/* --------------------------- WINDOW MOVEMENT --------------------------- */

void gui::menu_movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h) {
	if (spectator_framework::user_dragging_spec) return;	// Avoid overlapping

	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);

	if (input::gobal_input.IsHeld(VK_LBUTTON) && (cursor.x > x && cursor.x < x + w && cursor.y > y && cursor.y < y + h)) {
		user_dragging_menu = true;

		if (!should_move_menu) {
			cursor_corrected.x = cursor.x - x;
			cursor_corrected.y = cursor.y - y;
			should_move_menu = true;
		}
	}
	
	if (user_dragging_menu) {
		x = cursor.x - cursor_corrected.x;
		y = cursor.y - cursor_corrected.y;
	}
	
	if (!input::gobal_input.IsHeld(VK_LBUTTON)) {
		user_dragging_menu = false;
		should_move_menu = false;
	}
}

void spectator_framework::spec_list_movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h) {
	if (gui::user_dragging_menu) return;		// Avoid overlapping
	
	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);

	if (input::gobal_input.IsHeld(VK_LBUTTON) && (cursor.x > x && cursor.x < x + w && cursor.y > y && cursor.y < y + h)) {
		user_dragging_spec = true;

		if (!should_move_spec) {
			cursor_corrected.x = cursor.x - x;
			cursor_corrected.y = cursor.y - y;
			should_move_spec = true;
		}
	}

	if (user_dragging_spec) {
		x = cursor.x - cursor_corrected.x;
		y = cursor.y - cursor_corrected.y;
	}

	if (!input::gobal_input.IsHeld(VK_LBUTTON)) {
		user_dragging_spec = false;
		should_move_spec = false;
	}
}
#pragma endregion

#pragma region POPUPS
/* --------------------------- POPUPS --------------------------- */

// Will call each check_popups()
void popup_system::render_popups() {	
	check_color_popups();
	check_combo_popups();
	check_multicombo_popups();
}

// Checks if the mouse is in an active popup
bool popup_system::mouse_in_popup(int x, int y) {
	// For each color popup in the active_color_popups vector
	for (const color_popup_info& pinfo : active_color_popups) {
		if ( pinfo.col_t.toggle && (x >= pinfo.x) && (x <= pinfo.x + popup_system::win_w) && (y >= pinfo.y) && (y <= pinfo.y + popup_system::win_h) )
			return true;
	}

	for (const combo_popup_info& pinfo : active_combo_popups) {
		if ( pinfo.popup_toggle && (x >= pinfo.x) && (x <= pinfo.x + pinfo.w) && (y >= pinfo.y) && (y <= pinfo.y + pinfo.h) )
			return true;
	}

	for (const multicombo_popup_info& pinfo : active_multicombo_popups) {
		if (pinfo.popup_toggle && (x >= pinfo.x) && (x <= pinfo.x + pinfo.w) && (y >= pinfo.y) && (y <= pinfo.y + pinfo.h))
			return true;
	}

	return false;
}

// Will check for popups in the active_color_popups vector
void popup_system::check_color_popups() {
	// Render each active popup and pop from vector until there are no popups left
	while (!active_color_popups.empty()) {
		color_picker_popup(active_color_popups.back());		// Render
		active_color_popups.pop_back();						// Remove
	}
}

// Will check for popups in the active_combo_popups vector
void popup_system::check_combo_popups() {
	// Render each active popup and pop from vector until there are no popups left
	while (!active_combo_popups.empty()) {
		combobox_popup(active_combo_popups.back());			// Render
		active_combo_popups.pop_back();						// Remove
	}
}

// Will check for popups in the active_combo_popups vector
void popup_system::check_multicombo_popups() {
	// Render each active popup and pop from vector until there are no popups left
	while (!active_multicombo_popups.empty()) {
		multicombobox_popup(active_multicombo_popups.back());	// Render
		active_multicombo_popups.pop_back();				// Remove
	}
}

// Actual popup for the color picker hue and all that
void popup_system::color_picker_popup(color_popup_info col_p) {
	if (!col_p.col_t.toggle) return;
	
	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);

	const int slider_x = col_p.x + win_padding;
	int slider_y = col_p.y + win_padding;		// Will change when adding more sliders to the popup

	render::draw_filled_rect(col_p.x, col_p.y, win_w, win_h, color(36, 36, 36, 255));
	render::draw_rect(col_p.x, col_p.y, win_w, win_h, color::black(255));

	static color hueColors[7] = {
		{ 255, 0,   0   },
		{ 255, 255, 0   },
		{ 0,   255, 0   },
		{ 0,   255, 255 },
		{ 0,   0,   255 },
		{ 255, 0,   255 },
		{ 255, 0,   0   }
	};

	float_hsv hsv_buffer = col_p.col_t.f_hsv;

	#pragma region HUE BAR
	// Draw hsv fades (6 segments)
	for (auto n = 0; n < 6; n++) {
		const int fade_w = slider_w / 6;
		const int fade_x = slider_x + (fade_w * n);

		render::draw_fade(fade_x, slider_y, fade_w, slider_h, hueColors[n], hueColors[n + 1], true);
	}
	
	// Check selected hue (mouse in slider)
	float color_hue = hsv_buffer.h;
	if ((cursor.x >= slider_x) && (cursor.x <= slider_x + slider_w) && (cursor.y >= slider_y) && (cursor.y < slider_y + slider_h) && input::gobal_input.IsHeld(VK_LBUTTON)) {
		color_hue = float(cursor.x - slider_x) / float(slider_w);
		color_hue = (color_hue == 1.f) ? 0.99 : color_hue;		// If max slider value, subtract 1 color (max value is the same as min value in rgb, so when converting back the slider reset to 0)
		hsv_buffer.h = color_hue;
	}
	
	// Render color selector depenging on the color's hue
	render::draw_rect(slider_x + slider_w * color_hue - 1, slider_y - 1, 3, slider_h + 2, color::white(255));
	#pragma endregion

	#pragma region SATURATION BAR
	slider_y += win_padding + slider_h;
	
	const color lowest_sat = helpers::colors::hsv_float2color({ color_hue, 0.f, 1.f }, 255);
	const color highest_sat = helpers::colors::hsv_float2color({ color_hue, 1.f, 1.f }, 255);
	render::draw_fade(slider_x, slider_y, slider_w, slider_h, lowest_sat, highest_sat, true);

	// Check selected hue (mouse in slider)
	float color_sat = hsv_buffer.s;
	if ((cursor.x >= slider_x) && (cursor.x <= slider_x + slider_w) && (cursor.y >= slider_y) && (cursor.y <= slider_y + slider_h) && input::gobal_input.IsHeld(VK_LBUTTON)) {
		color_sat = float(cursor.x - slider_x) / float(slider_w);
		color_sat = (color_sat == 0.f) ? 0.01 : color_sat;
		hsv_buffer.s = color_sat;
	}

	// Render color selector depenging on the color's hue
	render::draw_rect(slider_x + slider_w * color_sat - 1, slider_y - 1, 3, slider_h + 2, color::white(255));
	#pragma endregion

	#pragma region HSV VALUE BAR
	slider_y += win_padding + slider_h;

	const color lowest_val = helpers::colors::hsv_float2color({ color_hue, 1.f, 0.f }, 255);
	const color highest_val = helpers::colors::hsv_float2color({ color_hue, 1.f, 1.f }, 255);
	render::draw_fade(slider_x, slider_y, slider_w, slider_h, lowest_val, highest_val, true);

	// Check selected hue (mouse in slider)
	float color_val = hsv_buffer.v;
	if ((cursor.x >= slider_x) && (cursor.x <= slider_x + slider_w) && (cursor.y >= slider_y) && (cursor.y <= slider_y + slider_h) && input::gobal_input.IsHeld(VK_LBUTTON)) {
		color_val = float(cursor.x - slider_x) / float(slider_w);
		color_val = (color_val == 0.f) ? 0.01 : color_val;
		hsv_buffer.v = color_val;
	}

	// Render color selector depenging on the color's hue
	render::draw_rect(slider_x + slider_w * color_val - 1, slider_y - 1, 3, slider_h + 2, color::white(255));
	#pragma endregion

	#pragma region ALPHA BAR
	slider_y += win_padding + slider_h;

	render::draw_fade(slider_x, slider_y, slider_w, slider_h, col_p.col_t.col.get_custom_alpha(0), col_p.col_t.col.get_custom_alpha(255), true);
	
	// Check selected hue (mouse in slider)
	if ((cursor.x >= slider_x) && (cursor.x <= slider_x + slider_w) && (cursor.y >= slider_y) && (cursor.y <= slider_y + slider_h) && input::gobal_input.IsHeld(VK_LBUTTON)) {
		float input_alpha = float(cursor.x - slider_x) / float(slider_w);
		col_p.col_t.col.a = input_alpha * 255.f;
	}

	// Render color selector depenging on the color's hue
	float color_alpha = col_p.col_t.col.a / 255.f;
	render::draw_rect(slider_x + slider_w * color_alpha - 1, slider_y - 1, 3, slider_h + 2, color::white(255));
	#pragma endregion

	// Set new color replacing changed values
	col_p.col_t.f_hsv = hsv_buffer;
	col_p.col_t.col = helpers::colors::hsv_float2color(hsv_buffer, col_p.col_t.col.a);
}

void popup_system::combobox_popup(combo_popup_info combo_p) {
	if (!combo_p.popup_toggle) return;

	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);
	
	// We get the largest item width when rendering the button, and we pass it to the combo_p, so we just use that
	render::draw_filled_rect(combo_p.x, combo_p.y, combo_p.w, combo_p.h, color(30, 30, 30, 255));
	render::draw_filled_rect(combo_p.x, combo_p.y + combo_p.target_idx * 15 - 1, combo_p.w, 15, color(20, 20, 20, 255));

	int item_n = 0;
	for (std::string item : combo_p.opt_vec) {
		render::draw_text_string(combo_p.x + combo_win_padding, combo_p.y + (15 * item_n), render::fonts::watermark_font_ns, item, false, color::white());
		item_n++;
	}

	if ((cursor.x >= combo_p.x) && (cursor.x <= combo_p.x + combo_p.w) && (cursor.y >= combo_p.y) && (cursor.y < combo_p.y + combo_p.h) && input::gobal_input.IsHeld(VK_LBUTTON))
		combo_p.target_idx = (cursor.y - combo_p.y) / 15;		// Get clicked item
}

void popup_system::multicombobox_popup(multicombo_popup_info combo_p) {
	if (!combo_p.popup_toggle) return;

	interfaces::surface->surface_get_cursor_pos(cursor.x, cursor.y);

	// We get the largest item width when rendering the button, and we pass it to the combo_p, so we just use that
	render::draw_filled_rect(combo_p.x, combo_p.y, combo_p.w, combo_p.h, color(30, 30, 30, 255));
	int cur_option = 0;
	for (multicombo_opt_t item : combo_p.target_vec) {
		if (item.state)
			render::draw_filled_rect(combo_p.x, combo_p.y + cur_option * 15 - 1, combo_p.w, 15, color(20, 20, 20, 255));
		cur_option++;
	}

	int item_n = 0;
	for (multicombo_opt_t item : combo_p.target_vec) {
		render::draw_text_string(combo_p.x + combo_win_padding, combo_p.y + (15 * item_n), render::fonts::watermark_font_ns, item.text, false, item.state ? color(200, 10, 10, 255) : color(190, 190, 190, 255));
		item_n++;
	}

	if ((cursor.x >= combo_p.x) && (cursor.x <= combo_p.x + combo_p.w) && (cursor.y >= combo_p.y) && (cursor.y < combo_p.y + combo_p.h) && input::gobal_input.IsPressed(VK_LBUTTON)) {
		int clicked_idx = (cursor.y - combo_p.y) / 15;		// Get clicked item
		combo_p.target_vec.at(clicked_idx).state = !combo_p.target_vec.at(clicked_idx).state;
	}
}
#pragma endregion