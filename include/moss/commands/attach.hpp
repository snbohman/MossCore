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
class Attach<With<Wth...>, View<Include<VwInc...>, Exclude<VwEx...>>> {
public:
    M_SA(sizeof...(Wth) > 0, "With<> is required to have at least one component");

    Attach() = default;
    Attach(const Key<key::READ>& key) { apply(key); }

    static Attach init() { return Attach(); }
    static Attach init(const Key<key::READ>& key) { return Attach(key); }

    void apply(Key<key::READ> key) { m_registry = key.m_registry; m_view.apply(key); }
    void reg(entt::registry* reg) { m_registry = reg; m_view.reg(reg); }
    void clean() { m_registry = nullptr; m_view.clean(); }

    [[nodiscard]] Pool<Wth...> pool(bool doClean = false) {
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


        Pool<Wth...> p = { m_registry->emplace<Wth>(*view.begin())... };

        if (doClean) clean();
        return std::move(p);
    }

    [[nodiscard]] Atlas<Wth...> atlas(bool doClean = false) {
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
             atlas.push_back(std::move({ m_registry->emplace<Wth>(entity)... }));
        }

        if (doClean) clean();
        return std::move(atlas);
    }

private:
    View<Include<VwInc...>, Exclude<VwEx...>> m_view;
    entt::registry* m_registry;
};

template<typename... Wth>
class DynamicAttach<With<Wth...>> {
public:
    M_SA(sizeof...(Wth) > 0, "With<> is required to have at least one component");

    DynamicAttach() = default;
    DynamicAttach(const Key<key::READ>& key) { apply(key); }
    DynamicAttach(const Key<key::WRITE>& key) { apply(key); }

    static DynamicAttach init() { return DynamicAttach(); }
    static DynamicAttach init(const Key<key::READ>& key) { return DynamicAttach(key); }
    static DynamicAttach init(const Key<key::WRITE>& key) { return DynamicAttach(key); }

    void apply(Key<key::READ> key) { m_registry = key.m_registry; }
    void apply(Key<key::WRITE> key) { m_registry = key.m_registry; }
    void reg(entt::registry* reg) { m_registry = reg; }
    void clean() { m_registry = nullptr; }

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


        Pool<Wth...> p = { m_registry->emplace<Wth>(*view.begin())... };

        if (doClean) clean();
        return std::move(p);
    }

    [[nodiscard]] Atlas<Wth...> atlas(const DynamicView& view, bool doClean = false) {
        M_ERROR_IFF(m_registry == nullptr,
            "Registry is null. Note that apply must be called before any get method"
        );
        M_ERROR_IFF(view.size() == 0,
            "View size is zero. Undefined behaviour is expected"
        );

        Atlas<Wth...> atlas;
        atlas.reserve(view.size());
        for (auto& entity : view) {
             atlas.push_back(std::move({ m_registry->emplace<Wth>(entity)... }));
        }

        if (doClean) clean();
        return std::move(atlas);
    }

private:
    entt::registry* m_registry;
};

}
