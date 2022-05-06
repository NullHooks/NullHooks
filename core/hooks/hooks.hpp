#pragma once

namespace hooks {
	bool initialize();
	void release();

	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }
	
	using AllocKeyValuesMemoryFn = void* (__thiscall*)(void*, const std::int32_t);
	inline AllocKeyValuesMemoryFn AllocKeyValuesMemoryOriginal;
	void* __stdcall AllocKeyValuesMemory(const std::int32_t size);

	namespace create_move {
		using fn = bool(__stdcall*)(float, c_usercmd*);
		bool __stdcall hook(float input_sample_frametime, c_usercmd* cmd);
	};

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	};

	using DoPostScreenSpaceEffectsFn = void(__thiscall*)(void*, const void*) noexcept;
	inline DoPostScreenSpaceEffectsFn DoPostScreenSpaceEffectsOriginal = nullptr;
	void __stdcall DoPostScreenSpaceEffects(const void* viewSetup) noexcept;

	/*
	using get_viewmodel_fov_fn = void(__thiscall*)(void*, uintptr_t, uintptr_t) noexcept;
	inline get_viewmodel_fov_fn get_viewmodel_fov_original = nullptr;
	float __fastcall get_viewmodel_fov(uintptr_t, uintptr_t);
	*/

	using override_view_fn = void(__thiscall*)(void*, view_setup_t* setup);
	inline override_view_fn override_view_original;
	void __fastcall override_view(uintptr_t, uintptr_t, view_setup_t* setup);
}