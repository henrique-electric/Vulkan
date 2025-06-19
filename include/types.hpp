#pragma once
#include <vulkan/vulkan.hpp>

namespace vkEng {
    struct gpuDevice {
        VkPhysicalDevice device;
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures features;
        std::vector<VkQueueFamilyProperties> queueProperties;
    };
}
