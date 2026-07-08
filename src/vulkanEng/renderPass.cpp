#include <sys/types.h>
#include <vulkanEng.hpp>

namespace vkEng
{
    void VulkanEng::createRenderPass() {
        VkAttachmentDescription colorAttachment = {
            .format = this->pickSwapFormat(m_graphicsCard.swapChainProps.formats).format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
        };
    }
} 
