#pragma once

#include <moss/core/key.hpp>
#include <moss/commands/primitives.hpp>

namespace moss {

class System {
public:
    virtual ~System() = default; 

    virtual void init()  { }
    virtual void build() { }
    virtual void tick()  { }
    virtual void exit()  { }

    virtual void build(Key<key::WRITE>& key) { }
    virtual void tick(Key<key::READ>& key)  { }
    virtual void exit(Key<key::WRITE>& key)  { }

    virtual void build(const Key<key::WRITE>& key, const DynamicView& entities) { }
    virtual void tick(const Key<key::READ>& key, const DynamicView& entities)  { }
    virtual void exit(const Key<key::WRITE>& key, const DynamicView& entities)  { }
};

}
