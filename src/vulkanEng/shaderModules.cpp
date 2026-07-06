#include <sys/types.h>
#include <vulkanEng.hpp>

namespace vkEng {

    VkShaderModule VulkanEng::createShaderModule(std::vector<char>& byteCode)
    {
        VkShaderModule shader;
        VkShaderModuleCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pCode = reinterpret_cast<uint32_t*>(byteCode.data()),
            .codeSize = byteCode.size()

        };

        if (vkCreateShaderModule(m_graphicsCard.logicalInstance, &createInfo, nullptr, &shader) != VK_SUCCESS)
            throw std::runtime_error("Error creating a shader");

        return shader;
    }
}