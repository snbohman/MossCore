/*
core/read.hpp

All structs/types related to the 
read operations of contex.
*/

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/defines.hpp>
#include <moss/core/ecs/primitives/component.hpp>


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
    static_assert(sizeof...(Inc) > 0, "Include<> is required to have at least one component");

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

template<typename... C>
using Atlas = std::vector<Pool<C...>>;


/////////////////////
//// -- Query -- ////
/////////////////////
template<typename W, typename Vw> struct Query;

template<typename... W, typename... Vw>
struct Query<With<W...>, View<Vw...>> {
    static_assert(sizeof...(W) > 0, "With<> is required to have at least one component");

    [[nodiscard]] Atlas<W...> atlas(entt::registry& registry) {
        Atlas<W...> atlas;
        View<Vw...> view;
        auto eView = view.apply(registry);

        atlas.reserve(std::distance(eView.begin(), eView.end()));

        for (const auto& entity : eView) {
            atlas.push_back(std::move(registry.get<W...>(entity)));
        }

        return std::move(atlas);
    }

    [[nodiscard]] Pool<W...> pool(entt::registry& registry) {
        View<Vw...> view;
        auto eView = view.apply(registry);
        ERROR_IFF(std::distance(eView.begin(), eView.end()) > 2, "View size is greater than one. Consider using an Atlas instead");

        return std::move(registry.get<W...>(*eView.begin()));
    }
};

} // moss
