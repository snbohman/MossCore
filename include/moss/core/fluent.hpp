#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/defines.hpp>
#include <moss/commands/primitives.hpp>
#include <memory>
#include <mutex>

namespace moss {

namespace fluent {

enum Permissions {
    READ = 1 << 0,
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

    ///////////////////
    //// -- ECS -- ////
    ///////////////////
    Fluent<P>& quit() { }

    Fluent& create(glm::u32 count = 1) {
        m_view.clear();
        m_view.reserve(count);

        for (glm::u32 i = 0; i < count; i++) {
            m_view.push_back(m_registry->create());
        }

        return *this;
    }

    Fluent& view() { }
    Fluent& query() { }

    ///////////////////////////////
    //// -- ECS (Templated) -- ////
    ///////////////////////////////
    template<typename... T> Fluent& attach() {
        for (entt::entity entity : m_view) m_registry->emplace<T...>(entity);
        return *this;
    }
 
private:
    Fluent() = default;

    static inline std::unique_ptr<Fluent<P>> s_instance = nullptr;
    static inline std::mutex s_mutex;

    entt::registry* m_registry;
    commands::write::DynamicView m_view;
};

}
