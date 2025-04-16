/**
 * @file    core/app.hpp
 * @brief   Main application class responsible for lifecycle management.
 *
 * The `App` class orchestrates the overall application flow, managing
 * the ECS registry and systems. It provides methods for initialization,
 * building, running, and cleaning up. The `App` itself does not directly
 * interact with entities or systems; instead, it relies on the `Context` 
 * class to manage them, with the `mount` method to add them.
 */

#pragma once

#include <moss/meta/defines.hpp>
#include <moss/core/context.hpp>
#include <moss/core/mirror.hpp>
#include <moss/ecs/ecs.hpp>


namespace moss {

class App {
public:
    static App& instance();
    App& init();
    App& build();
    App& run();
    App& exit();

    template<typename Ctx>
    App& mount() {
        static_assert(
            std::is_base_of_v<Context, Ctx>,
            "Ctx must inherit moss::Contex"
        );

        Mirror mirror;
        mirror.m_registry = &m_registry;

        auto ctx = std::make_unique<Ctx>();
        mirror.m_contex = ctx.get();
        ctx->init(mirror);

        m_contexts.push_back(std::move(ctx));
        return *this;
    }

private:
    std::vector<std::unique_ptr<Context>> m_contexts;
    entt::registry m_registry;
    bool m_quit;
};

}
