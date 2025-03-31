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

////////////////////
//// -- Base -- ////
////////////////////
using Entity = entt::entity;

template<typename... Ts>
struct Include { };

template<typename... Ts>
struct Exclude { };

template<typename... Ts>
struct With { };


/////////////////////
//// -- Write -- ////
/////////////////////
namespace  commands::write {

using DynamicView = std::vector<Entity>;

template<typename... T>
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
