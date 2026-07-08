#include <sys/types.h>
#include <vulkanEng.hpp>
#include <fstream>

namespace vkEng {

    VkShaderModule VulkanEng::createShaderModule(std::string shaderPath)
    {
        std::ifstream shaderFile;
        shaderFile.open(shaderPath, std::ios::binary | std::ios::ate);

        size_t fileSize = shaderFile.tellg();
        shaderFile.seekg(0, std::ios::beg);
        std::vector<char> byteCode(fileSize);
        shaderFile.read(byteCode.data(), fileSize);

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