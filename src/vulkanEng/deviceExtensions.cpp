#include <sys/types.h>
#include <vulkanEng.hpp>

namespace vkEng {

    /*
        Function to validate if the graphics card has support for the extensions needed by the application, in this case, we just need the swapchain extension, but
        more extensions can be added in the future
    */
    void VulkanEng::validateCardExtensions(vkEng::gpuDevice& card) {

        //  Get how much extensions the card being used has support for and all the details of these extensiosn
        uint32_t deviceExtCount = 0;
        vkEnumerateDeviceExtensionProperties(m_graphicsCard.device, nullptr,
            &deviceExtCount, nullptr);

        VkExtensionProperties* deviceExtensions = new VkExtensionProperties[deviceExtCount];
        vkEnumerateDeviceExtensionProperties(m_graphicsCard.device, nullptr, &deviceExtCount,
            deviceExtensions);
        //=================================================================================================

        bool foundSwapChain = false;

        for (size_t count = 0; count < deviceExtCount; count++) {
            if (strcmp(deviceExtensions[count].extensionName, "VK_KHR_swapchain") == 0)
                foundSwapChain = true;
        }

        if (foundSwapChain == false)
             throw std::runtime_error("Vulkan swapchain not available");

        delete[] deviceExtensions;
    }



}
