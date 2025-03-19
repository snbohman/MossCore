/*
core/app.cpp - parentless

Implements core/app.hpp.

*/

#include <moss/meta/libs.hpp>
#include <moss/core/app.hpp>
#include <moss/core/ecs.hpp>
#include <type_traits>


using moss::App;

///////////////////
//// -- ECS -- ////
///////////////////
App& App::create(glm::u32 count) {
    for (glm::u32 i = 0; i < count; i++) {
        m_latest.push_back(m_registry.create());
    }

    return *this;
}

template<typename ...T>
App& App::attachComponent() {
    static_assert(
        std::is_base_of<T..., Component>::value,
        "[MOSS] All type inputs must be a base class of Component"
    );

    for (entt::entity entity : m_latest) {
        m_registry.emplace<T...>(entity);
    }

    return *this;
}

template<typename ...T>
App& App::attachSystem() {
    static_assert(
        std::is_base_of<T..., System>::value,
        "[MOSS] All type inputs must be a base class of System"
    );

    for (entt::entity entity : m_latest) {
        m_registry.emplace<T...>(entity).init();
    }

    return *this;
}



////////////////////////
//// -- Lifetime -- ////
////////////////////////
App& App::init() {
    static App instance;

    spdlog::set_pattern("[\e[1;32mMOSS\e[0m] [%^%L%$] %v");
    spdlog::set_level(spdlog::level::warn);

    return instance;
}

App& App::build() {
    for (entt::entity system : m_registry.view<System>()) {
        m_registry.get<System>(system).build();
    }

    return *this;
}

App& App::run() {
    while (m_quit) {
        for (entt::entity system : m_registry.view<System>()) {
            m_registry.get<System>(system).tick();
        }
    }

    return *this;
}

void App::exit() {
    for (entt::entity system : m_registry.view<System>()) {
        m_registry.get<System>(system).exit();
    }
}
