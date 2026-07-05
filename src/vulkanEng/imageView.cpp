#include <sys/types.h>
#include <vulkanEng.hpp>

namespace vkEng {
	void VulkanEng::createImageViews() 
	{

		// Setup the image handle vector
		uint32_t imageCount = 0;
		vkGetSwapchainImagesKHR(m_graphicsCard.logicalInstance, m_graphicsCard.swapChain, &imageCount, nullptr);
		if (imageCount == 0)
			throw std::runtime_error("Image count got zero");

		m_graphicsCard.swapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_graphicsCard.logicalInstance, m_graphicsCard.swapChain, &imageCount, m_graphicsCard.swapchainImages.data());


		// Setup the image view for each image
		m_graphicsCard.swapchainImageViews.resize(imageCount);

		VkImageViewCreateInfo viewInfo;

		size_t count = 0;
		for (const auto& image : m_graphicsCard.swapchainImages) {
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = image;
			viewInfo.format = this->pickSwapFormat(m_graphicsCard.swapChainProps.formats).format; // I should change this to store the format in a variable instead, that will save the overhead of calling the funcion again.
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(m_graphicsCard.logicalInstance, &viewInfo, nullptr, &m_graphicsCard.swapchainImageViews[count]) != VK_SUCCESS)
				throw std::runtime_error("Error create one of the image views for the image at swapchain");
		}
	}

	void VulkanEng::destroyImageViews()
	{
		for (size_t i = 0; i < m_graphicsCard.swapchainImageViews.size(); i++) {
			vkDestroyImageView(m_graphicsCard.logicalInstance, m_graphicsCard.swapchainImageViews[i], nullptr);
		}
	}
}