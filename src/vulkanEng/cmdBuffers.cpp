#include <sys/types.h>
#include <vulkanEng.hpp>

namespace vkEng {
    void VulkanEng::createCommandPool() 
    {
        VkCommandPoolCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .queueFamilyIndex = m_graphicsCard.queueFamily,
            .flags = 0,
        };
        
        if (vkCreateCommandPool(m_graphicsCard.logicalInstance, &info, nullptr, &m_graphicsCard.gpuCmd.pool) != VK_SUCCESS)
            throw std::runtime_error("Error creating a command pool");
    }
}