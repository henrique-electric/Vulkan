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
}
