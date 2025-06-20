#pragma once
#include <vulkan/vulkan.hpp>

namespace vkEng {
    struct gpuDevice {
        VkPhysicalDevice device;
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures features;
        std::vector<VkQueueFamilyProperties> queueProperties;
        VkDevice logicalInstance;
    };

    struct VkDeviceQueueCreateInfoMod {
        VkDeviceQueueCreateInfo queueInfo;
        float queuePriority;
    };
}
