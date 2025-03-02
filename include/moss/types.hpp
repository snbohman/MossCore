#pragma once

#include <moss/includes.hpp>
#include <unordered_map>
#include <functional>


namespace moss {

using ComponentRegistry = std::unordered_map<std::string, std::function<void(entt::entity, const json&)>>;

}
