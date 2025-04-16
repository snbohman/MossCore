/**
 * @file    commands/primitives.hpp
 * @brief   Seperation of declerations of commands.
 *
 * Seperation used for importing purposes.
 * Includes all declarations of the base 
 * types and structs, but no further
 * definitions.
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


///////////////////////
//// -- Outputs -- ////
///////////////////////
using DynamicView = std::vector<Entity>;

template<typename... C>
using Pool = std::tuple<C&...>;

template<typename... C>
using Atlas = std::vector<Pool<C&...>&>;

////////////////////////
//// -- Commands -- ////
////////////////////////
namespace commands {

template<typename Include, typename Exclude>
class View;

template<typename With, typename View>
class Query;

template<typename With>
class DynamicQuery;

template<int N>
class Create;

template<typename Components, typename View>
class Attach;

template<typename With>
class DynamicAttach;

class Quit;

}

}
