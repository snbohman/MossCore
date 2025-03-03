/*
core/scene.hpp - App

Handles the ecs of the scene. Has a unique
registry, only including entities of the scene.

*/


#pragma once

#include <moss/includes.hpp>
#include <moss/ecs/systems.hpp>
#include <moss/defines.hpp>


namespace moss {

class Scene : public System {
public:
    Scene(const char* id, const moss::types::ComponentRegistry& componentRegistry);

    void tick() override;

    const char* id;
    entt::registry registry;
};

} // moss
