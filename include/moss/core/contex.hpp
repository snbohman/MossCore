#pragma once

#include <moss/ecs/system.hpp>
#include <moss/commands/primitives.hpp>


namespace moss {

class Mirror;

class Contex {
public:
    virtual ~Contex() = default; 

    virtual void init(Mirror& mirror) { }
    void build();
    void tick();
    void exit();

private:
    friend class Mirror;

    DynamicView m_view;
    std::vector<std::unique_ptr<System>> m_systems;
};

}
