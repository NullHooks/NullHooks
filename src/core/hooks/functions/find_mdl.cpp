#include "../hooks.hpp"

unsigned long __stdcall hooks::findmdl::hook(char* path) noexcept {
	visuals::models::draw_models(path);		// We will change (or not) the path in draw_models()

	return findmdl::original(interfaces::mdl_cache, path);
}
