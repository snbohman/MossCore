/**
 * @file commands/attach.hpp
 * @brief Attach command specification
 *
 * Simple attach component command used
 * by systems.
 *
 * Divided into two groups, Attach and dynamic
 * attach. Attach attaches components onto
 * the tempalted given view. Dynamic attach
 * attaches components, altough templated
 * to a DynamicView.
 */

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/logs.hpp>
#include <moss/core/key.hpp>
#include <moss/commands/view.hpp>
#include <moss/commands/primitives.hpp>


namespace moss::commands {

template<typename... Wth, typename... VwInc, typename... VwEx>
requires(sizeof...(Wth) > 0)
class Attach<With<Wth...>, View<Include<VwInc...>, Exclude<VwEx...>>> {
public:
    Attach() = default;
    Attach(const Key<key::READ>& key) { apply(key); }
    Attach(entt::registry* registry) { apply(registry); }

    static Attach init() { return Attach(); }
    static Attach init(const Key<key::READ>& key) { return Attach(key); }
    static Attach init(entt::registry* registry) { return Attach(registry); }

    void apply(Key<key::READ> key) { m_registry = key.m_registry; m_view.apply(key); }
    void apply(entt::registry* registry) { m_registry = registry; m_view.apply(registry); }
    void clean() { m_registry = nullptr; m_view.clean(); }

    template<typename... Args>
    [[nodiscard]] Pool<Wth...> pool(bool doClean = false, Args&&... args) {
        M_ERROR_IFF(m_registry == nullptr,
            "Registry is null. Note that apply must be called before any get method"
        );

        auto view = m_view.view(doClean);
        M_ERROR_IFF(view.size() > 1,
            "View size is greater than one. Consider using \
            an Atlas instead. Undefined behaviour is \
            otherwise expected"
        );
        M_ERROR_IFF(view.size() == 0,
            "View size is zero. Undefined behaviour is expected"
        );

        Pool<Wth...> p(
            m_registry->emplace<Wth>(*view.begin(), args...)...
        );

        if (doClean) clean();
        return std::move(p);
    }

    template<typename... Args>
    [[nodiscard]] Atlas<Wth...> atlas(bool doClean = false, Args&&... args) {
        M_ERROR_IFF(m_registry == nullptr,
            "Registry is null. Note that apply must be called before any get method"
        );

        auto view = m_view.view(doClean);
        M_ERROR_IFF(view.size() == 0,
            "View size is zero. Undefined behaviour is expected"
        );

        Atlas<Wth...> atlas;
        atlas.reserve(view.size());
        for (auto& entity : view) {
            atlas.push_back(Pool<Wth...>(
                m_registry->emplace<Wth>(*view.begin(), args...)...
            ));
        }

        if (doClean) clean();
        return std::move(atlas);
    }

private:
    View<Include<VwInc...>, Exclude<VwEx...>> m_view;
    entt::registry* m_registry;
};

template<typename... Wth>
requires(sizeof...(Wth) > 0)
class DynamicAttach<With<Wth...>> {
public:
    DynamicAttach() = default;
    DynamicAttach(const Key<key::READ>& key) { apply(key); }
    DynamicAttach(const Key<key::WRITE>& key) { apply(key); }
    DynamicAttach(entt::registry* registry) { apply(registry); }

    static DynamicAttach init() { return DynamicAttach(); }
    static DynamicAttach init(const Key<key::READ>& key) { return DynamicAttach(key); }
    static DynamicAttach init(const Key<key::WRITE>& key) { return DynamicAttach(key); }
    static DynamicAttach init(entt::registry* registry) { return DynamicAttach(registry); }

    void apply(Key<key::READ> key) { m_registry = key.m_registry; }
    void apply(Key<key::WRITE> key) { m_registry = key.m_registry; }
    void apply(entt::registry* registry) { m_registry = registry; }
    void clean() { m_registry = nullptr; }

    template<typename... Args>
    [[nodiscard]] Pool<Wth...> pool(const DynamicView& view, bool doClean = false, Args&&... args) {
        M_ERROR_IFF(m_registry == nullptr,
            "Registry is null. Note that apply must be called before any get method"
        );
        M_ERROR_IF(view.size() > 1,
            "View size ({}) is greater than one. Consider using \
            an Atlas instead. Undefined behaviour is \
            otherwise expected"
        , view.size());
        M_ERROR_IFF(view.size() == 0,
            "View size is zero. Undefined behaviour is expected"
        );


        Pool<Wth...> p(
            m_registry->emplace<Wth>(*view.begin(), args...)...
        );

        if (doClean) clean();
        return std::move(p);
    }

    template<typename... Args>
    [[nodiscard]] Atlas<Wth...> atlas(const DynamicView& view, bool doClean = false, Args&&... args) {
        M_ERROR_IFF(m_registry == nullptr,
            "Registry is null. Note that apply must be called before any get method"
        );
        M_ERROR_IFF(view.size() == 0,
            "View size is zero. Undefined behaviour is expected"
        );

        Atlas<Wth...> atlas;
        atlas.reserve(view.size());
        for (auto& entity : view) {
            atlas.push_back(Pool<Wth...>(
                m_registry->emplace<Wth>(*view.begin(), args...)...
            ));
        }

        if (doClean) clean();
        return std::move(atlas);
    }

private:
    entt::registry* m_registry;
};

}
