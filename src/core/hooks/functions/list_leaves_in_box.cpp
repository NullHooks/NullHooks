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
	using fn_t = entity_t *(__thiscall *)(void *);

	if (_ReturnAddress() == ret_addr) {
		auto frame_addr  = reinterpret_cast<uint8_t *>(_AddressOfReturnAddress());
		auto info        = *reinterpret_cast<renderable_info_t **>(frame_addr + 0x14);
		auto unknown     = (**(fn_t **)info->renderable)(info->renderable);

		if (info != nullptr && unknown != nullptr) {
			auto entity = (*(fn_t **)unknown)[7](unknown);

			if(entity != nullptr && entity->is_player()) {
				info->flags  &= ~0x100;
				info->flags2 |=  0x040;

				return original(ecx, &g_mins, &g_maxs, list, listmax);
			}
		}
	}

	return original(ecx, mins, maxs, list, listmax);
}
