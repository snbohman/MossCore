/*
render/vulkan.hpp

Main render system/class for the vulkan API. This together with raylib
are the only rendering alternatives. Inside the moss::render namespace.

    Create a VkInstance
    Select a supported graphics card (VkPhysicalDevice)
    Create a VkDevice and VkQueue for drawing and presentation
    Create a window, window surface and swap chain
    Wrap the swap chain images into VkImageView
    Create a render pass that specifies the render targets and usage
    Create framebuffers for the render pass
    Set up the graphics pipeline
    Allocate and record a command buffer with the draw commands for every possible swap chain image
    Draw frames by acquiring images, submitting the right draw command buffer and returning the images back to the swap chain

*/

#pragma once

#include <moss/meta/vulkanLibs.hpp>
#include <moss/meta/libs.hpp>
#include <moss/render/render.hpp>


namespace moss::render {

class Vulkan : Renderer {
public:
    void init() override;
    void tick() override;
    void exit() override;
    bool shouldClose() override;

private:
    void readConfig();
    void initWindow();
    void createInstance();
        void checkValidationLayerSupport();
        std::vector<const char*> getRequiredExtensions();
    void initDebugMessenger();

    // -- Moss -- //
    json windowConfig;
    json vulkanConfig;

    // -- GLFW -- //
    GLFWwindow* window;
    
    // -- Vulkan -- //
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    const std::array<const char*, 1> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

};


} // moss::render
