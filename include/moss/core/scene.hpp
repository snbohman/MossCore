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

struct SceneInitCrate {
    const char* id;
    const moss::types::ComponentRegistry& componentRegistry;
};

class Scene : public System {
public:
    // -- Special init for scene system -- //
    Scene(moss::SceneInitCrate crate);
    void tick(moss::SystemTickCrate crate) override;

    const char* id;
    entt::registry registry;
};

} // moss
