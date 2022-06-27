#include "core/hooks/hooks.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void __fastcall hooks::draw_model_execute::hook(void* _this, int edx, i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix) {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;
	if (interfaces::engine->is_taking_screenshot() && variables::misc::clean_screenshots) {
		original(_this, edx, ctx, state, info, matrix);		// Need original so we see the normal players lol
		return;
	}

	const auto mdl = info.model;
	if (!mdl) return;

	if (interfaces::studio_render->is_forced())
		return original(_this, edx, ctx, state, info, matrix);

	visuals::chams::draw_chams(ctx, state, info, matrix);
	original(_this, edx, ctx, state, info, matrix);
	interfaces::model_render->override_material(nullptr);	// Reset to default materials
}