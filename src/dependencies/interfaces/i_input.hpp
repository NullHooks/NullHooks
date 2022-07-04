#pragma once
#include "source-sdk/classes/c_usercmd.hpp"

class i_input {
public:
	std::byte			pad0[0xC];				// 0x00
	bool				bTrackIRAvailable;		// 0x0C
	bool				bMouseInitialized;		// 0x0D
	bool				bMouseActive;			// 0x0E
	std::byte			pad1[0x9A];				// 0x0F
	bool				camera_in_third_person;	// 0xA9
	std::byte			pad2[0x2];				// 0xAA
	vec3_t				camera_offset;			// 0xAC
	std::byte			pad3[0x38];				// 0xB8
	c_usercmd*          pCommands;				// 0xF0


	c_usercmd *get_user_cmd( int slot, int sequence_num ) {
		using fn = c_usercmd* (__thiscall*)(void *, int, int);
		return (*( fn**)this)[8](this, slot, sequence_num);
	}
};
