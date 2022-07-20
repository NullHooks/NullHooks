#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void animations::local::run_local_animations()
{
	// are we alive?
	if (!csgo::local_player) return;

	// are we in thirdperson ?
	if (interfaces::input->camera_in_third_person)
	{
		// if we are save the angles
		auto angles = csgo::local_player->eye_angles();
		angles.z = 0.f;

		// and set them
		interfaces::prediction->set_local_view_angles(angles);

		// then update to apply
		csgo::local_player->update_client_side_animations();
	}
}