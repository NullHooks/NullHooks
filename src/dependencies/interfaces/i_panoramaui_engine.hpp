#pragma once
#include <cstdint>

// credits - para
class c_ui_panel {
public:
	const char* get_panel_id() {
		using original_fn = const char*(__thiscall*)(c_ui_panel*);
		return (*(original_fn**)this)[9](this);
	}

	c_ui_panel* get_parent() {
		using original_fn = c_ui_panel*(__thiscall*)(c_ui_panel*);
		return (*(original_fn**)this)[25](this);
	}

	void set_visible(bool visible) {
		using original_fn = void(__thiscall*)(c_ui_panel*, bool);
		return (*(original_fn**)this)[27](this, visible);
	}

	c_ui_panel* find_child_traverse(const char* name) {
		using original_fn = c_ui_panel*(__thiscall*)(c_ui_panel*, const char*);
		return (*(original_fn**)this)[40](this, name);
	}
};

class c_ui_engine {
public:
	bool is_valid_panel_pointer(c_ui_panel const* panel) {
		using original_fn = bool(__thiscall*)(c_ui_engine*, c_ui_panel const*);
		return (*(original_fn**)this)[36](this, panel);
	}

	c_ui_panel* get_last_dispatched_event_target_panel() {
		using original_fn = c_ui_panel*(__thiscall*)(c_ui_engine*);
		return (*(original_fn**)this)[56](this);
	}

	void run_script(c_ui_panel* panel, const char* java_script, const char* schema_path, int a5 = 8, int a6 = 10, bool a7 = false, bool a8 = false) {
		using original_fn = void(__thiscall*)(c_ui_engine*, c_ui_panel*, const char*, const char*, int, int, bool, bool);
		return (*(original_fn**)this)[113](this, panel, java_script, schema_path, a5, a6, a7, a8);
	}
};

class i_panoramaui_engine {
public:
	c_ui_engine* access_ui_engine() {
		//return ((c_ui_engine * (__thiscall*)(i_panoramaui_engine*))((*(uintptr_t**)(this))[11]))(this);

		using original_fn = c_ui_engine *(__thiscall*)(i_panoramaui_engine*);
		return (*(original_fn**)this)[11](this);
	}
};