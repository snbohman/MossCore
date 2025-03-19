/*
core/app.cpp - parentless

Implements core/app.hpp.

*/

#include <moss/meta/libs.hpp>
#include <moss/core/app.hpp>
#include <moss/core/ecs.hpp>


using moss::App;

///////////////////
//// -- ECS -- ////
///////////////////
App& App::create(glm::u32 count) {
    for (int i = 0; i < count; i++) {
        m_latest.push_back(m_registry.create());
    } return *this;
}

template<typename T>
App& App::attachComponent(std::initializer_list<entt::entity> entities) {
    for (entt::entity entity : entities.size() == 0 ? m_latest : entities) {
        m_registry.emplace<T>(entity);
    } return *this;
}

App& App::attachSystem(const System& func, std::initializer_list<entt::entity> entities) {
    for (entt::entity entity : entities.size() == 0 ? m_latest : entities) {
        m_registry.emplace<System>(entity).init();
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
            m_registry.get<System>(system).build();
        }
    }

    return *this;
}

void App::exit() {
    for (entt::entity system : m_registry.view<System>()) {
        m_registry.get<System>(system).build();
    }
}
