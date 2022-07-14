#include "dependencies/utilities/csgo.hpp"
#include "core/features/visuals/skin_changer.hpp"
#include "core/hooks/hooks.hpp"

void hooks::viewmodel_sequence::hook(c_recv_proxy_data *data, void *out_struct, void *arg3) {

        const auto view_model = reinterpret_cast<base_view_model_t *>(out_struct);

        if(interfaces::entity_list->get_client_entity_handle(view_model->owner()) == csgo::local_player)
                if(const auto weapon = reinterpret_cast<weapon_t *>(interfaces::entity_list->get_client_entity_handle(view_model->weapon())))
                        skins::fix_knife_animation(weapon, data->value.m_int);

	original(data, out_struct, arg3);

}