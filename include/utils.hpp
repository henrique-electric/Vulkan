#pragma once
#include <vulkan/vulkan.hpp>
#include <types.hpp>


// std includes
#include <iostream>


#define LIST_FAMILY_COMMAND_SUPPORT(family) if (family.queueFlags & VK_QUEUE_COMPUTE_BIT) \
                                                printf("Family has queues with computing commands support\n"); \
                                             if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) \
                                                printf("Family has queues with graphic commands support\n"); \
                                             if (family.queueFlags & VK_QUEUE_TRANSFER_BIT) \
                                                printf("Family has queues with memory transfering\n"); \
                                             if (family.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) \
                                                printf("Family has queues with sparsing memory binding support\n");
namespace utils {

    bool strcmp(const char* str1, const char* str2);
    void listAvailableExtensions();
    void listAvailableLayers();
    void listAvailableDevices(VkInstance& instance);
    void printCardsDetails(std::vector<vkEng::gpuDevice>& cardArray);
    void listCardAvailableExt(vkEng::gpuDevice& card);
    void listDeviceFeatures(const vkEng::gpuDevice& card);
}
