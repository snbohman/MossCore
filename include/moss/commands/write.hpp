#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/defines.hpp>
#include <moss/core/contex.hpp>
#include <moss/core/commands/primitives.hpp>


namespace moss {

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
