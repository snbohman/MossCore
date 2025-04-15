/*
 * @file    command/view.hpp
 * @brief   Defines entity views for ECS operations, both runtime and compile-time.
 *
 * An entity view is a container holding a selection of entities based on
 * specified criteria.  This file defines two types of views:
 *
 * 1.  **DynamicView:** A runtime view implemented as a heap-allocated
 *     `std::vector<entt::entity>`.  It offers flexibility without compile-time
 *     safety.  Defined in `primitives.hpp` to avoid circular dependencies
 *     and because it's a simple output type.
 *
 * 2.  **View:** A compile-time view that leverages templates to enforce
 *     inclusion and exclusion criteria at compile time. While the ECS registry
 *     access is inherently runtime, the view's structure and filtering logic
 *     are determined at compile time, providing increased safety and
 *     performance through static analysis.
*/


#pragma once

#include <moss/core/key.hpp>
#include <moss/commands/primitives.hpp>

namespace moss::commands {

template<typename... Inc, typename... Ex>
class View<Include<Inc...>, Exclude<Ex...>> {
public:
    View() { }
    ~View() { }

    static_assert(sizeof...(Inc), "Include<> is required to have at least one component specified");

    void apply(const Key<key::WRITE>& key) { m_registry = key.m_registry; } 
    void clean() { m_registry = nullptr; }

    [[nodiscard]] auto view(bool doClean = true) {
        static_assert(m_registry == nullptr, "Apply function must be called before aquiring view");

        if constexpr (sizeof...(Ex) == 0) {
            auto v = m_registry->view<Inc...>();
            if (doClean) clean();
            return std::move(v);
        } else {
            auto v = m_registry->view<Inc...>(entt::exclude<Ex...>);
            if (doClean) clean();
            return std::move(v);
        }
    }

private:
    entt::registry* m_registry = nullptr;
};

}
