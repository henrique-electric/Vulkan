#include <vulkanEng.hpp>

namespace vkEng
{
    VulkanEng::VulkanEng(const char *appName, const char *engName) {

        // Init struct containing the info about the application
        m_appInfo.sType  = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        m_appInfo.pEngineName  = engName;
        m_appInfo.pApplicationName = appName;
        m_appInfo.apiVersion = VK_API_VERSION_1_0;
        m_appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        m_appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        m_appInfo.pNext = nullptr;

        // Get the vulkan instances required by GLFW
        const char **extensions = glfwGetRequiredInstanceExtensions(&m_extCount);

        for (int i = 0; i < m_extCount; i++)
            m_instExts.emplace_back(extensions[i]);

#ifdef __APPLE__
        requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        m_instanceInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

        // Init the structure used to create a vulkan instance
        m_instanceInfo.sType  = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        m_instanceInfo.pApplicationInfo = &m_appInfo;
        m_instanceInfo.enabledExtensionCount = m_extCount;
        m_instanceInfo.ppEnabledExtensionNames = m_instExts.data();
        m_instanceInfo.enabledLayerCount = 0;


        if (vkCreateInstance(&m_instanceInfo, nullptr, &m_vkInstance) != VK_SUCCESS) {
            throw std::runtime_error("Error to init a vulkan instance");
        }


    }
} 
