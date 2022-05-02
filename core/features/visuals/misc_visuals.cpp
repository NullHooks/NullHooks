#include "../features.hpp"

void visuals::misc::nade_predict() noexcept {
    if (!variables::nade_predict_bool) return;

    static auto nadeVar{ interfaces::console->get_convar("cl_grenadepreview") };
    
    nadeVar->callbacks.size = 0;
    nadeVar->set_value(variables::nade_predict_bool);
}