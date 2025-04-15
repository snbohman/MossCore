#include <moss/core/context.hpp>
#include <moss/core/key.hpp>

using namespace moss;

void Context::build(const Key<key::WRITE>& key) {
    for (auto& s : m_systems) {
        s->build();
        s->build(key);
        s->build(key, m_view);
    }
}

void Context::tick(const Key<key::READ>& key) {
    for (auto& s : m_systems) {
        s->tick();
        s->tick(key);
        s->tick(key, m_view);
    }
}

void Context::exit(const Key<key::WRITE>& key) {
    for (auto& s : m_systems) {
        s->exit();
        s->exit(key);
        s->exit(key, m_view);
    }
}
