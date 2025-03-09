/*
render/vulkan.cpp - parentless

Implements render/vulkan.hpp

*/

#include <moss/meta/vulkanLibs.hpp>
#include <moss/render/vulkan.hpp>
#include <fstream>


void moss::render::Vulkan::init() {
    readConfig();
    initWindow();
}

void moss::render::Vulkan::readConfig() {
    config = json::parse(std::ifstream("data/vulkanConfig.hpp"));
}

void moss::render::Vulkan::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(config["width"].get<glm::f32>(), config["height"].get<glm::f32>(), config["title"].get<std::string>().data(), nullptr, nullptr);
}
