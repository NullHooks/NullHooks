#pragma once
#include "dependencies/utilities/csgo.hpp"

#pragma region STRUCTS
struct multicombo_opt_t {
	std::string text;
	bool state;
};

class colorpicker_col_t {
public:
	color col;
	bool toggle;

	colorpicker_col_t(colorpicker_col_t& col_picker) {
		this->col = col_picker.col;
		this->toggle = col_picker.toggle;
	}

	colorpicker_col_t(const color col, const bool toggle = false) {
		this->col = col;
		this->toggle = toggle;
	}

	operator color() { return col; }
	operator bool() { return toggle; }
};

class combobox_toggle_t {
public:
	int idx;		// The selected idx of the vector
	bool toggle;	// For toggling combobox popup

	combobox_toggle_t(const int idx, const bool toggle = false) {
		this->idx = idx;
		this->toggle = toggle;
	}

	operator int() { return idx; }
	operator bool() { return toggle; }
};

class multicombobox_toggle_t {
public:
	std::vector<multicombo_opt_t> vector;		// The selected idx of the vector. We don't need an address here cuz we will pass the whole &multicombobox_toggle_t
	bool toggle;	// For toggling combobox popup

	multicombobox_toggle_t(std::vector<multicombo_opt_t> vector, const bool toggle = false) {
		this->vector = vector;
		this->toggle = toggle;
	}

	operator std::vector<multicombo_opt_t>() { return vector; }
	operator bool() { return toggle; }
};
#pragma endregion
