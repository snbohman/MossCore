#include <moss/core/mirror.hpp>
#include <moss/core/contex.hpp>

using namespace moss;


Mirror& Mirror::pushContex(entt::entity entity) {
    m_contex->m_view.push_back(entity);
    return *this;
}

Mirror& Mirror::pushContex(System& system) {
    m_contex->m_systems.push_back(system);
    return *this;
}
