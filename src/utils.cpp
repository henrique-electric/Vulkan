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

    void printCardDetails(vkEng::gpuDevice &card) {
        printf("Card name: %s\n", card.properties.deviceName);
        printf("Card driver version: %d\n", card.properties.driverVersion);
        printf("Card API version: %d\n", card.properties.apiVersion);
       
        for (int familyCount = 0; familyCount < card.queueProperties.size(); familyCount++) {
            printf("\n\n---------Family %d--------\n", familyCount);
            printf("Family has %d queues\n", card.queueProperties[familyCount].queueCount);
            LIST_FAMILY_COMMAND_SUPPORT(card.queueProperties[familyCount]); 
            puts("\n\n"); 
        }
    }

    void listCardAvailableExt(vkEng::gpuDevice& card) {
        uint32_t extCount = 0;
        vkEnumerateDeviceExtensionProperties(card.device, nullptr, &extCount, nullptr);
        VkExtensionProperties extentions[extCount];
        
        vkEnumerateDeviceExtensionProperties(card.device, nullptr, &extCount, extentions);
        for (int i = 0; i < extCount; i++)
            printf("Found the device extension %s\n", extentions[i].extensionName);
        
    }

    // TODO
    void listDeviceFeatures(const vkEng::gpuDevice& card) {
        VkPhysicalDeviceFeatures features{};
        vkGetPhysicalDeviceFeatures(card.device, &features);

    }
}
