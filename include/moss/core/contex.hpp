/*
core/contex.hpp

Utelizing the flunet design pattern, inspired
by rs-bevvy. It is dependent on "runtime". In
it's ways to increase ease of use, it has to
drop the compile time safety seen in contex
and it's structs. This operates on vectors,
heap and non compile time friendly queries.
The list goes on and on...

It might seem useless, but in less intensive
tasks, where safety isn't an issue, this is
very handy for ease of use.

It utelizes mutex locks for ensuring that
only one instance can exist, so no 
permissions will be remapped by the user.

It does not do read operations, hence the 
name. Only creation of entities and
attachments of those.

------------------------
<<<< -- Examples -- >>>>
------------------------

Contex::init()
    .create()
        .attach<PlayerTag, PlayerMovement, Position>()
    .create(10)
        .attach<EnemyTag, EnemyMovement, Position>();

<<<< ---- >>>>

Contex::get();     // THROWS RUNTIME ERROR
Contex::init();
Contex::get();     // RUNS
Contex::destroy();
Contex::get();     // THROWS RUNTIME ERROR

*/


#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/defines.hpp>
#include <moss/commands/primitives.hpp>
#include <memory>
#include <mutex>

namespace moss {

namespace contex {

enum Permissions {
    READ = 1 << 0,
    WRITE = 1 << 1
};


/* Initializing once logic */
class MutexBase {
protected:
    MutexBase() {
        std::lock_guard<std::mutex> lock(s_mutex);
        M_ERROR_IFF(s_initialized, "Contex instance already exists");
        s_initialized = true;
    }

    ~MutexBase() {
        std::lock_guard<std::mutex> lock(s_mutex);
        s_initialized = false;
    }

    static inline std::mutex s_mutex;
    static inline bool s_initialized = false;
};

}

template<contex::Permissions P>
class Contex : public contex::MutexBase {
public:
    /////////////////////////
    //// -- Operators -- ////
    /////////////////////////
    Contex(const Contex&) = delete;
    Contex& operator=(const Contex&) = delete;
    Contex(Contex&&) = delete;
    Contex& operator=(Contex&&) = delete;

    ////////////////////////////
    //// -- Public locks -- ////
    ////////////////////////////
    static Contex<P>& get() {
        std::lock_guard<std::mutex> lock(s_mutex);
        M_ERROR_IFF(!s_instance, "Contex instance not initialized");

        return *s_instance;
    }

    //////////////////////////
    //// -- Public ECS -- ////
    //////////////////////////
    Contex<P>& quit() { }

    Contex<P>& entity(glm::u32 count = 1) {
        m_view.clear();
        m_view.reserve(count);

        for (glm::u32 i = 0; i < count; i++) {
            m_view.push_back(m_registry->create());
        }

        return *this;
    }

    template<typename... T> Contex& component() {
        for (entt::entity entity : m_view)
            m_registry->emplace<T...>(entity).init();

        return *this;
    }

 
private:
    Contex() = default;
    ~Contex() = default;


    //////////////////////
    //// -- Friend -- ////
    //////////////////////
    /*
    The primitive file that we include is
    only basic definitions. So it basically
    counts as a forward declaration. See 
    deepseek chat for more info (somewhere):
        **https://chat.deepseek.com/a/chat/s/fbb442af-23f3-42a1-9f12-047245b41586**
    */
    friend class App;

    template<typename Wth, typename Vw>
    friend class commands::read::Query;

    template<typename Inc, typename Ex>
    friend class commands::read::View;

    //////////////////////////////
    //// -- Restricted ECS -- ////
    //////////////////////////////
    template<typename T>
    void inject(entt::registry* registry) {
        m_registry = registry;
    }

    void apply(entt::registry*& registry) {
        registry = m_registry;
    }

    ////////////////////////////////
    //// -- Restricted Locks -- ////
    ////////////////////////////////
    static Contex<P>& init() {
        std::lock_guard<std::mutex> lock(s_mutex);
        M_ERROR_IFF(s_instance, "Contex instance already initialized");

        s_instance = std::make_unique<Contex<P>>();
        return *s_instance;
    }

    static void destroy() {
        std::lock_guard<std::mutex> lock(s_mutex);
        s_instance.reset();
    }

    /////////////////////
    //// -- Mutex -- ////
    /////////////////////
    static inline std::unique_ptr<Contex> s_instance = nullptr;
    static inline std::mutex s_mutex;

    ///////////////////
    //// -- ECS -- ////
    ///////////////////
    entt::registry* m_registry;
    /*commands::write::DynamicView m_view;*/

};

}
