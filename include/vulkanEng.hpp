#pragma once
#define DEBUG
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <vector>
#include <iostream>

namespace vkEng
{
    class VulkanEng
    {
    protected:
        VkInstance m_vkInstance{}; // Stores the vulkan instance itself

#ifdef DEBUG
        VkDebugUtilsMessengerEXT m_debugMessenger{};
        void cleanDebugRes();
#endif


    private:
        VkApplicationInfo m_appInfo{}; // store basic info about the vulkan application
        VkInstanceCreateInfo m_instanceInfo{}; // Stores info used to create a vulkan instance

        uint32_t m_extCount{}; // number of extensions
        std::vector<const char*> m_instExts{}; // Extension list

#ifdef DEBUG
        uint32_t m_valLayersCount{};
        std::vector<const char*> m_valLayers{};

        VkDebugUtilsMessengerCreateInfoEXT m_debugMessengerInfo{};

        // Debug Functions
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                            VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

        VkResult setupDebugger();
#endif

        // Functions
        static void listAvailableExtensions();
        static void listAvailableLayers();

    public:
        VulkanEng(const char *appName, const char *engName);
    };
    
}
