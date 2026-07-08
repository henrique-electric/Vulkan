#include <sys/types.h>
#include <vulkanEng.hpp>

namespace vkEng {
    void VulkanEng::createPipeline(VkShaderModule vert, VkShaderModule frag) {
        VkPipelineShaderStageCreateInfo vertInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .pName = "main",
            .module = vert,
        };

        VkPipelineShaderStageCreateInfo fragInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = frag,
            .pName = "main",
        };
    }
}