/*
 * @file    commands/query.hpp
 * @brief   Defines query structures for retrieving components from entity views.
 *
 * Queries allow efficient retrieval of component data from a specified entity
 * view. The retrieved components are stored either in an `Atlas` or a `Pool`.
 *
 * -   `Atlas`:  A container suitable for storing components from multiple
 *     entities within the view.
 * -   `Pool`:   A container intended for storing components from a single
 *     entity. Using a `Pool` with a view containing multiple entities may
 *     lead to undefined behavior, and a warning is issued.
 *
 * Two types of queries are provided:
 *
 * 1.  **Query:** A compile-time query that operates on a statically defined
 *     `View`.  This offers compile-time safety and optimization opportunities.
 *
 * 2.  **DynamicQuery:** A runtime query that operates on a `DynamicView`.
 *     This provides flexibility but sacrifices compile-time safety.
*/

#include <moss/meta/defines.hpp>
#include <moss/core/contex.hpp>
#include <moss/commands/primitives.hpp>

namespace moss::commands {

template<typename... Wth, typename... VwInc, typename... VwEx>
struct Query<With<Wth...>, View< Include<VwInc...>, Exclude<VwEx...> >> {
    static_assert(sizeof...(Wth) > 0, "With<> is required to have at least one component");

    void apply() { Contex<contex::READ>::get().apply(m_registry); m_view.apply(); }
    void clean() { m_registry = nullptr; m_view.clean(); }

    [[nodiscard]] Atlas<Wth...> atlas(bool doClean = true) {
        auto eView = m_view.view(doClean);

        Atlas<Wth...> atlas;
        atlas.reserve(std::distance(eView.begin(), eView.end()));
        for (const auto& entity : eView) {
            atlas.push_back(std::move(m_registry->get<Wth...>(entity)));
        }

        if (doClean) clean();
        return std::move(atlas);
    }

    [[nodiscard]] Pool<Wth...> pool(bool doClean = true) {
        auto eView = m_view.view(doClean);
        M_WARN_IFF(std::distance(eView.begin(), eView.end()) > 1,
            "View size is greater than one. Consider using \
            an Atlas instead. Undefined behaviour is \
            otherwise expected"
        );

        return std::move(m_registry->get<Wth...>(*eView.begin()));
    }

private:
    View<Include<VwInc...>, Exclude<VwEx...>> m_view;
    entt::registry* m_registry;
};


template<typename... Wth>
struct DynamicQuery<With<Wth...>> {
    static_assert(sizeof...(Wth) > 0, "With<> is required to have at least one component");

    void apply(Contex<contex::READ>& contex) { contex.}
    void clean() { m_registry = nullptr; }

    [[nodiscard]] Atlas<Wth...> atlas(const DynamicView& view, bool doClean = true) {
        Atlas<Wth...> atlas;
        atlas.reserve(std::distance(view.begin(), view.end()));
        for (const auto& entity : view) {
            atlas.push_back(std::move(m_registry->get<Wth...>(entity)));
        }

        if (doClean) clean();
        return std::move(atlas);
    }

    [[nodiscard]] Pool<Wth...> pool(const DynamicView& view, bool doClean = true) {
        M_WARN_IFF(std::distance(view.begin(), view.end()) > 1,
            "View size is greater than one. Consider using \
            an Atlas instead. Undefined behaviour is \
            otherwise expected"
        );

        if (doClean) clean();
        return std::move(m_registry->get<Wth...>(*view.begin()));
    }

private:
    entt::registry* m_registry;
};

}
