#pragma once
#include "source-sdk/classes/c_usercmd.hpp"

class i_input {
private:
	char u0[172];
	bool u1;
public:
	bool camera_in_third_person;
private:
	char u2[2];
public:
	vec3_t camera_offset;

	c_usercmd* get_user_cmd(int slot, int sequence_num) {
		using original_fn = c_usercmd*( __thiscall*)(i_input*, int, int);
		return (*(original_fn**)this)[8](this, slot, sequence_num);
	}
};
