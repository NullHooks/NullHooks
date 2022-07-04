#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/hooks/hooks.hpp"

void __stdcall hooks::frame_stage_notify::hook(client_frame_stage_t frame_stage) {
	switch (frame_stage) {
		case FRAME_UNDEFINED:                       break;
		case FRAME_START:                           break;
		case FRAME_NET_UPDATE_START:                break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START: break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_END:   break;
		case FRAME_NET_UPDATE_END:
			misc::backtrack();
			break;
		case FRAME_RENDER_START:
			visuals::misc::nosmoke(frame_stage);
			break;
		case FRAME_RENDER_END:                      break;
		default:                                    break;
	}

	original(interfaces::client, frame_stage);
}
