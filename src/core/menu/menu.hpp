#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "framework.hpp"
#include "variables.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/dx9/imgui_impl_dx9.h"

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

extern IDirect3DStateBlock9* state_block;

class Menu {
public:
	void render();
	//void check_toggle();
	bool menu_opened = false;

	void __stdcall create_objects(IDirect3DDevice9* device)
	{
		if (hooks::hCSGOWindow)
			ImGui_ImplDX9_CreateDeviceObjects();
	}

	void __stdcall invalidate_objects()
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
	}

	void __stdcall setup_resent(IDirect3DDevice9* device)
	{
		ImGui_ImplDX9_Init(hooks::hCSGOWindow, device);

		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.f;
		style.WindowRounding = 5;
		style.FramePadding = ImVec2(4, 3);
		style.WindowPadding = ImVec2(0, 0);
		style.ItemInnerSpacing = ImVec2(4, 4);
		style.ItemSpacing = ImVec2(17, 7);
		style.FrameRounding = 12;
		style.ScrollbarSize = 2.f;
		style.ScrollbarRounding = 12.f;

		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(222 / 255.f, 222 / 255.f, 222 / 255.f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(18 / 255.f, 18 / 255.f, 18 / 255.f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(50 / 255.f, 53 / 255.f, 58 / 255.f, 1);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(110 / 255.f, 122 / 255.f, 200 / 255.f, 1.f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(110 / 255.f, 122 / 255.f, 200 / 255.f, 1.f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(110 / 255.f, 122 / 255.f, 200 / 255.f, 1.f);
		colors[ImGuiCol_CheckMark] = ImVec4(214 / 255.f, 252 / 255.f, 255 / 255.f, 1);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(50 / 255.f, 53 / 255.f, 58 / 255.f, 1);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 0);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_TabActive] = ImVec4(18 / 255.f, 18 / 255.f, 18 / 255.f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);


		create_objects(device);
	}

	void apply_fonts() {
		ImGui::CreateContext();

		ImFontConfig font_config;
		font_config.OversampleH = 1; //or 2 is the same
		font_config.OversampleV = 1;
		font_config.PixelSnapH = 1;

		static const ImWchar ranges[] =
		{
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0x0400, 0x044F, // Cyrillic
			0,
		};

		//put your fonts down :)
	}

	void __stdcall end_present(IDirect3DDevice9* device)
	{
		if (menu_opened)
		{
			if (ImGui::GetStyle().Alpha > 1.f)
				ImGui::GetStyle().Alpha = 1.f;
			else if (ImGui::GetStyle().Alpha != 1.f)
				ImGui::GetStyle().Alpha += 0.03f;
		}

		ImGui::Render();

		state_block->Apply();
		state_block->Release();
	}

	void __stdcall pre_render(IDirect3DDevice9* device)
	{
		D3DVIEWPORT9 d3d_viewport;
		device->GetViewport(&d3d_viewport);

		device->CreateStateBlock(D3DSBT_ALL, &state_block);
		state_block->Capture();

		device->SetVertexShader(nullptr);
		device->SetPixelShader(nullptr);
		device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

		device->SetRenderState(D3DRS_LIGHTING, FALSE);
		device->SetRenderState(D3DRS_FOGENABLE, FALSE);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		device->SetRenderState(D3DRS_STENCILENABLE, FALSE);

		device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
		device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

		device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
		device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	}

	void __stdcall post_render()
	{
		ImGui_ImplDX9_NewFrame();
	}
};

extern Menu menu;

namespace watermark {
	void draw();
	void draw_stats();
	void draw_stats_string(std::string ts, color tscolor, std::string fps, color fpscolor, std::string speed, color speedcolor, bool draw_speed);

	namespace helpers {
		int get_fps() noexcept;
		std::string get_timestamp_string() noexcept;
	};
};