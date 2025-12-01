cmake_minimum_required(VERSION 3.10)
project(MyVulkanApp)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4 # You can choose a specific version
)

FetchContent_MakeAvailable(glfw)

# Find Vulkan SDK
find_package(Vulkan REQUIRED)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
option(BUILD_SHARED_LIBS "" off)

if(APPLE)
    find_library(MOLTENVK_LIBRARY MoltenVK PATHS ${Vulkan_LIBRARY})
endif()

set(IMGUI_CORE 
    # ======== Backends =========
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_vulkan.cpp
    # ===========================

    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui.cpp
)

add_library(imgui ${IMGUI_CORE})
target_include_directories(imgui PUBLIC external/imgui)
target_include_directories(imgui PUBLIC external/imgui/backends)
target_include_directories(imgui PUBLIC /opt/homebrew/include/)
target_link_libraries(imgui glfw)

# Include and link
add_executable(MyVulkanApp src/main.cpp src/window.cpp src/vulkanEng/vulkanEng.cpp
        src/vulkanEng/vulkanEngDebug.cpp src/utils.cpp)

target_include_directories(MyVulkanApp PRIVATE /opt/homebrew/include/)
target_include_directories(MyVulkanApp PRIVATE ${Vulkan_INCLUDE_DIRS})
target_include_directories(MyVulkanApp PUBLIC ./include/)
target_include_directories(MyVulkanApp PUBLIC ./build/_deps/fmt-src/include/)

target_link_libraries(MyVulkanApp PRIVATE ${Vulkan_LIBRARIES} glfw imgui)
