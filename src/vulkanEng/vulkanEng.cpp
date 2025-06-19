#include <vulkanEng.hpp>

namespace vkEng {

    void VulkanEng::setupApplicationInfo(const char *appName, const char *engName) {
         m_appInfo.sType  = VK_STRUCTURE_TYPE_APPLICATION_INFO;
         m_appInfo.pEngineName  = engName;
         m_appInfo.pApplicationName = appName;
         m_appInfo.apiVersion = VK_API_VERSION_1_0;
         m_appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
         m_appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
         m_appInfo.pNext = nullptr;
    }
    
    int VulkanEng::analyzeGpu(std::vector<gpuDevice>& physicalDevices) {
        
        // Run throught an array with all the GPUs
        for (int i = 0; i < physicalDevices.size(); i++) {
            if (physicalDevices[i].properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                m_graphicsCard = std::move(physicalDevices[i]); // Pick the card if it is a discrete GPU
                return i; // Return the index of the GPU in the array
            }
        }
        
        m_graphicsCard = std::move(physicalDevices[0]); // There is no discrete GPU, pick the first one found
        return 0;
    }

    void VulkanEng::getGpuVector(std::vector<gpuDevice> &array) {
        uint32_t availableCards = 0;
        vkEnumeratePhysicalDevices(m_vkInstance, &availableCards, nullptr);
        
        VkPhysicalDevice cardsFound[availableCards];
        VkPhysicalDeviceProperties cardsPropeties[availableCards];
        VkPhysicalDeviceFeatures cardsFeatures[availableCards];
        
        vkEnumeratePhysicalDevices(m_vkInstance, &availableCards, cardsFound);
        
        gpuDevice newCardStruct{};

        uint32_t cardQueueFamilyCount = 0;
        
        for (int i = 0; i < availableCards; i++) {

            vkGetPhysicalDeviceProperties(cardsFound[i], &cardsPropeties[i]);
            vkGetPhysicalDeviceFeatures(cardsFound[i], &cardsFeatures[i]);
            
            vkGetPhysicalDeviceQueueFamilyProperties(cardsFound[i], &cardQueueFamilyCount, nullptr);
            
            std::vector<VkQueueFamilyProperties> cardQueueProperties(cardQueueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(cardsFound[i], &cardQueueFamilyCount, cardQueueProperties.data());


            newCardStruct.device = cardsFound[i];
            newCardStruct.properties = cardsPropeties[i];
            newCardStruct.features = cardsFeatures[i];
            newCardStruct.queueProperties = std::move(cardQueueProperties);
            array.push_back(newCardStruct);
        }
    }

    void VulkanEng::setupGraphicsCard() {
        std::vector<gpuDevice> gpus;
        getGpuVector(gpus);
        utils::printCardsDetails(gpus);
    }

    VulkanEng::VulkanEng(const char *appName, const char *engName) {

       setupApplicationInfo(appName, engName); // Init struct containing the info about the application


        // Get the vulkan instances required by GLFW
        const char **extensions = glfwGetRequiredInstanceExtensions(&m_extCount);

        for (int i = 0; i < m_extCount; i++)
            m_instExts.emplace_back(std::move(extensions[i]));

#ifdef __APPLE__
        m_instExts.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        m_instanceInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

        // Init the structure used to create a vulkan instance
        m_instanceInfo.sType  = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        m_instanceInfo.pApplicationInfo = &m_appInfo;


// Load all validation layers and Instance extensions if in debug enviroment
#ifdef DEBUG
        setupDebugLayersAndExt();
        populateDebugMessengerStruct();

        m_instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &m_debugMessengerInfo;
#else
         m_instanceInfo.enabledLayerCount = 0;

#endif

         // Finally add the extensions list the extensions to be used by the instance
         m_instanceInfo.enabledExtensionCount = static_cast<uint32_t>(m_instExts.size());
         m_instanceInfo.ppEnabledExtensionNames = m_instExts.data();

        if (vkCreateInstance(&m_instanceInfo, nullptr, &m_vkInstance) != VK_SUCCESS) {
            throw std::runtime_error("Error to init a vulkan instance");
        }
        
// Setup the messenger debugger if in debug enviroment
#ifdef DEBUG
         setupDebugger();
#endif
        setupGraphicsCard();
    }
}
