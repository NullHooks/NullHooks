#include "dependencies/utilities/csgo.hpp"
#include "core/hooks/hooks.hpp"
#include "core/features/features.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"

void __stdcall hooks::frame_stage_notify::hook(client_frame_stage_t frame_stage) {
	switch (frame_stage) {
		case FRAME_UNDEFINED:                       break;
		case FRAME_START:                           break;
		case FRAME_NET_UPDATE_START:                break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
			skins::change_skins(frame_stage);
			skins::change_misc_models();					// Other models like localplayer, players and hands
			break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_END:   break;
		case FRAME_NET_UPDATE_END:
			backtrack::frame_stage_notify();
			break;
		case FRAME_RENDER_START:
			// Force update on thread safe manner to avoid crashes
			if (globals::forcing_update) {
				interfaces::clientstate->full_update();
				globals::forcing_update = false;
			}
			
			skins::change_misc_models();
			skins::change_skins(frame_stage);				// Run here too to avoid model flickering online
			visuals::nosmoke(frame_stage);					// Disables smoke, not the overlay. The overlay is disabled by hooking render_smoke_overlay
			animations::local::run_local_animations();		// Fix aa animations
			break;
		case FRAME_RENDER_END:                      break;
		default:                                    break;
	}

	original(interfaces::client, frame_stage);
}
