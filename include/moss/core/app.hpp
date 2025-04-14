/*
 * @file    core/app.hpp
 * @brief   Main application class.
 *
 * The `App` class orchestrates the application lifecycle, managing the ECS
 * registry and systems.  It provides methods for initialization, building,
 * running, cleaning up, and exiting the application.  The `App` class
 * itself does not directly modify the ECS registry or systems.  Instead,
 * it relies on the `Contex` class to perform these operations.
 */

#pragma once

#include <moss/core/contex.hpp>
#include <moss/ecs/ecs.hpp>


namespace moss {

class App {
public:
    void init();
    void build();
    void run();
    void clean();
    void exit();

    template<typename Ctx>
    void attach();

private:
    entt::registry m_registry;
};

}
