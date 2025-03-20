/*
core/contex.hpp
*/

#pragma once

#include <moss/core/app.hpp>


namespace moss{

namespace contex {
enum Permissions {
    CREATE,
    ATTACH,
    READ
};
} // contex


/*
-- app

Contex<contex::Permissions::CREATE>::init() contex;
sys(contex);

-- innto param. in system:

Query query = contex.query<Transform, Physics>(contex.view<PlayerTag>())
contex.create().attachComponent<Transform>()

-- back to app
contex.exit();

A static assert will be called on every contex func to ensure 
proper permissions.

Users wont bbe able to create their own contexes with their own
permissions because they nneed app to initialize contex.

*/

template<contex::Permissions P>
class Contex {
public:
    Contex(const App& app) { m_app = app; }
    ~Contex() { }

    template<contex::Permissions T> static Contex<T>& init(const App& app) {
        static Contex<T> instance(app);
        return instance;
    }

    Contex& create(glm::u32 count = 1) { m_app.create(count); }
    template<typename... T> Contex& attachComponent() { m_app.attachComponent<T...>(); }
    template<typename... T> Contex& attachSystem() { m_app.attachSystem<T...>(); };
    template<typename... T> Contex& attachPackage() { m_app.attachSystem<T...>(); };

private:
    App& m_app;
};

}
