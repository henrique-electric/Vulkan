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
        puts("\nCards infos\n");
        for (int i = 0; i < cardArray.size(); i++) {
            printf("Card name ----> %s\n", cardArray[i].properties.deviceName);
            printf("Card driver version ----> %d\n", cardArray[i].properties.driverVersion);
            printf("Card API version ----> %d\n\n", cardArray[i].properties.apiVersion);
            
            for (int j = 0; j < cardArray[i].queueProperties.size(); j++) {
                printf("------Family %d------\n", j);
                printf("%d available queues\n", cardArray[i].queueProperties[j].queueCount);
                LIST_FAMILY_COMMAND_SUPPORT(cardArray[i].queueProperties[j]);
                printf("----------------------\n\n");
            }
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
}
