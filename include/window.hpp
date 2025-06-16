#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkanEng.hpp>
#include <string>
#include <iostream>
#include <stdexcept>
#include <functional>

namespace win {
    class Window : public vkEng::VulkanEng
    {
    private:
        GLFWwindow *m_glfwWin;
        int m_height, m_width;
        std::function<void(void)> m_mainLoop;

        // functions
        static void windowKeyHandler(GLFWwindow* window, int key, int scancode, int action, int mods);

    public:

        // Functions
        Window(const char *name, int width, int height, std::function<void(void)> mainFunc);
        ~Window();

        void run();

        inline int  shouldClose() { return glfwWindowShouldClose(m_glfwWin); }; // simple wrapper for GLFW windowShouldClose function
        inline void handleEvents() { glfwPollEvents(); }; // Simple wrapper for GLFW event pooling function
    };
}