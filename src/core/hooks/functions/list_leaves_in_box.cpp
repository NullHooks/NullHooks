#include "core/hooks/hooks.hpp"

struct renderable_info_t {
	entity_t *renderable;
	std::byte pad[18];
	uint16_t flags;
	uint16_t flags2;
};

constexpr auto g_max_coord = 16384.0f;
constexpr auto g_min_coord = -g_max_coord;
const auto g_mins = vec3_t(g_min_coord, g_min_coord, g_min_coord);
const auto g_maxs = vec3_t(g_max_coord, g_max_coord, g_max_coord);

int __fastcall hooks::list_leaves_in_box::hook(void *ecx, void *edx, const vec3_t *mins, const vec3_t *maxs, uint16_t *list, int listmax) {
	static auto ret_addr = utilities::pattern_scan("client.dll", "56 52 FF 50 18") + 5;

	if (_ReturnAddress() == ret_addr) {
		auto frame_addr  = reinterpret_cast<uint8_t *>(_AddressOfReturnAddress());
		auto info        = *reinterpret_cast<renderable_info_t **>(frame_addr + 0x14);
		auto base_entity = reinterpret_cast<uint8_t *>(info->renderable) - 0x4;

		if (info != nullptr && base_entity != nullptr) {
			using fn_t  = entity_t *(__thiscall *)(void *);
			auto fn     = (*(fn_t **)base_entity)[7];
			auto entity = fn(base_entity);

			if(entity != nullptr && entity->is_player()) {
				info->flags  &= ~0x100;
				info->flags2 |=  0x040;

				return original(ecx, &g_mins, &g_maxs, list, listmax);
			}
		}
	}

	return original(ecx, mins, maxs, list, listmax);
}