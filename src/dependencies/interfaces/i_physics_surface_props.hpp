#pragma once
#include <cstddef>

struct surface_data {
    std::byte pad[80];
    float maxspeedfactor;
    float jumpfactor;
    float penetrationmodifier;
    float damagemodifier;
    short material;
    bool climbable;
};

class physics_surface_props {
public:
    auto get_surface_data(int index) {
        using original_fn = surface_data*(__thiscall*)(physics_surface_props*, int);
        return (*(original_fn**)this)[5](this, index);
    }
};