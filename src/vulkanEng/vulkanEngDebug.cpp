#include <vulkanEng.hpp>

#ifdef DEBUG
namespace vkEng {
    void VulkanEng::cleanDebugRes() {
        auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_vkInstance, "vkDestroyDebugUtilsMessengerEXT");

        vkDestroyDebugUtilsMessengerEXT(m_vkInstance, m_debugMessenger, nullptr);
        std::cout << "Destroyed the debugger mesenger\n";
    }

    VkBool32 VulkanEng::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                      VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                      void *pUserData) {

        std::cout << "Validation Layer: " << pCallbackData->pMessage << std::endl;


        return VK_FALSE;
    }

    void VulkanEng::setupDebugLayersAndExt() {
        m_instExts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // load the extension for debbugin

        //vkEnumerateInstanceLayerProperties(&m_valLayersCount, nullptr); // Get how many layers available
        std::vector<VkLayerProperties> tmpLayerVector(m_valLayersCount); // Initialize a temporary vector to store the Layers structures
        vkEnumerateInstanceLayerProperties(&m_valLayersCount, tmpLayerVector.data()); // Put the layer structs inside the vector

        //Load basic validation layers
        m_valLayers.push_back("VK_LAYER_NV_optimus");
        m_valLayers.push_back("VK_LAYER_NV_present");
        m_valLayers.push_back("VK_LAYER_LUNARG_crash_diagnostic");
       

        m_instanceInfo.enabledLayerCount = m_valLayers.size();
        m_instanceInfo.ppEnabledLayerNames = m_valLayers.data();
    }

    void VulkanEng::populateDebugMessengerStruct() {
        m_debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT; // We are using a structure to hold create info of the Debugger

        m_debugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT; // We want all the severities of messages

        m_debugMessengerInfo.pfnUserCallback = debugCallback; // The function to be used as callback

        m_debugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT; // We want all types of messages

        m_debugMessengerInfo.pUserData = nullptr; // Nothing extra

    }

    VkResult VulkanEng::setupDebugger() {
        auto createDebugMessenger = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_vkInstance, "vkCreateDebugUtilsMessengerEXT");

        if (createDebugMessenger == nullptr) {
            std::cout << "Error loading Vulkan debugger messenger creator\n";
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }

        createDebugMessenger(m_vkInstance, &m_debugMessengerInfo, nullptr, &m_debugMessenger);

        std::cout << "Created a debugger messenger instance\n";
        return VK_SUCCESS;
    }
}
#endif
