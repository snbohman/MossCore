#pragma once

#include <moss/includes.hpp>
#include <moss/ecs/systems.hpp>
#include <moss/types.hpp>


namespace moss {

class Scene : public System {
public:
    Scene(const char* id, const ComponentRegistry& componentRegistry);
    ~Scene();

    void tick() override;

    const char* id;
    entt::registry registry;
};

} // moss
