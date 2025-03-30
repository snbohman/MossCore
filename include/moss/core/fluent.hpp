/*
core/writer.hpp

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

Fluent::init()
    .create()
        .attach<PlayerTag, PlayerMovement, Position>()
    .create(10)
        .attach<EnemyTag, EnemyMovement, Position>();

<<<< ---- >>>>

Fluent::get();     // THROWS RUNTIME ERROR
Fluent::init();
Fluent::get();     // RUNS
Fluent::destroy();
Fluent::get();     // THROWS RUNTIME ERROR

*/


#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/defines.hpp>
#include <moss/commands/primitives.hpp>
#include <memory>
#include <mutex>

namespace moss {

namespace fluent {

/* Only write permissions available for fluent */
enum Permissions {
    WRITE = 1 << 1,
};

class MutexBase {
protected:
    MutexBase() {
        std::lock_guard<std::mutex> lock(s_mutex);
        M_ERROR_IFF(s_initialized, "Fluent instance already exists");
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

template<fluent::Permissions P>
class Fluent : public fluent::MutexBase {
public:
    /////////////////////////
    //// -- Operators -- ////
    /////////////////////////
    Fluent(const Fluent&) = delete;
    Fluent& operator=(const Fluent&) = delete;
    Fluent(Fluent&&) = delete;
    Fluent& operator=(Fluent&&) = delete;

    ///////////////////////////
    //// -- Mutex Locks -- ////
    ///////////////////////////
    static Fluent<P>& init() {
        std::lock_guard<std::mutex> lock(s_mutex);
        M_ERROR_IFF(s_instance, "Fluent instance already initialized");

        s_instance = std::make_unique<Fluent<P>>();
        return *s_instance;
    }

    static Fluent<P>& get() {
        std::lock_guard<std::mutex> lock(s_mutex);
        M_ERROR_IFF(!s_instance, "Fluent instance not initialized");

        return *s_instance;
    }

    static bool exists() {
        std::lock_guard<std::mutex> lock(s_mutex);
        return s_instance != nullptr;
    }

    static void destroy() {
        std::lock_guard<std::mutex> lock(s_mutex);
        s_instance.reset();
    }

    ////////////////////////
    //// -- User ECS -- ////
    ////////////////////////
    Fluent<P>& quit() { }

    Fluent<P>& create(glm::u32 count = 1) {
        m_view.clear();
        m_view.reserve(count);

        for (glm::u32 i = 0; i < count; i++) {
            m_view.push_back(m_registry->create());
        }

        return *this;
    }

    template<typename... T> Fluent& attach() {
        for (entt::entity entity : m_view)
            m_registry->emplace<T...>(entity);

        return *this;
    }

 
private:
    Fluent() = default;

    static inline std::unique_ptr<Fluent> s_instance = nullptr;
    static inline std::mutex s_mutex;

    entt::registry* m_registry;
    commands::write::DynamicView m_view;
};

}
