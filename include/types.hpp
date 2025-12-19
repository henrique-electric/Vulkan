#pragma once
#include <vulkan/vulkan.hpp>

namespace vkEng {
    struct gpuDevice {
        VkPhysicalDevice device;
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures features;
        std::vector<VkQueueFamilyProperties> queueProperties;
        VkDevice logicalInstance;

        uint8_t queueFamily; // In this project i'll be only using one queue, which one that has all the 3 main command processing capabilites 
        VkQueue queueInterface; // interface with the queue family being used
    };

    struct VkDeviceQueueCreateInfoMod {
        VkDeviceQueueCreateInfo queueInfo;
        float queuePriority;
    };

    struct swapChainFrameDimensions {
        uint32_t width;
        uint32_t height;
    };

    struct SwapChainProperties {
        VkSurfaceCapabilitiesKHR capabilities;   // capabilities of the surface
        std::vector<VkSurfaceFormatKHR> formats; // formats supported by the surface
        std::vector<VkPresentModeKHR> presentationModes; // Presentation mode that the surface has support to
    };
}
