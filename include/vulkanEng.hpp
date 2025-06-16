#pragma once
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

    private:
        VkApplicationInfo m_appInfo{}; // store basic info about the vulkan application
        VkInstanceCreateInfo m_instanceInfo{}; // Stores info used to create a vulkan instance

        uint32_t m_extCount{}; // number of extensions
        std::vector<const char*> m_instExts; // Extension list

        // Functions
        static void listAvailableExtensions();

    public:
        VulkanEng(const char *appName, const char *engName);
    };
    
}
