#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/hooks/hooks.hpp"

void run_backtrack();

void __stdcall hooks::frame_stage_notify::hook(client_frame_stage_t frame_stage) {
	switch (frame_stage) {
		case FRAME_UNDEFINED:                       break;
		case FRAME_START:                           break;
		case FRAME_NET_UPDATE_START:                break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START: break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_END:   break;
		case FRAME_NET_UPDATE_END:
			run_backtrack();
			break;
		case FRAME_RENDER_START:                    break;
		case FRAME_RENDER_END:                      break;
		default:                                    break;
	}

	original(interfaces::client, frame_stage);
}

// This is temporal
void run_backtrack() {
	static auto set_interpolation_flags = [](player_t* e, int flag) {
		const auto var_map = (uintptr_t)e + 36;
		const auto sz_var_map = *(int*)(var_map + 20);
		for (auto index = 0; index < sz_var_map; index++)
			*(uintptr_t*)((*(uintptr_t*)var_map) + index * 12) = flag;
	};

	if (!(true /*REPLACE VAR*/ && csgo::local_player && csgo::local_player->is_alive())) return;
	for (uint32_t i = 1; i <= interfaces::globals->max_clients; i++) {
		player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!player || player->team() == csgo::local_player->team() || player == csgo::local_player || player->dormant())
			continue;
		set_interpolation_flags(player, 0);
	}
}