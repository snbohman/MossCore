/*
core/Fluent.cpp - parentless

Implements core/Fluent.hpp.

*/

#include <moss/meta/libs.hpp>
#include <moss/core/fluent.hpp>


using moss::Fluent;


////////////////////////
//// -- Lifetime -- ////
////////////////////////
Fluent& Fluent::init() {
    static Fluent instance;
    return instance;
}

Fluent& Fluent::get() {
    return *this;
}

///////////////////
//// -- ECS -- ////
///////////////////
Fluent& Fluent::create(glm::u32 count) {
    m_latest.clear();
    m_latest.reserve(count);

    for (glm::u32 i = 0; i < count; i++) {
        m_latest.push_back(m_registry.create());
    }

    return *this;
}

Fluent& Fluent::quit() {
    m_quit = true;
    return *this;
}
