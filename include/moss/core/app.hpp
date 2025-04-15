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
#include <moss/core/mirror.hpp>
#include <moss/ecs/ecs.hpp>


namespace moss {

class App {
public:
    App& instance();
    App& init();
    App& build();
    App& run();
    App& exit();

    template<typename Ctx>
    App& mount() {
        static_assert(
            std::is_base_of<Contex, Ctx>::value,
            "Expected all of T to inherit moss::Contex"
        );

        Mirror mirror;
        mirror.m_registry = &m_registry;

        Ctx c; c.init(mirror);
        m_contexts.push_back(Ctx());
    }

private:
    std::vector<Contex> m_contexts;
    entt::registry m_registry;
    bool m_quit;
};

}
