/**
 * @file    core/app.cpp
 * @brief   Implements the core engine lifecycle logic for moss::App.
 */

#include <moss/core/app.hpp>
#include <moss/core/key.hpp>
#include <moss/ecs/system.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>


namespace moss {

App& App::instance() {
    static App app;
    return app;
}

App& App::init() {
    spdlog::set_pattern("[MOSS] [%s] [%!] [%#]: %v");
    spdlog::set_level(spdlog::level::debug);

    return *this;
}

App& App::build() {
    Key<key::WRITE> key;
    key.m_registry = &m_registry;
    key.m_quit = &m_quit;

    for (auto& ctx : m_contexts) {
        ctx->build(key);
    }

    return *this;
}

App& App::run() {
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

App& App::exit() {
    Key<key::WRITE> key;
    key.m_registry = &m_registry;
    key.m_quit = &m_quit;

    for (auto& ctx : m_contexts) {
        ctx->exit(key);
    }

    return *this;
}

}
