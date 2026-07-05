#include <window.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

namespace win {

    // Window contructor
    Window::Window(const char *name, int width, int height, std::function<void(void)> mainFunc) : m_height(height), m_width(width), m_mainLoop(std::move(mainFunc))
    {
    
    // Initialize GLFW and set the window hints
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        
        // Create the window and handle error
        m_glfwWin = glfwCreateWindow(m_width, m_height, name, nullptr, nullptr);
        if (m_glfwWin == nullptr)
            throw std::runtime_error("Error to create GLFW window"); // error creating the window
        
        glfwSetKeyCallback(m_glfwWin, windowKeyHandler); // Set the default handler for key inputs
        engine = std::make_unique<vkEng::VulkanEng>("Teste", "Teste");
        engine->setupWindowSurface(m_glfwWin);
        engine->pickChainExtent(m_glfwWin);
        engine->initSwapChain();

        initImGui();
    }

    void Window::initImgui(void)
    {

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForVulkan(m_glfwWin, true);
        
        ImGui_ImplVulkanH_Window mainWindowData;
        ImGui_ImplVulkan_InitInfo init = {
            .Allocator = nullptr,
            .Instance = engine->getEngineVulkanInstance(),
            .Device = engine->getEngineLogicalDevice(),
            .MinImageCount = 2,
            .Queue = engine->getEngineQueueHandler(),
            .QueueFamily = engine->getEngineQueueFamilyIndex(),
            .PipelineInfoMain.Subpass = 0,
            .PipelineInfoMain.RenderPass = mainWindowData.RenderPass,
            .PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT,
            .ImageCount = 2,
            .PhysicalDevice = engine->getEnginePhysicalDevice(),
            .DescriptorPool = VK_NULL_HANDLE,
            .DescriptorPoolSize = 8
        };

        if (!ImGui_ImplVulkan_Init(&init))
            throw std::runtime_error("Error init imgui");

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }


    // Object descontructor, frees the glfw resources and deletes vulkan instance
    Window::~Window() {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_glfwWin);
        glfwTerminate();

        #ifdef DEBUG
            std::cout << "Finished glfw\n";
        #endif
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
        switch (key) {
            case GLFW_KEY_A:
                std::cout << ((action == GLFW_PRESS) ? "Pressed A\n" : "Released A\n");
                break;
        }
    }
   
}
