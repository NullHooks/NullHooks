#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

inline void apply_worldcolor(color col) {
    for (material_handle_t handle = interfaces::material_system->first_material(); handle != interfaces::material_system->invalid_material_handle(); handle = interfaces::material_system->next_material(handle)) {
        i_material* mat = interfaces::material_system->get_material(handle);
        if (!mat || !mat->is_precached()) continue;

        const std::string_view texture_group = mat->get_texture_group_name();

        if (texture_group.starts_with("World") || texture_group.starts_with("StaticProp"))
            mat->color_modulate(col.r / 255.f, col.g / 255.f, col.b / 255.f);
    }
}

void visuals::worldcolor() {
    static bool using_worldcolor = true;        // If true it means we just used worldcolor, so if we disable the menu var, it will apply default worldcolor to reset.
                                                // Default is true in case we load a the cheat in a match with worldcolor enabled

    if (!variables::misc_visuals::worldcolor) {
        if (using_worldcolor) {
            apply_worldcolor(color::white());
            using_worldcolor = false;
        }
        return;
    }
    if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;

    using_worldcolor = true;
    apply_worldcolor(variables::colors::worldcolor_c.col);
}