/**
 * @file    core/app.hpp
 * @brief   Main application class responsible for lifecycle management.
 *
 * The `App` class orchestrates the overall application flow, managing
 * the ECS registry and systems. It provides methods for initialization,
 * building, running, and cleaning up. The `App` itself does not directly
 * interact with entities or systems; instead, it relies on the `Context` 
 * class to manage them.
 */

#pragma once

#include <moss/core/context.hpp>
#include <moss/core/mirror.hpp>
#include <moss/ecs/ecs.hpp>


namespace moss {

class App {
public:
    /**
     * @brief Retrieves the singleton instance of the App.
     *
     * This method returns the single instance of the `App` class.
     * It ensures that only one instance exists throughout the application's lifetime.
     */
    App& instance();

    /**
     * @brief Initializes the app, setting up the logger and necessary components.
     *
     * This method prepares the app for execution by setting up logging
     * and any other required initialization tasks.
     */
    App& init();

    /**
     * @brief Builds the app by configuring all contexts and systems.
     *
     * This method sets up all the contexts and their associated systems.
     */
    App& build();

    /**
     * @brief Runs the app's main loop.
     *
     * This method starts the main loop of the app, repeatedly calling
     * `tick` on each context. It continues running until `quit` is set to true.
     */
    App& run();

    /**
     * @brief Exits the app and cleans up all contexts.
     *
     * This method exits the app, ensuring that all contexts and systems
     * are properly shut down.
     */
    App& exit();

    /**
     * @brief Mounts a new context into the app.
     *
     * This method adds a new context to the app. The context is initialized
     * and prepared for operation, and it will be part of the app's system.
     */
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
