#include "dependencies/utilities/csgo.hpp"
#include "core/hooks/hooks.hpp"

constexpr auto VENDORID_NVIDIA = 0x10DEu;
constexpr auto VENDORID_ATI    = 0x1002u;
constexpr auto VENDORID_INTEL  = 0x8086u;

bool hooks::supports_resolve_depth::hook() {

	i_matsys_material_adapter_info_t info;
	auto adapter = interfaces::material_system->get_current_adapter();
	interfaces::material_system->get_display_adapter_info(adapter, &info);

	if(info.vendor_id == VENDORID_ATI)
		return false;

	return original();

}
