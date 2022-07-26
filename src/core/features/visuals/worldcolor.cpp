#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void visuals::worldcolor() {
	if (!variables::misc_visuals::worldcolor) return;
    if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;

    for (material_handle_t handle = interfaces::material_system->first_material(); handle != interfaces::material_system->invalid_material_handle(); handle = interfaces::material_system->next_material(handle)) {
        i_material* mat = interfaces::material_system->get_material(handle);
        if (!mat || !mat->is_precached()) continue;     // @todo: crash

        const std::string_view texture_group = mat->get_texture_group_name();

        if (texture_group.starts_with("World") || texture_group.starts_with("StaticProp"))
            mat->color_modulate(variables::colors::worldcolor_c.col.r / 255.f, variables::colors::worldcolor_c.col.g / 255.f, variables::colors::worldcolor_c.col.b / 255.f);
    }
}