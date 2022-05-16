#include "framework.hpp"

POINT cursor;
POINT cursor_corrected;

// Returns true if pressed
bool gui::button_bool(std::int32_t x, std::int32_t y, std::int32_t butt_pos, unsigned long font, const std::string label) {
	GetCursorPos(&cursor);

	const int w = 30, h = 10;	// Button size
	const color c_default = color(150, 22, 22, 255);
	const color c_hover = color(135, 21, 21, 255);
	bool pressed = false;

	// Checkbox label
	render::draw_text_string(x + 2, y - 1, font, label, false, color::white());

	// Cursor in button and clicked
	if ((cursor.x > butt_pos) && (cursor.x < butt_pos + w) && (cursor.y > y) && (cursor.y < y + h)) {
		render::draw_filled_rect(butt_pos, y, w, h, color(115, 21, 21, 255));		// Checkbox background (Hover)
		pressed = GetAsyncKeyState(VK_LBUTTON) & 1;
	}
	else render::draw_filled_rect(butt_pos, y, w, h, color(150, 22, 22, 255));		// Checkbox background

	return pressed;
}

// Second implementation for button, it pass callback function
void gui::button(std::int32_t x, std::int32_t y, std::int32_t butt_pos, unsigned long font, const std::string label, void(*callback)()) {
	const bool pressed = button_bool(x, y, butt_pos, font, label);		// Call the bool function instead of copying the code again
	if (pressed) callback();
}

void gui::id_changer(std::int32_t x, std::int32_t y, std::int32_t right_position, int val_cont_w, unsigned long font, const std::string label, int& target, int min, int max) {
	GetCursorPos(&cursor);

	const int button_margins = 2;									// After first button and before second
	const int bw = 11, bh = 10;										// Increase and decrease buttons
	const int br_x = right_position - bw;							// Right button - Increase
	const int val_cont_x = br_x - button_margins - val_cont_w;		// Value container
	const int bl_x = val_cont_x - button_margins - bw;				// Left button - Decrease

	color bl_col = color(150, 22, 22, 255);
	color br_col = color(150, 22, 22, 255);

	if ((cursor.x > bl_x) && (cursor.x < bl_x + bw) && (cursor.y > y) && (cursor.y < y + bh)) {
		bl_col = color(115, 21, 21, 255);					// Hover
		if (GetAsyncKeyState(VK_LBUTTON) & 1)
			target = (target == min) ? max : target - 1;	// Decrease
	} else if ((cursor.x > br_x) && (cursor.x < br_x + bw) && (cursor.y > y) && (cursor.y < y + bh)) {
		br_col = color(115, 21, 21, 255);					// Hover
		if (GetAsyncKeyState(VK_LBUTTON) & 1)
			target = (target == max) ? min : target + 1;	// Increase
	}

	// Increase and decrease buttons
	render::draw_filled_rect(bl_x, y, bw, bh, bl_col);										// Normal color
	render::draw_text_string(bl_x + bw / 2  + 1, y - 1, font, "-", true, color::white());	// Button text
	render::draw_filled_rect(br_x, y, bw, bh, br_col);										// Normal color
	render::draw_text_string(br_x + bw / 2, y - 1, font, "+", true, color::white());		// Button text

	// Value container
	//render::draw_filled_rect(val_cont_x, y, val_cont_w, bh, color(150, 22, 22, 255));
	render::draw_text_string(val_cont_x + val_cont_w / 2, y - 1, font, std::to_string(target), true, color::white());

	// Label
	render::draw_text_string(x + 2, y - 1, font, label, false, color::white());
}

void gui::group_box(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, bool show_label) {
	// Groupbox background
	render::draw_filled_rect(x, y, w, h, color(25, 25, 25, 255));

	// Groupbox outline
	render::draw_rect(x, y, w, h, color(45, 45, 45, 255));

	// Groupbox label
	if (show_label)
		render::draw_text_string(x + 2, y - 12, font, string, false, color::white());
}

void gui::tab(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, std::int32_t& tab, std::int32_t count) {
	GetCursorPos(&cursor);

	if ((cursor.x > x) && (cursor.x < x + w) && (cursor.y > y) && (cursor.y < y + h) && (GetAsyncKeyState(VK_LBUTTON) & 1))
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
	GetCursorPos(&cursor);

	const int w = 10, h = 10;

	switch (click_area_id) {
		default:
		case 0: {	// Only checkbox
			if ((cursor.x > position) && (cursor.x < position + w) && (cursor.y > y) && (cursor.y < y + h) && GetAsyncKeyState(VK_LBUTTON) & 1)
				value = !value;		// If in checkbox and clicked
			break;
		}
		case 1: {	// Name and checkbox, not color
			if (((cursor.x > position) && (cursor.x < position + w) && (cursor.y > y) && (cursor.y < y + h)		// Checkbox
				|| (cursor.x > x) && (cursor.x < position - 55) && (cursor.y > y) && (cursor.y < y + h))		// Name and all that. (5 + 20 + 5 + 20 + 5)
				&& GetAsyncKeyState(VK_LBUTTON) & 1)
				value = !value;
			break;
		}
		case 2: {	// All width from name to checkbox
			if ((cursor.x > x) && (cursor.x < position + w) && (cursor.y > y - 1) && (cursor.y < y + h + 1) && GetAsyncKeyState(VK_LBUTTON) & 1)
				value = !value;		// If in checkbox or text and clicked
			break;
		}
	}

	// Checkbox itself
	render::draw_filled_rect(position, y, w, h, value ? color(150, 22, 22, 255) : color(36, 36, 36, 255));

	// Checkbox label
	render::draw_text_string(x + 2, y - 1, font, string, false, color::white());
}

// Checkbox with color picker
void gui::check_box(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value, color& setting_color, bool& toggle_color) {
	check_box(x, y, position, font, string, value, 1);		// Call normal checkbox with no button toggle on color region
	
	const int margin = 5;
	const int w = 20, h = 10;
	const int color_x = position - margin - w;

	if ((cursor.x > color_x) && (cursor.x < color_x + w) && (cursor.y > y) && (cursor.y < y + h) && GetAsyncKeyState(VK_LBUTTON) & 1) {
		toggle_color = !toggle_color;
	}

	render::draw_filled_rect(color_x, y, w, h, setting_color);					// Color itself
	render::draw_rect(color_x - 1, y - 1, w + 2, h + 2, color::black(255));		// Color outline

	if (toggle_color) {
		color_picker_popup(color_x, y + h + margin, setting_color, toggle_color);
	}
}

// Actual popup for the color picker hue and all that
void gui::color_picker_popup(std::int32_t x, std::int32_t y, color& target, bool& toggle_color) {
	const int slider_w = 100, slider_h = 15;
	const int win_margin = 5;
	const int win_w = slider_w + win_margin * 2;
	const int win_h = slider_h * 2 + win_margin * 3;	// +1 slider and margin for alpha slider

	// Close popup if user clicks outside
	if (!( (cursor.x > x) && (cursor.x < x + win_w) && (cursor.y > y) && (cursor.y < y + win_h) ) && GetAsyncKeyState(VK_LBUTTON) & 1) {
		toggle_color = false;
	}

	render::draw_filled_rect(x, y, win_w, win_h, color(36, 36, 36, 255));
	render::draw_rect(x, y, win_w, win_h, color::black(255));

	/*
	for (auto i = 0; i < 6; ++i)
	{
		const SRect tmpHueRect =
		{
			rcHueBar.left,
			int(rcHueBar.top + i * (float(rcPicker.Height()) / 6.f)),
			rcHueBar.right,
			int(rcHueBar.top + (i + 1) * (float(rcPicker.Height()) / 6.f))
		};

		g_Render.RectFilledGradient(tmpHueRect, hueColors[i], hueColors[i + 1], GradientType::GRADIENT_VERTICAL);
	}
	*/
}

// Thanks to https://github.com/bobloxmonke
float map_slider_constrain(float n, float start1, float stop1, float start2, float stop2) {
	float value = (n - start1) / (stop1 - start1) * (stop2 - start2) + start2;
	return std::clamp(value, start2, stop2);
};

void gui::slider(std::int32_t x, std::int32_t y, std::int32_t slider_pos_x, std::int32_t slider_len, unsigned long font, const std::string string, float& value, float min_value, float max_value) {
	GetCursorPos(&cursor);
	const int slider_y = y + 2;
	const int slider_width = 8;
	
	// Get value from cursor and assign it
	if ((cursor.x > slider_pos_x) && (cursor.x < slider_pos_x + slider_len) && (cursor.y > slider_y) && (cursor.y < slider_y + slider_width) && (GetAsyncKeyState(VK_LBUTTON)))
		value = map_slider_constrain((cursor.x - slider_pos_x), 0.0f, float(slider_len), float(min_value), float(max_value));

	// Slider background and value display
	const float reverse_map = map_slider_constrain(value, float(min_value), float(max_value), 0.0f, float(slider_len));
	render::draw_filled_rect(slider_pos_x, slider_y, slider_len, slider_width, color(36, 36, 36, 255));
	render::draw_filled_rect(slider_pos_x, slider_y, reverse_map, slider_width, color(150, 22, 22, 255));

	// Slider label
	render::draw_text_string(x + 2, y - 1, font, (std::stringstream{ } << string << ": " <<  std::setprecision(3) << value).str(), false, color::white());
}

void gui::menu_movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h) {
	GetCursorPos(&cursor);
	
	if (GetAsyncKeyState(VK_LBUTTON) < 0 && (cursor.x > x && cursor.x < x + w && cursor.y > y && cursor.y < y + h)) {
		should_drag = true;

		if (!should_move) {
			cursor_corrected.x = cursor.x - x;
			cursor_corrected.y = cursor.y - y;
			should_move = true;
		}
	}
	
	if (should_drag) {
		x = cursor.x - cursor_corrected.x;
		y = cursor.y - cursor_corrected.y;
	}
	
	if (GetAsyncKeyState(VK_LBUTTON) == 0) {
		should_drag = false;
		should_move = false;
	}
}

void spectator_framework::spec_list_movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h) {
	GetCursorPos(&cursor);

	if (GetAsyncKeyState(VK_LBUTTON) < 0 && (cursor.x > x && cursor.x < x + w && cursor.y > y && cursor.y < y + h)) {
		should_drag = true;

		if (!should_move) {
			cursor_corrected.x = cursor.x - x;
			cursor_corrected.y = cursor.y - y;
			should_move = true;
		}
	}

	if (should_drag) {
		x = cursor.x - cursor_corrected.x;
		y = cursor.y - cursor_corrected.y;
	}

	if (GetAsyncKeyState(VK_LBUTTON) == 0) {
		should_drag = false;
		should_move = false;
	}
}