#pragma once

#include <moss/meta/libs.hpp>
#include <moss/core/contex.hpp>
#include <moss/ecs/component.hpp>
#include <moss/ecs/system.hpp>
#include <moss/commands/primitives.hpp>


namespace moss {

class Mirror {
public:
    Mirror& quit() { return *this; }

    Mirror& create(glm::u32 count = 1) {
        m_view.clear();
        m_view.reserve(count);

        for (glm::u32 i = 0; i < count; i++) {
            entt::entity e = m_registry->create();
            m_view.push_back(e); m_contex.m_view.push_back(e);
        }

        return *this;
    }

    template<typename... T>
    Mirror& attach() {
        static_assert(
            std::is_base_of<Component, T...>::value,
            "Expected all of T to inherit moss::Component"
        );

        for (entt::entity entity : m_view)
            m_registry->emplace<T...>(entity).init();

        return *this;
    }

    template<typename... T>
    Mirror& connect() {
        static_assert(
            std::is_base_of<System, T...>::value,
            "Expected all of T to inherit moss::System"
        );

        m_contex.m_view.push_back(T()...);
        return *this;
    }


private:
    friend class App;
    Mirror() { }

    Contex m_contex;
    DynamicView m_view;
    entt::registry* m_registry;
};

}
