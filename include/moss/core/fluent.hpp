#pragma once

#include <moss/meta/libs.hpp>
#include <moss/commands/write.hpp>

namespace moss {

namespace fluent {

enum Permissions {
    READ = 1 << 0,
    WRITE = 1 << 1,
};

}

template<fluent::Permissions P>
class Fluent {
public:
    Fluent(const Fluent&) = delete;
    Fluent& operator=(const Fluent&) = delete;
    Fluent(Fluent&&) = delete;
    Fluent& operator=(Fluent&&) = delete;

    static Fluent& init() { static Fluent<P> fluent; return fluent; }

    Fluent& create(int count = 1) {
        for (int i = 0; i < count; i++) {
            m_view.push_back(m_registry->create());
        }
    }

    template<typename... T>
    Fluent& attach() {
        for (entt::entity entity : m_view) {
            m_registry->emplace<T...>(entity);
        }

        return *this;
    }

private:
    entt::registry* m_registry;
    DynamicView m_view;
};

}
