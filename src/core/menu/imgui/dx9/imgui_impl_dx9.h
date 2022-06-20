// dear imgui: Renderer Backend for DirectX9
// This needs to be used along with a Platform Backend (e.g. Win32)

// Implemented features:
//  [X] Renderer: User texture binding. Use 'LPDIRECT3DTEXTURE9' as ImTextureID. Read the FAQ about ImTextureID!
//  [X] Renderer: Support for large meshes (64k+ vertices) with 16-bit indices.

// You can use unmodified imgui_impl_* files in your project. See examples/ folder for examples of using this. 
// Prefer including the entire imgui/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#pragma once
#include "../imgui.h"

struct IDirect3DDevice9;

IMGUI_API bool        ImGui_ImplDX9_Init(void* hwnd, IDirect3DDevice9* device);
IMGUI_API void        ImGui_ImplDX9_Shutdown();
IMGUI_API void        ImGui_ImplDX9_NewFrame();

// Use if you want to reset your rendering device without losing Dear ImGui state.
IMGUI_API void        ImGui_ImplDX9_InvalidateDeviceObjects();
IMGUI_API bool        ImGui_ImplDX9_CreateDeviceObjects();
