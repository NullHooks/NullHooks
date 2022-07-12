#include "dependencies/utilities/csgo.hpp"
#include "core/hooks/hooks.hpp"
#include "core/features/features.hpp"
#include "core/features/visuals/skin_changer.hpp"

void __stdcall hooks::frame_stage_notify::hook(client_frame_stage_t frame_stage) {
	switch (frame_stage) {
		case FRAME_UNDEFINED:                       break;
		case FRAME_START:                           break;
		case FRAME_NET_UPDATE_START:                break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
			skins::change_skins(frame_stage);
			break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_END:   break;
		case FRAME_NET_UPDATE_END:
			backtrack::frame_stage_notify();
			break;
		case FRAME_RENDER_START:
			visuals::misc::nosmoke(frame_stage);
			break;
		case FRAME_RENDER_END:                      break;
		default:                                    break;
	}

	original(interfaces::client, frame_stage);
}
