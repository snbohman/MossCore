/*
commands/query.hpp

The query. Queries for components from a view.
It stores it's components into an atlas or
a pool.

*/

#include <moss/meta/defines.hpp>
#include <moss/core/contex.hpp>
#include <moss/core/fluent.hpp>
#include <moss/commands/primitives.hpp>

namespace moss::commands {
namespace read {

template<typename... Wth, typename... VwInc, typename... VwEx>
struct Query<With<Wth...>, View< Include<VwInc...>, Exclude<VwEx...> >> {
    static_assert(sizeof...(Wth) > 0, "With<> is required to have at least one component");

    void apply(entt::registry& registry) { m_registry = &registry; m_view.apply(registry); }
    void clean() { m_registry = nullptr; }

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

}

namespace write {

template<typename... Wth>
struct DynamicQuery<With<Wth...>> {
    static_assert(sizeof...(Wth) > 0, "With<> is required to have at least one component");

    void apply(entt::registry& registry) { m_registry = &registry; }
    void clean() { m_registry = nullptr; }

    [[nodiscard]] read::Atlas<Wth...> atlas(const DynamicView& view, bool doClean = true) {
        read::Atlas<Wth...> atlas;
        atlas.reserve(std::distance(view.begin(), view.end()));
        for (const auto& entity : view) {
            atlas.push_back(std::move(m_registry->get<Wth...>(entity)));
        }

        if (doClean) clean();
        return std::move(atlas);
    }

    [[nodiscard]] read::Pool<Wth...> pool(const DynamicView& view, bool doClean = true) {
        M_WARN_IFF(std::distance(view.begin(), view.end()) > 1,
            "View size is greater than one. Consider using \
            an Atlas instead. Undefined behaviour is \
            otherwise expected"
        );

        return std::move(m_registry->get<Wth...>(*view.begin()));
    }

private:
    entt::registry* m_registry;

};

}

}
