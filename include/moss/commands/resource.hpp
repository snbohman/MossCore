/**
 * @file    commands/resource.hpp
 * @brief   Resource: one shared object per type, stored in the registry.
 *
 * Wraps entt::registry::ctx(). A resource is a singleton keyed by its type, for
 * state shared between systems that cannot reference each other (a renderer
 * core and its attachments, say).
 *
 *     build (WRITE):   auto& core = cmd::Resource<RenderCore>::init(key).emplace();
 *     anywhere:        auto& core = cmd::Resource<RenderCore>::init(key).get();
 *
 *     get()        the resource; aborts if it does not exist
 *     find()       pointer, or nullptr
 *     contains()   does it exist
 *     emplace()    construct it in place; WRITE key; aborts if it already exists
 *     erase()      destroy it; WRITE key
 *
 * Do not keep the returned reference across frames: re-fetch it in tick (it is
 * one hash lookup).
 */

#pragma once

#include <moss/commands/command.hpp>

namespace moss::commands {

template<typename T>
class Resource : public Command<Resource<T>> {
public:
    Resource() = default;

    template<typename... Args>
    T& emplace(Args&&... args) const {
        this->requireWritable("Resource::emplace");
        entt::registry& reg = this->registry();
        M_FATAL_IF(reg.ctx().contains<T>(), "Resource '{}' already exists", typeid(T).name());

        return reg.ctx().emplace<T>(std::forward<Args>(args)...);
    }

    [[nodiscard]] T& get() const {
        T* resource = find();
        M_FATAL_IF(resource == nullptr, "Resource '{}' does not exist", typeid(T).name());
        return *resource;
    }

    [[nodiscard]] T* find() const {
        entt::registry& reg = this->registry();
        return reg.ctx().find<T>();
    }

    [[nodiscard]] bool contains() const {
        entt::registry& reg = this->registry();
        return reg.ctx().contains<T>();
    }

    void erase() const {
        this->requireWritable("Resource::erase");
        entt::registry& reg = this->registry();
        reg.ctx().erase<T>();
    }
};

}
