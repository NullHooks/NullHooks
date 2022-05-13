#include "../hooks.hpp"

void __fastcall hooks::draw_model_execute::hook(void* _this, int edx, i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix) {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;
	if (interfaces::engine->is_taking_screenshot() && variables::clean_screenshots_bool) return;

	const auto mdl = info.model;
	if (!mdl) return;

	bool is_player = strstr(mdl->name, "models/player") != nullptr;

	if (!interfaces::studio_render->is_forced() && is_player) {
		visuals::chams::draw_chams(ctx, state, info, matrix);
		original(_this, edx, ctx, state, info, matrix);
		interfaces::model_render->override_material(nullptr);
	} else if (!interfaces::model_render->is_forced() && !is_player) {
		visuals::chams::draw_chams(ctx, state, info, matrix);
		original(_this, edx, ctx, state, info, matrix);
		interfaces::model_render->override_material(nullptr);
	} else original(_this, edx, ctx, state, info, matrix);
}