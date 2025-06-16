#include <utility>
#include <window.hpp>

namespace win {

    // Window contructor
    Window::Window(const char *name, int width, int height, std::function<void(void)> mainFunc) : VulkanEng(name, "Engine"), m_height(height), m_width(width), m_mainLoop(std::move(mainFunc)) {

        // Initialize GLFW and set the window hints
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // Create the window and handle error
        m_glfwWin = glfwCreateWindow(m_width, m_height, name, nullptr, nullptr);
        if (m_glfwWin == nullptr)
            throw std::runtime_error("Error to create GLFW window"); // error creating the window

        glfwSetKeyCallback(m_glfwWin, windowKeyHandler); // Set the default handler for key inputs
    }

    // Object descontructor, frees the glfw resources and deletes vulkan instance
    Window::~Window() {
        vkDestroyInstance(m_vkInstance, nullptr);
        glfwDestroyWindow(m_glfwWin);
        glfwTerminate();
    }

    void Window::run() {
        while(!shouldClose()) {
            m_mainLoop();

            handleEvents();
        }
    }
} 

namespace win {
    void Window::windowKeyHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {
        
    }
}
