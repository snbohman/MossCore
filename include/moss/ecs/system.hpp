/**
 * @file    ecs/system.hpp
 * @brief   System: the base class of all logic.
 *
 * Each phase (build, tick, exit) has three overloads; override the one you need:
 *
 *     build() / tick() / exit()                          no access to the registry
 *     build(key) / tick(key) / exit(key)                 Key<WRITE> for build and exit, Key<READ> for tick
 *     build(key, entities) / tick(key, entities) / ...   same, plus the entities of the
 *                                                         Context the system is connected to
 *
 * Systems are default-constructed by Mirror::connect and run in connection
 * order for build and tick, in REVERSE order for exit. There is no init() hook:
 * set up in build, use the constructor for plain members.
 */

#pragma once

#include <moss/core/key.hpp>
#include <moss/commands/primitives.hpp>

namespace moss {

class System {
public:
    virtual ~System() = default;

    virtual void build() { }
    virtual void tick()  { }
    virtual void exit()  { }

    virtual void build(const Key<key::WRITE>&) { }
    virtual void tick(const Key<key::READ>&)   { }
    virtual void exit(const Key<key::WRITE>&)  { }

    virtual void build(const Key<key::WRITE>&, const DynamicView&) { }
    virtual void tick(const Key<key::READ>&, const DynamicView&)   { }
    virtual void exit(const Key<key::WRITE>&, const DynamicView&)  { }
};

}
