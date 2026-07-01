#include <sys/types.h>
#include <vulkanEng.hpp>

namespace vkEng {
    // +=--------------------------------------------------------------------------
    void VulkanEng::pickChainExtent(swapChainFrameDimensions& dimensionsStruct) {

    }
    // --------------------------------------------------------------------------

    // +=--------------------------------------------------------------------------
    /*
        Function used to initialize the swap chain, this is done by getting the properties of the swap chain support of the
        graphics card, picking the best surface format and presentation mode, and then filling the structure used to create
        the swap chain with this info
    */
    void VulkanEng::initSwapChain() {
        SwapChainProperties chainProperties;

        VkSwapchainCreateInfoKHR chainInfo = {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .presentMode = this->pickSwapPresentMode(chainProperties.presentationModes),
            .surface = this->m_vulkanSurface,
        };
    }
    // --------------------------------------------------------------------------

    // +=--------------------------------------------------------------------------
    /*
        Make sure that the graphics card has support for the swap chain, this is done by checking if
        the card has support for the surface and if it has the necessary properties to create a swap chain
    */
    void VulkanEng::validateCardSwapChain() {
        vkEng::SwapChainProperties properties{};

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_graphicsCard.device, m_vulkanSurface,
            &properties.capabilities);

        uint32_t surfaceFormatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_graphicsCard.device, m_vulkanSurface, &surfaceFormatCount, nullptr);

        uint32_t presentationModesCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_graphicsCard.device, m_vulkanSurface, &presentationModesCount, nullptr);

        vkGetPhysicalDeviceSurfacePresentModesKHR(m_graphicsCard.device, m_vulkanSurface, &presentationModesCount, properties.presentationModes.data());

        if (properties.presentationModes.empty() || properties.formats.empty())
            std::runtime_error("Surface has not the necessary properties to use swap chain");

    }
    // --------------------------------------------------------------------------

    // +=--------------------------------------------------------------------------
    /*
        Handle the SwapChain format picking, this is done by checking if the desired format is
        available, if not, we just pick the first one available
    */
    VkSurfaceFormatKHR VulkanEng::pickSwapFormat(const std::vector<VkSurfaceFormatKHR>& formats) {

        if (formats.size() == 0)
            std::runtime_error("No surface formats found for the swap chain");

        // Pick the best desired surface format if available
        for (auto& format : formats) {
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return format;
        }

        return formats[0]; // Pick the first one if not found the desired one
    }
    // --------------------------------------------------------------------------

    // +=--------------------------------------------------------------------------
    VkPresentModeKHR VulkanEng::pickSwapPresentMode(const std::vector<VkPresentModeKHR>& modes) {
        if (modes.size() == 0)
            std::runtime_error("No presentation modes found for the swap chain");

        for (auto& mode : modes) {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
                return mode; // Pick the mailbox present mode if available, this mode does make the application to wait if the buffers are full

        }

        return VK_PRESENT_MODE_FIFO_KHR; // Pick the classic dual buffer image presentation
    }
    // --------------------------------------------------------------------------


}