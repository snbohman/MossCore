/*
core/app.hpp - parentless

Core application.

Maybe move the attach stuff too
it's own logic, like contex plans.

*/

#pragma once


#include <moss/meta/libs.hpp>
#include <moss/core/ecs.hpp>

namespace moss {

class App {
public:
    inline App() { };
    inline ~App() { };

    inline App& quit() { m_quit = true; }
    inline App& loadRegistry(entt::registry* registry) { registry = &m_registry; }

    inline App& create(glm::u32 count = 1) {
        for (glm::u32 i = 0; i < count; i++) {
            m_latest.push_back(m_registry.create());
        }

        return *this;
    }

    template<typename... T>
    App& attachComponent() {
        static_assert(
            std::is_base_of<T..., Component>::value,
            "[MOSS] All type inputs must be a base class of Component"
        );

        for (entt::entity entity : m_latest) {
            m_registry.emplace<T...>(entity);
        }

        return *this;
    }

    template<typename... T>
    App& attachSystem() {
        static_assert(
            std::is_base_of<T..., System>::value,
            "All type inputs must be a base class of moss::System"
        );

        for (entt::entity entity : m_latest) {
            m_registry.emplace<T...>(entity).init();
        }

        return *this;
    }

    inline static App& init() {
        static App instance;

        spdlog::set_pattern("[\e[1;32mMOSS\e[0m] [%^%L%$] %v");
        spdlog::set_level(spdlog::level::warn);

        return instance;
    }

    inline App& build() {
        for (entt::entity system : m_registry.view<System>()) {
            m_registry.get<System>(system).build();
        }

        return *this;
    }

    inline App& run() {
        while (!m_quit) {
            for (entt::entity system : m_registry.view<System>()) {
                m_registry.get<System>(system).tick();
            }
        }

        return *this;
    }

    inline void exit() {
        for (entt::entity system : m_registry.view<System>()) {
            m_registry.get<System>(system).exit();
        }
    }

private:
    entt::registry m_registry;
    DynamicView m_latest;

    bool m_quit = false;
};

} // moss
