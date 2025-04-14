/*
 * @file    core/contex.hpp
 * @brief   Singleton Contex class providing access to the ECS registry.
 *
 * This class provides a singleton instance for managing ECS operations, ensuring
 * that only one instance exists to prevent conflicting permission mappings.
 * It offers two primary modes of interaction:
 *
 * 1.  **Compile-Time Safe Commands:** Utilized through command structures, offering
 *     compile-time safety and type checking for ECS operations.
 *
 * 2.  **Runtime Fluent API:** A fluent design pattern (inspired by rs-bevvy) for
 *     runtime entity creation and component attachment. This approach, while
 *     sacrificing some compile-time safety, provides a more flexible and
 *     ergonomic API for rapid prototyping or less critical tasks.  The runtime
 *     API is accessed through the `Contex` functions (e.g., `create()`, `attach()`).
 *
 * The class uses mutex locks to guarantee thread-safe access and prevent
 * multiple instances from being created.
 *
 * **Usage Examples (Runtime API):**
 *
 * ```cpp
 * Contex::init()
 *     .create()
 *         .attach<PlayerTag, PlayerMovement, Position>()
 *     .create(10)
 *         .attach<EnemyTag, EnemyMovement, Position>();
 * ```
 *
 * **Singleton Management:**
 *
 * ```cpp
 * Contex::get();     // THROWS RUNTIME ERROR if not initialized
 * Contex::init();
 * Contex::get();     // Runs successfully
 * Contex::destroy();
 * Contex::get();     // THROWS RUNTIME ERROR after destruction
 * ```
 */


#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/defines.hpp>
#include <moss/core/ecs.hpp>
#include <moss/commands/primitives.hpp>

#include <memory>
#include <mutex>


namespace moss {

namespace contex {

enum Permissions {
    READ = 1 << 0,
    WRITE = 1 << 1
};

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
    Contex<P>(const Contex<P>&) = delete;
    Contex<P>& operator=(const Contex<P>&) = delete;
    Contex<P>(Contex<P>&&) = delete;
    Contex<P>& operator=(Contex<P>&&) = delete;

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

    Contex<P>& create(glm::u32 count = 1) {
        m_view.clear();
        m_view.reserve(count);

        for (glm::u32 i = 0; i < count; i++) {
            m_view.push_back(m_registry->create());
        }

        return *this;
    }

    /* For components to latest m_view. */
    template<typename... T> Contex<P>& attach() {
        static_assert(
            std::is_base_of<Component, T...>::value,
            "Expected all of T to inherit moss::Component"
        );

        for (entt::entity entity : m_view)
            m_registry->emplace<T...>(entity).init();

        return *this;
    }

    /* For attaching systems to global system entity. */
    template<typename... T> Contex& connect() {
        m_registry->emplace<e_system>().init();

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
    friend class commands::Query;

    template<typename Wth>
    friend class commands::DynamicQuery;

    template<typename Inc, typename Ex>
    friend class commands::View;

    //////////////////////////////
    //// -- Restricted ECS -- ////
    //////////////////////////////
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
    entt::entity& e_system;

    DynamicView m_view;
};

}
