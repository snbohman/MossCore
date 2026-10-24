/**
 * @file    core/app.hpp
 * @brief   App: owns the registry and the contexts, and runs the lifecycle.
 *
 *     App app;
 *     app.init()
 *        .mount<MyContext>()
 *        .build()
 *        .run()
 *        .exit();
 *
 * init     logging setup
 * mount    creates the Context and runs its init(Mirror&)
 * build    build phase of every context, in mount order (WRITE key)
 * run      tick every context in mount order until something calls
 *          cmd::Quit (the frame in progress always finishes)
 * exit     exit phase, contexts and their systems in REVERSE order (WRITE key)
 *
 * The App does not touch entities or systems itself; contexts do.
 */

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/logs.hpp>
#include <moss/core/key.hpp>
#include <moss/core/context.hpp>
#include <moss/core/mirror.hpp>

namespace moss {

class App {
public:
    App() = default;
    App(const App&) = delete;
    App& operator=(const App&) = delete;

    App& init() {
        spdlog::set_pattern("[MOSS] [%s] [%!] [%#]: %v");
        spdlog::set_level(spdlog::level::debug);

        return *this;
    }

    template<typename Ctx>
    requires(std::derived_from<Ctx, Context>)
    App& mount() {
        auto context = std::make_unique<Ctx>();

        Mirror mirror(&m_registry, context.get());
        context->init(mirror);

        m_contexts.push_back(std::move(context));
        return *this;
    }

    App& build() {
        Key<key::WRITE> key(&m_registry, &m_quit);

        for (auto& context : m_contexts)
            context->build(key);

        return *this;
    }

    App& run() {
        Key<key::READ> key(&m_registry, &m_quit);

        while (!m_quit) {
            for (auto& context : m_contexts)
                context->tick(key);
        }

        return *this;
    }

    App& exit() {
        Key<key::WRITE> key(&m_registry, &m_quit);

        for (auto it = m_contexts.rbegin(); it != m_contexts.rend(); ++it)
            (*it)->exit(key);

        return *this;
    }

private:
    // Declared first, destroyed last: contexts (and their systems) die before the registry
    entt::registry m_registry;
    std::vector<std::unique_ptr<Context>> m_contexts;
    bool m_quit = false;
};

}
