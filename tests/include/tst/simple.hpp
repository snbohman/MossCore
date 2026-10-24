#pragma once

#include <moss/moss.hpp>

#include <algorithm>
#include <string>

/*
 * Small self-contained components and a ready-made world, independent of
 * tst/components.hpp, so the command tests state their own expectations.
 */
namespace tst {

struct Pos  { int x = 0;  int y = 0; };
struct Vel  { int dx = 0; int dy = 0; };
struct Name { std::string text; };
struct Flag { };                         // tag (empty)
struct Ctor {                            // not an aggregate, no default constructor
    Ctor(int a, int b) : sum(a + b) { }
    int sum;
};

/*
 *   a: Pos{1,1}  Vel{10,10}
 *   b: Pos{2,2}  Vel{20,20}  Flag
 *   c: Pos{3,3}  Flag
 *   d:           Vel{40,40}
 *   e: Name{"solo"}
 */
struct World {
    entt::registry reg;
    moss::Entity a{}, b{}, c{}, d{}, e{};

    World() {
        a = reg.create();
        reg.emplace<Pos>(a, Pos{ 1, 1 });
        reg.emplace<Vel>(a, Vel{ 10, 10 });

        b = reg.create();
        reg.emplace<Pos>(b, Pos{ 2, 2 });
        reg.emplace<Vel>(b, Vel{ 20, 20 });
        reg.emplace<Flag>(b);

        c = reg.create();
        reg.emplace<Pos>(c, Pos{ 3, 3 });
        reg.emplace<Flag>(c);

        d = reg.create();
        reg.emplace<Vel>(d, Vel{ 40, 40 });

        e = reg.create();
        reg.emplace<Name>(e, Name{ "solo" });
    }
};

inline moss::DynamicView sorted(moss::DynamicView entities) {
    std::sort(entities.begin(), entities.end());
    return entities;
}

// Sorted list of the given entities, for comparing against sorted(...)
template<typename... E>
moss::DynamicView set(E... entities) {
    return sorted(moss::DynamicView{ entities... });
}

}
