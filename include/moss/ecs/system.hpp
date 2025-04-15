/**
 * @file    core/system.hpp
 * @brief   Defines the base `System` class for ECS systems.
 *
 * The `System` class serves as the base class for all systems in the ECS
 * architecture. It provides virtual methods for system lifecycle management,
 * including `init`, `build`, `tick`, and `exit`. Derived classes can override
 * these methods to implement specific system logic.
 *
 * The class supports permission-controlled access to the ECS registry via
 * the `Key` class, allowing fine-grained control over read and write access
 * to entities.
 *
 * Systems can interact with entities using a `DynamicView`, offering flexibility
 * in which entities a system affects.
 */

#pragma once

#include <moss/core/key.hpp>
#include <moss/commands/primitives.hpp>

namespace moss {

/**
 * @class System
 * @brief Base class for ECS systems.
 * 
 * The `System` class is the fundamental base class for systems within the ECS
 * framework. It provides default methods for `init`, `build`, `tick`, and `exit`,
 * which can be overridden by derived classes to implement system behavior.
 * 
 * The class also manages access permissions to the ECS registry through the `Key`
 * class, supporting both read and write access.
 */
class System {
public:
    virtual ~System() = default; 

    virtual void init()  { }
    virtual void build() { }
    virtual void tick()  { }
    virtual void exit()  { }

    virtual void build(const Key<key::WRITE>& key) { }
    virtual void tick(const Key<key::READ>& key)  { }
    virtual void exit(const Key<key::WRITE>& key)  { }

    virtual void build(const Key<key::WRITE>& key, const DynamicView& entities) { }
    virtual void tick(const Key<key::READ>& key, const DynamicView& entities)  { }
    virtual void exit(const Key<key::WRITE>& key, const DynamicView& entities)  { }
};

}
