/*
core/commands/primitives.hpp

Seperation used for importing purposes.
Includes all declarations of the base 
types and structs, but no further
definitions.

*/

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/ecs/component.hpp>


namespace moss {

/////////////////////
//// -- Types -- ////
/////////////////////
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


/////////////////////
//// -- Write -- ////
/////////////////////
namespace  commands::write {

using DynamicView = std::vector<entt::entity>;

template<typename With>
struct DynamicQuery;

}

////////////////////
//// -- Read -- ////
////////////////////
namespace commands::read {

template<typename Include, typename Exclude>
struct View;

template<typename... C>
using Pool = std::tuple<C...>;

template<typename... C>
using Atlas = std::vector<Pool<C...>&>;

template<typename With, typename View>
struct Query;

}

}
