/*
core/read.hpp

All structs/types related to the 
read operations of contex.
*/

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/defines.hpp>
#include <moss/core/ecs/primitives/component.hpp>
#include <moss/core/contex.hpp>


namespace moss {

///////////////////////
//// -- General -- ////
///////////////////////
template <typename... T>
struct Types {
    using types = std::tuple<T...>;
};

template<typename... T>
struct Include : public Types<T...> { };

template<typename... T>
struct Exclude : public Types<T...> { };

template<typename... T>
struct With : public Types<T...> { };


////////////////////
//// -- View -- ////
////////////////////
template<typename IncludeT, typename ExcludeT = std::tuple<>>
struct View;

template<typename... Inc, typename... Ex>
struct View<Include<Inc...>, Exclude<Ex...>> {
    static_assert(sizeof...(Inc), "Include<> is required to have at least one component specified");

    [[nodiscard]] auto apply(Contex<contex::READ>& contex) {
        entt::registry* registry;
        contex.loadRegistry(registry);

        if constexpr (sizeof...(Ex) == 0) {
            return std::move(registry->view<Inc...>());
        } else {
            return std::move(registry->view<Inc...>(entt::exclude<Ex...>));
        }
    }

    [[nodiscard]] auto apply(entt::registry& registry) {
        if constexpr (sizeof...(Ex) == 0) {
            return std::move(registry.view<Inc...>());
        } else {
            return std::move(registry.view<Inc...>(entt::exclude<Ex...>));
        }
    }
};


/////////////////////
//// -- Atlas -- ////
/////////////////////
template<typename... C>
using Pool = std::tuple<C...>;

/*
Lists of Pools. Might seem useless, when entities are unkown,
but if I were to reimplement this as suggested, it would
essentially just become a view. It has many usecases, but not
the one above, and it will never have. The user will just have
to manipulate their views better.
*/
template<typename... C>
using Atlas = std::vector<Pool<C...>>;


/////////////////////
//// -- Query -- ////
/////////////////////
template<typename...>
struct Query;

template<typename... Wth, typename... VwInc, typename... VwEx>
struct Query<With<Wth...>, View< Include<VwInc...>, Exclude<VwEx...> >> {
    static_assert(sizeof...(Wth) > 0, "With<> is required to have at least one component");

    [[nodiscard]] Atlas<Wth...> atlas(entt::registry& registry) {
        Atlas<Wth...> atlas;
        auto eView = m_view.apply(registry);
        atlas.reserve(std::distance(eView.begin(), eView.end()));

        for (const auto& entity : eView) {
            atlas.push_back(std::move(registry.get<Wth...>(entity)));
        }

        return std::move(atlas);
    }

    [[nodiscard]] Pool<Wth...> pool(entt::registry& registry) {
        auto eView = m_view.apply(registry);
        M_WARN_IFF(std::distance(eView.begin(), eView.end()) > 1,
            "View size is greater than one. Consider using \
            an Atlas instead. Undefined behaviour is \
            otherwise expected"
        );

        return std::move(registry.get<Wth...>(*eView.begin()));
    }

    [[nodiscard]] Atlas<Wth...> atlas(Contex<contex::READ>& contex) {
        entt::registry* registry;
        contex.loadRegistry(registry);

        return std::move(atlas(*registry));
    }
 
    [[nodiscard]] Pool<Wth...> pool(Contex<contex::READ>& contex) {
        entt::registry* registry;
        contex.loadRegistry(registry);

        return std::move(pool(*registry));
    }

private:
    View<Include<VwInc...>, Exclude<VwEx...>> m_view;
};


///////////////////////////
//// -- DynamicView -- ////
///////////////////////////
using DynamicView = std::vector<entt::entity>;


////////////////////////////
//// -- DynamicQuery -- ////
////////////////////////////
/*
So a dynamic view, a runtime view, is provided in the
atlas and pool statments. So the only compile time
thing about this is what components to get.

Contex::WRITE permissions is only required here,
as actually reading with DynamicView is impossible,
and is only provided by the app to the system.
*/


template<typename... Wth>
struct Query<With<Wth...>> {
    static_assert(sizeof...(Wth) > 0, "With<> is required to have at least one component");

    [[nodiscard]] Atlas<Wth...> atlas(entt::registry& registry, const DynamicView& view) {
        Atlas<Wth...> atlas;
        for (entt::entity entity : view) {
            atlas.push_back(std::move(registry.get<Wth...>(entity)));
        }

        return std::move(atlas);
    };

    [[nodiscard]] Pool<Wth...> pool(entt::registry& registry, const DynamicView& view) {
        M_WARN_IFF(std::distance(view.begin(), view.end()) > 1,
            "View size is greater than one. Consider using \
            an Atlas instead. Undefined behaviour is \
            otherwise expected"
        );

        return std::move(registry.get<Wth...>(*view.begin()));
    };

    [[nodiscard]] Atlas<Wth...> atlas(Contex<contex::READ>& contex, const DynamicView& view) {
        entt::registry* registry;
        contex.loadRegistry(registry);

        return std::move(atlas(*registry, view));
    };

    [[nodiscard]] Pool<Wth...> pool(Contex<contex::READ>& contex, const DynamicView& view) {
        entt::registry* registry;
        contex.loadRegistry(registry);

        return std::move(pool(*registry, view));
    };

    [[nodiscard]] Atlas<Wth...> atlas(Contex<contex::WRITE>& contex, const DynamicView& view) {
        entt::registry* registry;
        contex.loadRegistry(registry);

        return std::move(atlas(*registry, view));
    };

    [[nodiscard]] Pool<Wth...> pool(Contex<contex::WRITE>& contex, const DynamicView& view) {
        entt::registry* registry;
        contex.loadRegistry(registry);

        return std::move(pool(*registry, view));
    };
};

} // moss
