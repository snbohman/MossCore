/*
render/vulkan.cpp

Implements render/vulkan.hpp

*/

#include <moss/meta/vulkanLibs.hpp>
#include <moss/render/vulkan/vulkan.hpp>
#include <moss/utils/utils.hpp>


using namespace moss::render; // for ease of use when implementing functions

              /////////////////////////////////
// ---------- //// -- VULKAN OVERWRITES -- //// ---------- //
              /////////////////////////////////
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
) {
    ERROR("[VULKAN] Validation layer: {}", pCallbackData->pMessage);
    return VK_FALSE;
}

              //////////////////////
// ---------- //// -- PUBLIC -- //// ---------- //
              //////////////////////
bool Vulkan::shouldClose() {
    return glfwWindowShouldClose(window);
}

void Vulkan::init() {
    readConfig();
    initWindow();
}

void Vulkan::tick() {
    glfwPollEvents();
}

void Vulkan::exit() {
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}


              ///////////////////////
// ---------- //// -- PRIVATE -- //// ---------- //
              ///////////////////////
void Vulkan::readConfig() {
    utils::config::readConfig(windowConfig, "data/windowConfig.json");
    utils::config::readConfig(vulkanConfig, "data/vulkanConfig.json");
}

void Vulkan::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(
        windowConfig["dimensions"]["width"].get<glm::u32>(),
        windowConfig["dimensions"]["height"].get<glm::u32>(),
        windowConfig["title"].get<std::string>().c_str(),
        nullptr, nullptr
    );
}

// -- TODO: Enable version management -- //
void Vulkan::createInstance() {
    if (enableValidationLayers) checkValidationLayerSupport();

    VkApplicationInfo appInfo { };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = windowConfig["title"].get<std::string>().c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    auto extensions = getRequiredExtensions();

    VkInstanceCreateInfo createInfo { };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<glm::u32>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<glm::u32>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    ERROR_IF(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS, "Failed to create vulkan instance {}", 1);
}

void Vulkan::checkValidationLayerSupport() {
    glm::u32 layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const auto& layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperites : availableLayers) {
            if (strcmp(layerName, layerProperites.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        ERROR_IF(!layerFound, "Requested validation layer \"{}\" not available", layerName);
    }
}

std::vector<const char*> Vulkan::getRequiredExtensions() {
    glm::u32 glfwExtensionCount = 0; const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void Vulkan::initDebugMessenger() {
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;
}
