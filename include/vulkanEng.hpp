#pragma once
#define DEBUG

// Third Party includes
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

// std includes
#include <cstdint>
#include <vector>
#include <iostream>

// Project includes
#include <utils.hpp>
#include <types.hpp>

namespace vkEng
{
    enum windowHandler {
        GLFW,
        SDL
    };

    class VulkanEng
    {
    protected:
        VkInstance m_vkInstance{}; // Stores the vulkan instance itself
        VkSurfaceKHR m_vulkanSurface{};
        gpuDevice  m_graphicsCard{};
        
        
        // Protected functions
        void setupWindowSurface(GLFWwindow *window);
        inline VkDevice getEngineLogicalDevice(void) { return this->m_graphicsCard.logicalInstance;};
        inline VkInstance getEngineVulkanInstance(void) { return this->m_vkInstance;};
        inline VkSurfaceKHR getEngineVulkanSurface(void) { return this->m_vulkanSurface;};
        inline VkQueue getEngineQueueHandler(void) { return this->m_graphicsCard.queueInterface;};
        inline windowHandler getEngineWindowHandler(void) { return this->winHandler; };
        
        

#ifdef DEBUG
        VkDebugUtilsMessengerEXT m_debugMessenger{};
        void cleanDebugRes();
#endif


    private:
        windowHandler winHandler; // Which framework to use, SDL or GLFW

        VkApplicationInfo m_appInfo{}; // store basic info about the vulkan application
        VkInstanceCreateInfo m_instanceInfo{}; // Stores info used to create a vulkan instance


        uint32_t m_extCount{}; // number of extensions
        std::vector<const char*> m_instExts{}; // Extension list
        

#ifdef DEBUG
        uint32_t m_valLayersCount{0};
        std::vector<const char*> m_valLayers{};
        std::vector<const char*> m_gpuValLayers{};

        VkDebugUtilsMessengerCreateInfoEXT m_debugMessengerInfo{};

        // Debug Functions
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                            VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

        void setupDebugLayersAndExt(std::vector<const char*>& instanceLayers, std::vector<const char*>& gpuLayers);
        void handleInstanceLayerLoading(const std::vector<const char*>& layersToLoad);
        void populateDebugMessengerStruct();
        VkResult setupDebugger();
#endif

        // Functions
        int  analyzeGpu(std::vector<gpuDevice>& physicalDevices);
        void getGpuVector(std::vector<gpuDevice>& array);
        void setupLogicalDevice();
        void setupQueues(VkDeviceQueueCreateInfoMod& queueCreationInfo, gpuDevice& card);
        void setupApplicationInfo(const char *appName, const char *engName);
        void setupGraphicsCard();
        void validateCardExtensions(vkEng::gpuDevice& card);
        void validateCardSwapChain();
        void initSwapChain();
        VkExtent2D pickChainExtent();
        VkSurfaceFormatKHR pickSwapFormat();
        VkPresentModeKHR pickSwapPresentMode(const std::vector<VkPresentModeKHR> &modes);

    public:
        VulkanEng(const char *appName, const char *engName, windowHandler windowHandler = windowHandler::GLFW);
    };
    
}
