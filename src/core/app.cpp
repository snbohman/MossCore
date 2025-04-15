/**
 * @file    core/app.cpp
 * @brief   Implements the core engine lifecycle logic for moss::App.
 */

#include <moss/core/app.hpp>
#include <moss/core/key.hpp>
#include <moss/ecs/system.hpp>
#include <moss/logger/logger.hpp>


using namespace moss;

/**
 * @brief Access the global singleton instance of the App.
 * 
 * Ensures there's only one App instance alive. Used to begin
 * the fluent lifecycle chain.
 */
App& App::instance() {
    static App app;
    return app;
}

/**
 * @brief Initializes engine internals (e.g., logger setup).
 * 
 * This should be called before mounting any contexts.
 */
App& App::init() {
    auto sink = std::make_shared<logger::MainSink<std::mutex>>();
    auto logger = std::make_shared<spdlog::logger>("MainLogger", sink);
    spdlog::set_default_logger(logger);

    return *this;
}

/**
 * @brief Builds all mounted contexts.
 * 
 * Each context's `build()` function is called. This is the stage
 * where systems are attached and internal data structures are set.
 */
App& App::build() {
    for (auto& ctx : m_contexts) {
        ctx->build();
    }

    return *this;
}

/**
 * @brief Enters the main loop, ticking all contexts until quit.
 * 
 * Continuously runs `tick()` on each mounted context.
 * Will only stop when `m_quit` is set to true (not exposed yet).
 */
App& App::run() {
    while (!m_quit) {
        for (auto& ctx : m_contexts) {
            ctx->tick();
        }
    }

    return *this;
}

/**
 * @brief Cleans up all mounted contexts.
 * 
 * Runs `exit()` on each context to gracefully shut them down.
 */
App& App::exit() {
    for (auto& ctx : m_contexts) {
        ctx->exit();
    }

    return *this;
}
