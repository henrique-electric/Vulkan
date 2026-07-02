#include <sys/types.h>
#include <vulkanEng.hpp>

namespace vkEng {

    /*
      Choose a queue from the listing ones and setup the structure used to create the logical device with the queue info, this is done
      by searching for a queue family that has support for graphics and compute commands, this
      way we can use the same queue for both types of commands, but more queues can be added in the future if needed
    */
    void VulkanEng::setupQueues(VkDeviceQueueCreateInfoMod& queueCreationInfo, gpuDevice& card) {

        // Run throught the queue families searching for a queue that uses all these 3 command processing
        for (size_t familyIndex = 0; familyIndex < card.queueProperties.size(); familyIndex++) {
            if ((card.queueProperties[familyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT) && (card.queueProperties[familyIndex].queueFlags & VK_QUEUE_COMPUTE_BIT)) {
                card.queueFamily = static_cast<uint8_t>(familyIndex); // get the index of the family

                queueCreationInfo.queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueCreationInfo.queueInfo.queueFamilyIndex = familyIndex;
                queueCreationInfo.queueInfo.queueCount = 1;
                queueCreationInfo.queuePriority = 1.0f;
                queueCreationInfo.queueInfo.pQueuePriorities = &queueCreationInfo.queuePriority;
                return;
            }
        }

        throw std::runtime_error("No required queue family found");
    }
}