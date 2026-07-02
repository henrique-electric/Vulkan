#include <sys/types.h>
#include <vulkanEng.hpp>

namespace vkEng {
    /*
      The function that holds the whole core logic to create a logical device, this is done by filling the structure with the queues info, the extensions
      to be used and the features of the card, then we create the logical device and get the queue interface
  */
    void VulkanEng::setupLogicalDevice(const std::vector<const char*>* additionalExtensions) {

        VkDeviceQueueCreateInfoMod logicalDeviceQueue{};
        VkDeviceCreateInfo logicalDeviceInfo{};
        setupQueues(logicalDeviceQueue, m_graphicsCard);

        logicalDeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        std::vector<const char*> extensions;

        // Default extensions to be loaded
        extensions.push_back("VK_KHR_swapchain");
        //=================================

#ifdef DEBUG
        if (additionalExtensions == nullptr) {
            log("No additional extensions were provided\n");
        }
        else {
            for (auto& extension : *additionalExtensions) {
                std::cout << "Additional extension ---> " << extension << " provided\n";
            }
        }
#endif

        logicalDeviceInfo.ppEnabledExtensionNames = extensions.data();
        logicalDeviceInfo.enabledExtensionCount = 1;

        /*
            TODO
            setup debug layers
        */
        logicalDeviceInfo.enabledLayerCount = 0;
        logicalDeviceInfo.ppEnabledLayerNames = nullptr;
        //

        logicalDeviceInfo.pEnabledFeatures = &m_graphicsCard.features;
        logicalDeviceInfo.pQueueCreateInfos = &logicalDeviceQueue.queueInfo;
        logicalDeviceInfo.queueCreateInfoCount = 1; // just one queue family for now

        if (vkCreateDevice(m_graphicsCard.device, &logicalDeviceInfo, nullptr, &m_graphicsCard.logicalInstance) != VK_SUCCESS)
            std::runtime_error("Error creating a logical device");

        vkGetDeviceQueue(m_graphicsCard.logicalInstance, m_graphicsCard.queueFamily, 0, &m_graphicsCard.queueInterface);

#ifdef DEBUG
        log("Got the queue interface\n");
#endif

    }
}