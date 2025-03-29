/*
core/commands/primitives.hpp

Seperation used for importing purposes.
Includes all declarations of the base 
types and structs, but no further
definitions.

*/

#pragma once

#include <moss/meta/libs.hpp>


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

///////////////////////////
//// -- DynamicView -- ////
///////////////////////////
using DynamicView = std::vector<entt::entity>;

////////////////////
//// -- View -- ////
////////////////////
template<typename IncludeT, typename ExcludeT = std::tuple<>>
struct View;

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

}
