#include "../hooks.hpp"

unsigned long __stdcall hooks::findmdl::hook(const char* path) noexcept {
	std::string new_path = path;		// We will change (or not) the path in draw_models()

	visuals::models::draw_models(new_path);

	return findmdl::original(interfaces::mdl_cache, new_path.c_str());
}
