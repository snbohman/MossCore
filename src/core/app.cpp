/**
 * @file    core/app.cpp
 * @brief   Implements the core engine lifecycle logic for moss::App.
 */

#include <moss/core/app.hpp>
#include <moss/core/key.hpp>
#include <moss/ecs/system.hpp>
#include <moss/logger/logger.hpp>


using namespace moss;

App& App::instance() {
    static App app;
    return app;
}

App& App::init() {
    auto sink = std::make_shared<logger::MainSink<std::mutex>>();
    auto logger = std::make_shared<spdlog::logger>("MainLogger", sink);
    spdlog::set_default_logger(logger);

    return *this;
}

App& App::build() {
    Key<key::WRITE> key;
    key.m_registry = &m_registry;

    for (auto& ctx : m_contexts) {
        ctx->build(key);
    }

    return *this;
}

App& App::run() {
    Key<key::READ> key;
    key.m_registry = &m_registry;

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

    for (auto& ctx : m_contexts) {
        ctx->exit(key);
    }

    return *this;
}
