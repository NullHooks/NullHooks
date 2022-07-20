#pragma once

// https://www.unknowncheats.me/forum/counterstrike-global-offensive/214760-hooking-findmdl.html
// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/datacache/imdlcache.h

class mdlcache
{
public:
	// Patoke - too lazy to implement the whole class, XD!
	void begin_lock() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[33](this);
	}

	void end_lock() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[34](this);
	}
};