#include <sys/types.h>
#include <vulkanEng.hpp>

namespace vkEng {

    // setup basic info of the vulkan application
    void VulkanEng::setupApplicationInfo(const char *appName, const char *engName) {
         m_appInfo.sType  = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		 m_appInfo.pEngineName = engName;                         // Name used by the engine (Not mandatory).
		 m_appInfo.pApplicationName = appName;                    // Name used by the application (Not mandatory).
         m_appInfo.apiVersion = VK_API_VERSION_1_0;               // Version of the vulkan API to be used, used to check if the current system vulkan API matches the requirement of this application to run.
         m_appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // Version of the application, this is used for informational purposes, so it's not mandatory.
		 m_appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);      // Version of the engine, also used for informational purposes, so it's also not mandatory.
         m_appInfo.pNext = nullptr;                               // Pointer to a structure with more infos for the application and to use with some extensions, also not mandatory, but it's a good practice to set it to nullptr.
    }

    /*
        Analyse all the gpus found searching for a discrete GPU, if none is found, we use the integrated GPU
    */
    int VulkanEng::analyzeGpu(std::vector<gpuDevice>& physicalDevices) {

        if(physicalDevices.size() == 0)
			std::runtime_error("No GPU found in the system with vulkan support");

        // Run throught an array with all the GPUs
        for (size_t i = 0; i < physicalDevices.size(); i++) {
            if (physicalDevices[i].properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                m_graphicsCard = std::move(physicalDevices[i]); // Pick the card if it is a discrete GPU
                return i; // Return the index of the GPU in the array
            }
        }

        m_graphicsCard = std::move(physicalDevices[0]); // There is no discrete GPU, pick the first one found
        return 0;
    }


    /*
        Get a vector with all GPUs detected on the system
     */
    void VulkanEng::getGpuVector(std::vector<gpuDevice> &array) {

		// Use vulkan API to list all the devices available on the system, and get their properties and features
        uint32_t availableCards = 0;
        vkEnumeratePhysicalDevices(m_vkInstance, &availableCards, nullptr);

		if (availableCards == 0)
            std::runtime_error("No GPU found in the system with vulkan support");

        VkPhysicalDevice *cardsFound = new VkPhysicalDevice[availableCards];
        VkPhysicalDeviceProperties *cardsPropeties = new VkPhysicalDeviceProperties[availableCards];
        VkPhysicalDeviceFeatures *cardsFeatures = new VkPhysicalDeviceFeatures[availableCards];
        vkEnumeratePhysicalDevices(m_vkInstance, &availableCards, cardsFound);
		//================================================================================================================


		// Run throught the array of devices found and get their properties, features and queue families, then store all this info in a gpuDevice struct and push it to the vector
        gpuDevice newCardStruct{};
        uint32_t cardQueueFamilyCount = 0;

        for (size_t i = 0; i < availableCards; i++) {

		// Get the properties and features of each physical device found by the vulkan API
            vkGetPhysicalDeviceProperties(cardsFound[i], &cardsPropeties[i]);
            vkGetPhysicalDeviceFeatures(cardsFound[i], &cardsFeatures[i]);
        

            // Get the details of queue for each physical device found
            vkGetPhysicalDeviceQueueFamilyProperties(cardsFound[i], &cardQueueFamilyCount, nullptr);
            std::vector<VkQueueFamilyProperties> cardQueueProperties(cardQueueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(cardsFound[i], &cardQueueFamilyCount, cardQueueProperties.data());


            newCardStruct.device = cardsFound[i];
            newCardStruct.properties = cardsPropeties[i];
            newCardStruct.features = cardsFeatures[i];
            newCardStruct.queueProperties = std::move(cardQueueProperties);
            array.push_back(newCardStruct);
        }
		//================================================================================================================
		delete[] cardsFound;
        delete[] cardsPropeties;
        delete[] cardsFeatures;
    }

	/*
        Function that setup the graphics card, this function should be called after creating the vulkan instance and the window surface, because it 
        needs both of these to validate the card extensions and swap chain support
    */
    void VulkanEng::setupGraphicsCard() {
        std::vector<gpuDevice> gpus;
        getGpuVector(gpus);
        analyzeGpu(gpus);
        validateCardExtensions(m_graphicsCard);
        setupLogicalDevice();
        utils::printCardDetails(m_graphicsCard);
        utils::listCardAvailableExt(m_graphicsCard);
    }

    /*
		Function to validate if the graphics card has support for the extensions needed by the application, in this case, we just need the swapchain extension, but 
        more extensions can be added in the future
    */
    void VulkanEng::validateCardExtensions(vkEng::gpuDevice& card) {

    //  Get how much extensions the card being used has support for and all the details of these extensiosn
        uint32_t deviceExtCount = 0;
        vkEnumerateDeviceExtensionProperties(m_graphicsCard.device, nullptr,
                                            &deviceExtCount, nullptr);

        VkExtensionProperties *deviceExtensions = new VkExtensionProperties[deviceExtCount];
        vkEnumerateDeviceExtensionProperties(m_graphicsCard.device, nullptr, &deviceExtCount,
                                            deviceExtensions);
		//=================================================================================================

        bool foundSwapChain = false;

        for (size_t count = 0; count < deviceExtCount; count++) {
            if (strcmp(deviceExtensions[count].extensionName, "VK_KHR_swapchain") == 0)
				foundSwapChain = true;
        }

        if (!foundSwapChain == false)
            std::runtime_error("Vulkan swapchain not available");

		delete[] deviceExtensions;
    }

    /*
		Make sure that the graphics card has support for the swap chain, this is done by checking if
        the card has support for the surface and if it has the necessary properties to create a swap chain
    */
    void VulkanEng::validateCardSwapChain() {
      vkEng::SwapChainProperties properties{};

      vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_graphicsCard.device, m_vulkanSurface,
                                                &properties.capabilities);

      uint32_t surfaceFormatCount = 0;
      vkGetPhysicalDeviceSurfaceFormatsKHR(m_graphicsCard.device, m_vulkanSurface, &surfaceFormatCount, nullptr );
      
      uint32_t presentationModesCount = 0;
      vkGetPhysicalDeviceSurfacePresentModesKHR(m_graphicsCard.device, m_vulkanSurface, &presentationModesCount, nullptr);

      vkGetPhysicalDeviceSurfacePresentModesKHR(m_graphicsCard.device, m_vulkanSurface,  &presentationModesCount, properties.presentationModes.data());

      if (properties.presentationModes.empty() || properties.formats.empty())
          std::runtime_error("Surface has not the necessary properties to use swap chain");
      
    }

    /*
		Handle the SwapChain format picking, this is done by checking if the desired format is
        available, if not, we just pick the first one available
    */
    VkSurfaceFormatKHR VulkanEng::pickSwapFormat(const std::vector<VkSurfaceFormatKHR> &formats) {

        if (formats.size() == 0)
            std::runtime_error("No surface formats found for the swap chain");

        // Pick the best desired surface format if available
        for (auto& format : formats) {
           if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
              return format;
        }

        return formats[0]; // Pick the first one if not found the desired one
    }


    VkPresentModeKHR VulkanEng::pickSwapPresentMode(const std::vector<VkPresentModeKHR> &modes) {
        if (modes.size() == 0)
			std::runtime_error("No presentation modes found for the swap chain");
        
        for(auto& mode : modes) {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
                return mode; // Pick the mailbox present mode if available, this mode does make the application to wait if the buffers are full

        }

      return VK_PRESENT_MODE_FIFO_KHR; // Pick the classic dual buffer image presentation
    }

    /*
		The function that holds the whole core logic to create a logical device, this is done by filling the structure with the queues info, the extensions 
        to be used and the features of the card, then we create the logical device and get the queue interface
    */
    void VulkanEng::setupLogicalDevice() {
        VkDeviceQueueCreateInfoMod logicalDeviceQueue{};
        VkDeviceCreateInfo logicalDeviceInfo{};
        setupQueues(logicalDeviceQueue, m_graphicsCard);

        logicalDeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        const char* extensions[] = {"VK_KHR_swapchain"}; // Load a simple device extension
        logicalDeviceInfo.ppEnabledExtensionNames = extensions;
        logicalDeviceInfo.enabledExtensionCount = 1;

        /*
            TODO
            setup debug layers
        */
        logicalDeviceInfo.enabledLayerCount = 0;
        logicalDeviceInfo.ppEnabledLayerNames = nullptr;
        //

        logicalDeviceInfo.pEnabledFeatures = &m_graphicsCard.features;
        logicalDeviceInfo.pQueueCreateInfos = &logicalDeviceQueue.queueInfo;
        logicalDeviceInfo.queueCreateInfoCount = 1; // just one queue family for now

        if (vkCreateDevice(m_graphicsCard.device, &logicalDeviceInfo, nullptr, &m_graphicsCard.logicalInstance) != VK_SUCCESS)
            throw std::runtime_error("Error creating a logical device");

        vkGetDeviceQueue(m_graphicsCard.logicalInstance, m_graphicsCard.queueFamily, 0, &m_graphicsCard.queueInterface);
        std::cout << "Got the queue interface\n";
    }

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

    /*
		Create a Vulkan surface for the window using the GLFW library, this is needed to be able to present the rendered images to the window, this function should be 
        called after creating the vulkan instance and before setting up the graphics card, because we need the surface to validate the swap chain support of the card
    */
    void VulkanEng::setupWindowSurface(GLFWwindow *window) {
        if (glfwCreateWindowSurface(m_vkInstance, window, nullptr, &m_vulkanSurface) != VK_SUCCESS) {
          std::runtime_error("Failed to create the vulkan window surface");
        }

        std::cout << "Created the Vulkan window surface\n";
    }

    void VulkanEng::pickChainExtent(swapChainFrameDimensions &dimensionsStruct) {
        
    }

    /*
		Function used to initialize the swap chain, this is done by getting the properties of the swap chain support of the 
        graphics card, picking the best surface format and presentation mode, and then filling the structure used to create 
        the swap chain with this info
    */
    void VulkanEng::initSwapChain() {
        SwapChainProperties chainProperties;
        
        VkSwapchainCreateInfoKHR chainInfo = {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .presentMode =  this->pickSwapPresentMode(chainProperties.presentationModes),
            .surface = this->m_vulkanSurface,
        };
    }

    /*
		The constructor of the vulkan engine, this is where we create the vulkan instance, setup the application info and load the extensions required by GLFW, then we setup 
        the graphics card and the logical device
    */
    VulkanEng::VulkanEng(const char *appName, const char *engName) {

       setupApplicationInfo(appName, engName); // Init struct containing the info about the application

        // Get the vulkan instances required by GLFW
        const char **extensions = glfwGetRequiredInstanceExtensions(&m_extCount);

        for (size_t i = 0; i < m_extCount; i++)
            m_instExts.emplace_back(std::move(extensions[i]));
            

       /*
		  Add intace extensions required by the application for MacOS in this case, we need the VK_KHR_portability_enumeration extension to be able to enumerate the 
          physical devices on the system, and the VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR flag to be able to use this extension, this is needed because 
          MacOS does not have native support for vulkan and uses a translation layer called MoltenVK to provide vulkan support, and this 
          translation layer does not support all the features of vulkan, so we need to use this extension to be able to enumerate the physical devices on the system
       */
#ifdef __APPLE__
        m_instExts.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        m_instanceInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

        // Init the structure used to create a vulkan instance
        m_instanceInfo.sType  = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        m_instanceInfo.pApplicationInfo = &m_appInfo;


// Load all validation layers and Instance extensions if in debug enviroment
#ifdef DEBUG
        std::vector<const char*> instanceLayers;
        std::vector<const char*> gpuLayers;
        instanceLayers.push_back("hi");
        gpuLayers.push_back("test");

        setupDebugLayersAndExt(instanceLayers, gpuLayers);
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
