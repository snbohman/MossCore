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

#include <moss/meta/log.hpp>
#include <moss/core/key.hpp>
#include <moss/core/context.hpp>
#include <moss/commands/primitives.hpp>

namespace moss {

template<typename... Wth, typename... VwInc, typename... VwEx>
class Query<With<Wth...>, View< Include<VwInc...>, Exclude<VwEx...> >> {
public:
    M_SA(sizeof...(Wth) > 0, "With<> is required to have at least one component");

    Query() = default;
    Query(const Key<key::READ>& key) { apply(key); }

    static Query init() { return Query(); }
    static Query init(const Key<key::READ>& key) { return Query(key); }

    void apply(const Key<key::READ>& key) { m_registry = key.m_registry; m_view.apply(key); }
    void clean() { m_registry = nullptr; m_view.clean(); }

    [[nodiscard]] Atlas<Wth...> atlas(bool doClean = false) {
        M_ERROR_IFF(m_registry == nullptr,
            "Registry is null. Note that apply must be called before any get method"
        );

        auto eView = m_view.view(doClean);
        M_ERROR_IFF(std::distance(eView.begin(), eView.end()) == 0,
            "View size is zero. Undefined behaviour is expected"
        );

        Atlas<Wth...> atlas;
        atlas.reserve(std::distance(eView.begin(), eView.end()));
        for (const auto& entity : eView) {
            atlas.push_back(std::move({ m_registry->get<Wth>(entity)... }));
        }

        if (doClean) clean();
        return std::move(atlas);
    }

    [[nodiscard]] Pool<Wth...> pool(bool doClean = false) {
        M_ERROR_IFF(m_registry == nullptr,
            "Registry is null. Note that apply must be called before any get method"
        );

        auto eView = m_view.view(doClean);
        M_ERROR_IFF(std::distance(eView.begin(), eView.end()) > 1,
            "View size is greater than one. Consider using \
            an Atlas instead. Undefined behaviour is \
            otherwise expected"
        );
        M_ERROR_IFF(std::distance(eView.begin(), eView.end()) == 0,
            "View size is zero. Undefined behaviour is expected"
        );

        Pool pool = { m_registry->get<Wth>(*eView.begin())... };
        if (doClean) clean();
        return std::move(pool);
    }

private:
    View<Include<VwInc...>, Exclude<VwEx...>> m_view;
    entt::registry* m_registry;
};


/**
 * @brief Wrapper around query without Exclude inside View
 *
 * Simple wrapper around the norma query. Has a query object
 * lying inside and calling that whenever.
 */
template<typename... Wth, typename... VwInc>
class Query<With<Wth...>, View< Include<VwInc...>> > {
public:
    M_SA(sizeof...(Wth) > 0, "With<> is required to have at least one component");

    Query() = default;
    Query(const Key<key::READ>& key) { q.apply(key); }

    static Query init() { return Query(); }
    static Query init(const Key<key::READ>& key) { return Query(key); }

    void apply(const Key<key::READ>& key) { q.apply(key); }
    void apply(const Key<key::WRITE>& key) { q.apply(key); }
    void clean() { q.clean(); }
    
    [[nodiscard]] Atlas<Wth...> atlas(bool doClean = false) { return std::move(q.atlas(doClean)); }
    [[nodiscard]] Pool<Wth...> pool(bool doClean = false) { return std::move(q.pool(doClean)); }

private:
    Query<With<Wth...>, View<Include<VwInc...>, Exclude<>>> q;
};

template<typename... Wth>
class DynamicQuery<With<Wth...>> {
public:
    M_SA(sizeof...(Wth) > 0, "With<> is required to have at least one component");

    DynamicQuery() = default;
    DynamicQuery(const Key<key::READ>& key) { apply(key); }
    DynamicQuery(const Key<key::WRITE>& key) { apply(key); }

    static DynamicQuery init() { return DynamicQuery(); }
    static DynamicQuery init(const Key<key::READ>& key) { return DynamicQuery(key); }
    static DynamicQuery init(const Key<key::WRITE>& key) { return DynamicQuery(key); }

    void apply(const Key<key::READ>& key) { m_registry = key.m_registry; }
    void apply(const Key<key::WRITE>& key) { m_registry = key.m_registry; }
    void clean() { m_registry = nullptr; }

    entt::registry* getReg() { return m_registry; }

    [[nodiscard]] Atlas<Wth...> atlas(const DynamicView& view, bool doClean = false) {
        M_ERROR_IFF(m_registry == nullptr,
            "Registry is null. Note that apply must be called before any get method"
        );
        M_ERROR_IFF(view.size() == 0,
            "View size is zero. Undefined behaviour is expected"
        );

        Atlas<Wth...> atlas;
        atlas.reserve(view.size());
        for (const auto& entity : view) {
            atlas.push_back(std::move({ m_registry->get<Wth>(entity)... }));
        }

        if (doClean) clean();
        return std::move(atlas);
    }

    [[nodiscard]] Pool<Wth...> pool(const DynamicView& view, bool doClean = false) {
        M_ERROR_IFF(m_registry == nullptr,
            "Registry is null. Note that apply must be called before any get method"
        );
        M_ERROR_IFF(view.size() > 1,
            "View size is greater than one. Consider using \
            an Atlas instead. Undefined behaviour is \
            otherwise expected"
        );
        M_ERROR_IFF(view.size() == 0,
            "View size is zero. Undefined behaviour is expected"
        );

        Pool pool = { m_registry->get<Wth>(*view.begin())... };
        if (doClean) clean();
        return std::move(pool);
    }

private:
    entt::registry* m_registry;
};

} // moss::commands
