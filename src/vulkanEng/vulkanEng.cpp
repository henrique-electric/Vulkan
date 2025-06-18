#include <vulkanEng.hpp>

namespace vkEng {
     void VulkanEng::listAvailableExtensions() {
        uint32_t extCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);

        std::vector<VkExtensionProperties> extensions(extCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, extensions.data());

        for (auto&[extensionName, specVersion]: extensions) {
            std::cout << "Extension: " << extensionName << std::endl;
        }
    }

    void VulkanEng::listAvailableLayers() {
        uint32_t layersCount = 0;
        vkEnumerateInstanceLayerProperties(&layersCount, nullptr);

        std::vector<VkLayerProperties> layers(layersCount);
        vkEnumerateInstanceLayerProperties(&layersCount, layers.data());

        for (auto& layer : layers) {
            std::cout << "Layer Name = " << layer.layerName << std::endl;
        }

    }

    void VulkanEng::setupApplicationInfo(const char *appName, const char *engName) {
         m_appInfo.sType  = VK_STRUCTURE_TYPE_APPLICATION_INFO;
         m_appInfo.pEngineName  = engName;
         m_appInfo.pApplicationName = appName;
         m_appInfo.apiVersion = VK_API_VERSION_1_0;
         m_appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
         m_appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
         m_appInfo.pNext = nullptr;
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
        listAvailableLayers();
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

    }
} 
