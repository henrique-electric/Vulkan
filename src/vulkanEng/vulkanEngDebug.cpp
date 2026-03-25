#include <vulkanEng.hpp>

#ifdef DEBUG
namespace vkEng {

    /*
		Clean up the resources used by the Vulkan debugger, because the debugger is an extension used by the instance, it is called at the end of the application.
    */
    void VulkanEng::cleanDebugRes() {
        auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_vkInstance, "vkDestroyDebugUtilsMessengerEXT");

        vkDestroyDebugUtilsMessengerEXT(m_vkInstance, m_debugMessenger, nullptr);
        std::cout << "Destroyed the debugger mesenger\n";
    }

    /*
        Callback used by the Vulkan debugger, being called always a message gets into a validation layer.
    */
    VkBool32 VulkanEng::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                      VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                      void *pUserData) {

        std::cout << "Validation Layer: " << pCallbackData->pMessage << std::endl;


        return VK_FALSE;
    }

    /*
		Setup the validation layers and instance extensions required by the application, this function is called in the constructor of the engine, and it is used to 
        load the validation layers and instance extensions required by the application, in this case, we load the VK_EXT_DEBUG_UTILS_EXTENSION_NAME extension to be 
        able to use the Vulkan debugger, and we load the validation layers provided in the parameters of the function.
    */
    void VulkanEng::setupDebugLayersAndExt(std::vector<const char*>& instanceLayers, std::vector<const char*>& gpuLayers) {
        m_instExts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // load the extension for debbugin

        //Load basic validation layers
        handleInstanceLayerLoading(instanceLayers);

        m_instanceInfo.enabledLayerCount = static_cast<uint32_t>(m_valLayers.size());
        m_instanceInfo.ppEnabledLayerNames = m_valLayers.data();
    }

    /*
		Intitialize the structure used to create the Debug Messenger, this structure is used to specify the details of the messages we want to receive from the validation layers.
    */
    void VulkanEng::populateDebugMessengerStruct() {
        m_debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT; // We are using a structure to hold create info of the Debugger

        m_debugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT; // We want all the severities of messages

        m_debugMessengerInfo.pfnUserCallback = debugCallback; // The function to be used as callback

        m_debugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT; // We want all types of messages

        m_debugMessengerInfo.pUserData = nullptr; // Nothing extra

    }
    
    /*
		Handle the loading of the validation layers, this function is used to check if the validation layers provided in the parameters are available in the current 
        system, and if they are, it loads them to be used by the instance.
    */
    void VulkanEng::handleInstanceLayerLoading(const std::vector<const char*>& layersToLoad) {
        uint32_t availableLayers = 0; // Variable to store all the available layers in the current system.
        vkEnumerateInstanceLayerProperties(&availableLayers, nullptr);  // Get How many layers are available in the current system.
        
        VkLayerProperties *currentLayersArr = new VkLayerProperties[availableLayers]; // Allocate a buffer to hold structures to each available layer.
        
        vkEnumerateInstanceLayerProperties(&availableLayers, currentLayersArr); // Load to buffer the layers available on the system.
        
        // Check if the provided layers are available to load.
        for (int i = 0; i < layersToLoad.size() && i < availableLayers; i++) {
            if (utils::strcmp(currentLayersArr[i].layerName, layersToLoad[i]) == 0) {
                printf("Unavailable layer in the current system. ----> %s\n", layersToLoad[i]);
            } else {
                m_valLayers = std::move(layersToLoad);
            }
        }

		delete[] currentLayersArr; // Clean the buffer used to hold the available layers.
    }

    /*
        Create the debugger messenger system after seting up the structure, the callback and the layers to be used by the debugger.
    */
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
