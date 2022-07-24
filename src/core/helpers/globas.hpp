#pragma once

namespace globals {
	// Set to true when pressing the full update button. If true, does full_update in fsn and reverts to false. full_update() is called in fsn so its more thread safe.
	inline bool forcing_update = false;

	// Used in fireevent hook
	inline bool round_ended = false;
}