#pragma once
#include <window.hpp>
#include <vulkan/vulkan.hpp>
#include <cstdint>
#include <vector>

namespace vkEng
{
    class VulkanEng
    {
    private:
        VkApplicationInfo m_appInfo; // store basic info about the vulkan application
        VkInstanceCreateInfo m_instanceInfo; // Stores info used to create a vulkan instance
        VkInstance m_vkInstance; // Stores the vulkan instance itself
        
        uint32_t m_extCount; // number of extensions
        std::vector<const char*> m_instExts; // Extension list

    public:
        VulkanEng(const char *appName, const char *engName);
    };
    
}
