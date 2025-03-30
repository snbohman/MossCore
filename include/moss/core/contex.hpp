/*
core/app.hpp - parentless

Core application.

Maybe move the attach stuff too
it's own logic, like contex plans.

*/

#pragma once


#include <moss/meta/libs.hpp>
#include <moss/ecs/component.hpp>
#include <moss/commands/primitives.hpp>

namespace moss {

namespace contex {

enum Permissions {
    READ = 1 << 0,
    WRITE = 1 << 1,
};

}

template<contex::Permissions P>
class Contex {
public:
    Contex(const Contex&) = delete;
    Contex& operator=(const Contex&) = delete;
    Contex(Contex&&) = delete;
    Contex& operator=(Contex&&) = delete;

    entt::registry& registry;
};

} // moss
