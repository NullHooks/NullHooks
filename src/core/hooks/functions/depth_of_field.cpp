#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/hooks/hooks.hpp"

bool __stdcall hooks::is_depth_of_field_enabled::hook() {
    visuals::motion_blur(nullptr);
    return false;
}