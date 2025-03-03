/*
core/render.hpp - App

Renders transforms (TODO) from scene registry.
*/


#pragma once

#include <moss/includes.hpp>
#include <moss/ecs/systems.hpp>


namespace moss {

class Render : public System {
public:
    void tick(entt::registry& registry) override;
};

} // moss
