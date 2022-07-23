#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

// Used in frame_stage_notify - FRAME_RENDER_START
void animations::local::run_local_animations() {
	if (!csgo::local_player || !csgo::local_player->is_alive()) return;
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected()) return;
	if (!interfaces::input->camera_in_third_person) return;		// For now we only care about local animations when on thirdperson

	// Save the angles
	auto angles = csgo::local_player->eye_angles();
	angles.z = 0.f;

	// Set them and update to apply
	interfaces::prediction->set_local_view_angles(angles);

	/*
	 * @todo: Fix crash when running full_update()

	if (globals::forcing_update)
		csgo::local_player->update_client_side_animations();
	*/
}