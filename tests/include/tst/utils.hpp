#pragma once

#include <moss/moss.hpp>
#include <tst/components.hpp>


namespace tst {

/*
Entities:
A: CompA
B: CompB
C: CompC
D: CompA + CompB
E: CompB + CompC
F: CompA + CompC

Layout:

CompA - size 3: A - D - F
NOT:            B - C - E

CompB - size 3: B - D - E
NOT:            A - C - F

CompC - size 3: C - E - F
NOT:            A - B - D
*/
inline entt::registry newRegistry() {
    entt::registry reg;

    auto A = reg.create();
    auto B = reg.create();
    auto C = reg.create();
    auto D = reg.create();
    auto E = reg.create();
    auto F = reg.create();

    reg.emplace<CompA>(A, "EntA");
    reg.emplace<CompB>(B, "EntB");
    reg.emplace<CompC>(C, "EntC");

    reg.emplace<CompA>(D, "EntD");
    reg.emplace<CompB>(D, "EntD");

    reg.emplace<CompB>(E, "EntE");
    reg.emplace<CompC>(E, "EntE");

    reg.emplace<CompA>(F, "EntF");
    reg.emplace<CompC>(F, "EntF");

    return std::move(reg);
}

inline u32 vSize(auto view) {
    return std::distance(view.begin(), view.end());
}

}
