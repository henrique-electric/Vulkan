#pragma once


#include <vulkan/vulkan.hpp>
#include <types.hpp>


// std includes
#include <iostream>

namespace utils {

    bool strcmp(const char* str1, const char* str2);
    void listAvailableExtensions();
    void listAvailableLayers();
    void listAvailableDevices(VkInstance& instance);
    void printCardsDetails(std::vector<vkEng::gpuDevice>& cardArray);
}
