#include "../hooks.hpp"

float __fastcall hooks::get_viewmodel_fov::hook(uintptr_t, uintptr_t) {
	if (interfaces::engine->is_taking_screenshot())
		return original(interfaces::clientmode);		// Return original viewmodel fov if in screenshot (68.f)

	return original(interfaces::clientmode) * variables::custom_vmfov_slider;
}