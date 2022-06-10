#include "../hooks.hpp"

unsigned long __stdcall hooks::findmdl::hook(char* path) noexcept {
	visuals::models::replace_model(path);		// We will change (or not) the path in draw_models()

	return findmdl::original(interfaces::mdl_cache, path);
}
