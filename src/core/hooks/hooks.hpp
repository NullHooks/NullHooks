#pragma once
#include <cstdint>
#include "source-sdk/sdk.hpp"
#include "core/helpers/helpers.hpp"
#include "core/menu/framework.hpp"

namespace hooks {
	bool initialize();
	void release();

	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }

	namespace WndProc_hook {
		inline HWND csgo_window;
		inline WNDPROC original;
		long __stdcall WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	}

	namespace alloc_key_values_memory {
		using fn = void* (__thiscall*)(void*, const std::int32_t);
		inline fn original;
		void* __stdcall hook(const std::int32_t size);
	}

	namespace create_move {
		using fn = bool(__stdcall *)(float, c_usercmd *);
		inline fn original;
		bool hook(float input_sample_frametime, c_usercmd *cmd, bool &send_packet);
		void proxy();
	}

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		inline fn original;
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}

	namespace findmdl {
		using fn = unsigned long(__thiscall*)(void*, char*);
		inline fn original;
		unsigned long __stdcall hook(char* path) noexcept;
	}
	
	namespace do_post_screen_space_effects {
		using fn = void(__thiscall*)(void*, const void*);
		inline fn original = nullptr;
		void __stdcall hook(const void* viewSetup);
	}

	namespace get_viewmodel_fov {
		using fn = float(__thiscall*)(void*);
		inline fn original;
		float __fastcall hook(uintptr_t, uintptr_t);
	}

	namespace override_view {
		using fn = void(__thiscall*)(void*, view_setup_t* setup);
		inline fn original;
		void __fastcall hook(uintptr_t, uintptr_t, view_setup_t* setup);
	}

	namespace draw_model_execute {
		using fn = void(__fastcall*)(void*, int, i_mat_render_context*, const draw_model_state_t&, const model_render_info_t&, matrix_t*);
		inline fn original = nullptr;
		void __fastcall hook(void* _this, int edx, i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& pInfo, matrix_t* pCustomBoneToWorld);
	}

	namespace list_leaves_in_box {
		using fn = int(__thiscall*)(void*, const vec3_t* mins, const vec3_t* maxs, uint16_t* list, int listmax);
		inline fn original;
		int __fastcall hook(void* ecx, void* edx, const vec3_t* mins, const vec3_t* maxs, uint16_t* list, int listmax);
	}

	namespace frame_stage_notify {
		using fn = void(__thiscall*)(void*, client_frame_stage_t);
		inline fn original;
		void __stdcall hook(client_frame_stage_t frame_stage);
	}

	namespace render_smoke_overlay {
		using fn = void(__thiscall*)(void*, bool);
		inline fn original;
		void __fastcall hook(void* thisptr, void* edx, bool previewmodel);
	}

	namespace on_screen_size_changed {
		using fn = void(__thiscall*)(void*, int, int);
		inline fn original;
		void __fastcall hook(void* thisptr, void* edx, int old_width, int old_height);
	}

	namespace emit_sound {
		using fn = void(__thiscall*)(void*, int&, int, int, const char*, unsigned int, const char*, float, int, float, int, int, const vec3_t*, const vec3_t*, void*, bool, float, int, StartSoundParams_t&);
		inline fn original;
		void __fastcall hook(void* thisptr, void* edx, int& filter, int ent_index, int channel, const char* sound_entry, unsigned int sound_entry_hash, const char* sample, float volume, int seed, float attenuation, int flags, int pitch, const vec3_t* origin, const vec3_t* direction, void* utl_vec_origins, bool update_positions, float soundtime, int speakerentity, StartSoundParams_t& params);
	}

	namespace get_client_model_renderable {
		using fn = void*(__stdcall *)();
		inline fn original;
		void* __stdcall hook();
	}

	namespace is_depth_of_field_enabled {
		using fn = void* (__thiscall*)(void*);
		inline fn original;
		bool __stdcall hook();
	}

	namespace supports_resolve_depth {
		using fn = bool(*)();
		inline fn original;
		bool hook();
	}

	namespace fire_event {
		using fn = void(__fastcall*)(void*, void* edx, i_game_event* gameEvent, bool bServerOnly, bool bClientOnly);
		inline fn original;
		void __fastcall hook(void* thisptr, void* edx, i_game_event* gameEvent, bool bServerOnly, bool bClientOnly);
	}

	namespace viewmodel_sequence {
		using fn = void(*)(c_recv_proxy_data *, void *, void *);
		inline fn original;
		void hook(c_recv_proxy_data *, void *, void *);
	}

	#pragma region sv_pure
	namespace loose_files_allowed {
		using fn = void(__thiscall*)(void*);
		inline fn original;
		bool __stdcall hook();
	}

	namespace CL_CheckForPureServerWhitelist {
		using fn = void(__thiscall*)(void*);
		inline fn original;
		void __fastcall hook(void* edx, void* ecx);
	}
	#pragma endregion
}
