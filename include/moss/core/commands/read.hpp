/*
core/commands/read.hpp

All structs/types related to the 
read operations/commands.
*/

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/defines.hpp>
#include <moss/core/contex.hpp>
#include <moss/core/commands/primitives.hpp>
#include <moss/core/ecs/primitives/component.hpp>


namespace moss {

template<typename... Inc, typename... Ex>
struct View<Include<Inc...>, Exclude<Ex...>> {
    static_assert(sizeof...(Inc), "Include<> is required to have at least one component specified");

    [[nodiscard]] auto apply(Contex<contex::READ>& contex) {
        return std::move(apply(*contex.registry));
    }

    [[nodiscard]] auto apply(entt::registry& registry) {
        if constexpr (sizeof...(Ex) == 0) {
            return std::move(registry.view<Inc...>());
        } else {
            return std::move(registry.view<Inc...>(entt::exclude<Ex...>));
        }
    }
};


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
        return std::move(atlas(*contex.registry));
    }
 
    [[nodiscard]] Pool<Wth...> pool(Contex<contex::READ>& contex) {
        return std::move(pool(*contex.registry));
    }

private:
    View<Include<VwInc...>, Exclude<VwEx...>> m_view;
};

} // moss
