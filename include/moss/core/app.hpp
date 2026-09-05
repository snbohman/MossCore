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

#include "moss/core/key.hpp"
#include <moss/meta/logs.hpp>
#include <moss/core/context.hpp>
#include <moss/core/mirror.hpp>


namespace moss {

class App {
public:
    App& instance() {
        static App app;
        return app;
    }

    App& init() {
        spdlog::set_pattern("[MOSS] [%s] [%!] [%#]: %v");
        spdlog::set_level(spdlog::level::debug);

        return *this;
    }

    App& build() {
        Key<key::WRITE> key;
        key.m_registry = &m_registry;
        key.m_quit = &m_quit;

        for (auto& ctx : m_contexts) {
            ctx->build(key);
        }

        return *this;
    }

    App& run() {
        Key<key::READ> key;
        key.m_registry = &m_registry;
        key.m_quit = &m_quit;

        while (!m_quit) {
            for (auto& ctx : m_contexts) {
                ctx->tick(key);
            }
        }

        return *this;
    }

    App& exit() {
        Key<key::WRITE> key;
        key.m_registry = &m_registry;
        key.m_quit = &m_quit;

        for (auto& ctx : m_contexts) {
            ctx->exit(key);
        }

        return *this;
    }

    template<typename Ctx>
    requires(std::is_base_of_v<Context, Ctx>)
    App& mount() {
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
