#pragma once

#include <moss/includes.hpp>
#include <moss/ecs/systems.hpp>

namespace moss {

class Render : public System {
public:
    virtual void tick(entt::registry& registry) override;
};

} // moss
