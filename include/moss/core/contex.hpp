/*
core/ecs/contex.hpp

Pool query = contex
    .read<View<With<proj::PlayerTag>, Out<cmp::Transform>>()
    .read<Query<cmp::Transform, cmp::Physics>>()
    .write<Pool>();

*/

#pragma once

#include <moss/core/app.hpp>
#include <moss/core/ecs/read.hpp>


namespace moss{

namespace contex {

enum Permissions {
    READ,
    WRITE
};

} // contex

template<contex::Permissions P>
class Contex {
public:
    Contex(App* app) { m_app = app; }
    ~Contex() { }

    template<contex::Permissions T> static Contex<T>& init(const App& app) {
        static Contex<T> instance(app);
        return instance;
    }

    Contex& create(glm::u32 count = 1) { m_app->create(count); }
    template<typename... T> Contex& attachComponent() { m_app->attachComponent<T...>(); }
    template<typename... T> Contex& attachSystem() { m_app->attachSystem<T...>(); };
    template<typename... T> Contex& attachPackage() { m_app->attachSystem<T...>(); };

private:
    App* m_app;

    View m_latestView;
    Query m_latestQuery;
    Pool m_latestPool;
};

} // namsespace moss
