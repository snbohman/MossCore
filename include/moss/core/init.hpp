#pragma once

#include <moss/includes.hpp>
#include <moss/components/components.hpp>


#define FILL_COMPONENT_DATA(component) { \
    #component, \
    [&registry](entt::entity entity, const json& data) { \
        registry.emplace<component>(entity, data.get<component>()); \
    } \
}

#define REGISTER_COMPONENT(component) componentRegistry[#component] = \
    [&registry](entt::entity entity, const json& data) { \
        registry.emplace<component>(entity, data.get<component>()); \
    }

#define REGISTER_TAG(component) componentRegistry[#component] = \
    [&registry](entt::entity entity, const json& data) { \
        registry.emplace<component>(entity); \
    }


namespace moss {

using ComponentRegistry = std::unordered_map<std::string, std::function<void(entt::entity, const json&)>>;

void initRaylib(const json& gameConfig);
void initEntt(const json& gameConfig, entt::registry& registry, const ComponentRegistry& componentMap = {0, 0});
void buildComponentRegistry(entt::registry& registry, ComponentRegistry& componentMap);


} // moss
