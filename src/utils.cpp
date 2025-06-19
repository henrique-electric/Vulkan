#include <utils.hpp>

namespace utils {
    bool strcmp(const char* str1, const char* str2) {
        int count = 0;
        
        if (str1 != nullptr && str2 != nullptr) {
            while (str1[count] != 0x00 && str2[count] != 0x00) {
                if (str1[count] != str2[count]) {
                    return false;
                }
                
                count++;
            }
            return true;
        }
        return false;
    }

    void listAvailableExtensions() {
       uint32_t extCount = 0;
       vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);

       std::vector<VkExtensionProperties> extensions(extCount);
       vkEnumerateInstanceExtensionProperties(nullptr, &extCount, extensions.data());

       for (auto&[extensionName, specVersion]: extensions) {
           std::cout << "Extension: " << extensionName << std::endl;
       }
    }

    void listAvailableLayers() {
       uint32_t layersCount = 0;
       vkEnumerateInstanceLayerProperties(&layersCount, nullptr);

       std::vector<VkLayerProperties> layers(layersCount);
       vkEnumerateInstanceLayerProperties(&layersCount, layers.data());

       for (auto& layer : layers) {
           std::cout << "Layer Name = " << layer.layerName << std::endl;
       }

    }

    void listAvailableDevices(VkInstance& instance) {
        uint32_t availableDevicesCount = 0;
        vkEnumeratePhysicalDevices(instance, &availableDevicesCount, nullptr);
        
        VkPhysicalDevice  devices[availableDevicesCount];
        VkPhysicalDeviceProperties properties[availableDevicesCount];
        
        vkEnumeratePhysicalDevices(instance, &availableDevicesCount, devices);
        
        for (int i = 0; i < availableDevicesCount; i++) {
            vkGetPhysicalDeviceProperties(devices[i], &properties[i]);
            printf("Detected GPU -----> %s\n", properties[i].deviceName);
        }
    }

    void printCardsDetails(std::vector<vkEng::gpuDevice> &cardArray) {
        for (int i = 0; i < cardArray.size(); i++) {
            printf("Card name ----> %s\n", cardArray[i].properties.deviceName);
            printf("Card driver version ----> %d\n", cardArray[i].properties.driverVersion);
            printf("Card API version ----> %d", cardArray[i].properties.apiVersion);

            if (cardArray[i].queueProperties[i]. & VK_QUEUE_GRAPHICS_BIT)
                printf("%s supports graphic commands\n", cardArray[i].properties.deviceName);

            if (card.que & VK_QUEUE_COMPUTE_BIT)
                printf("%s supports compute shaders\n", cardArray[i].properties.deviceName);

            if (card.que & VK_QUEUE_TRANSFER_BIT)
                printf("%s supports memory transfer commands\n", cardArray[i].properties.deviceName);

            if (card.que & VK_QUEUE_SPARSE_BINDING_BIT)
                printf("%s supports memory sparse operations\n", cardArray[i].properties.deviceName);
            
        }
    }
}
