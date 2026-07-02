#include <sys/types.h>
#include <vulkanEng.hpp>

namespace vkEng {


    /*
    Analyse all the gpus found searching for a discrete GPU, if none is found, we use the integrated GPU
    */
    int VulkanEng::analyzeGpu(std::vector<gpuDevice>& physicalDevices) {

        if (physicalDevices.size() == 0)
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
    void VulkanEng::getGpuVector(std::vector<gpuDevice>& array) {

        // Use vulkan API to list all the devices available on the system, and get their properties and features
        uint32_t availableCards = 0;
        vkEnumeratePhysicalDevices(m_vkInstance, &availableCards, nullptr);

        if (availableCards == 0)
            std::runtime_error("No GPU found in the system with vulkan support");

        VkPhysicalDevice* cardsFound = new VkPhysicalDevice[availableCards];
        VkPhysicalDeviceProperties* cardsPropeties = new VkPhysicalDeviceProperties[availableCards];
        VkPhysicalDeviceFeatures* cardsFeatures = new VkPhysicalDeviceFeatures[availableCards];
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

}