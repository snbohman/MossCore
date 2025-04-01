#pragma once

#include <moss/core/contex.hpp>
#include <moss/core/fluent.hpp>
#include <moss/ecs/ecs.hpp>


namespace moss {

class App {
public:
    void init();
    void build();
    void run();
    void clean();
    void exit();

    /*
    Attaching a registry from a contex or fluent.
    */
    void attach(Contex<contex::WRITE>& contex);
    void attach(Contex<contex::READ>& contex);
    void attach(Fluent<fluent::WRITE>& fluent);

private:
    entt::registry m_registry;
    std::vector<System> m_systems;
    bool m_quit = false;
};

}
