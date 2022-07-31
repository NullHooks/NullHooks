#pragma once

class c_hud_chat {
public:
	void chat_printf(int filter, const char* txt) {
		using original_fn = void(__cdecl*)(void*, int, const char*);
		(*(original_fn**)this)[26](this, filter, txt);
	}
};

class i_client_mode {
public:
	char u1[28];
	c_hud_chat* chat;	// 0x2A
};