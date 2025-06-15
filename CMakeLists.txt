cmake_minimum_required(VERSION 3.10)
project(MyVulkanApp)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4 # You can choose a specific version
)

FetchContent_MakeAvailable(glfw)

if(WIN32)
    set(CMAKE_CXX_COMPILER cl)
    set(CMAKE_C_COMPILER cl)
endif()

if(LINUX)
    set(CMAKE_CXX_COMPILER clang++)
    set(CMAKE_C_COMPILER clang)
endif()



# Find Vulkan SDK
find_package(Vulkan REQUIRED)


# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

if(APPLE)
    find_library(MOLTENVK_LIBRARY MoltenVK PATHS ${Vulkan_LIBRARY})
    set(CMAKE_CXX_COMPILER clang++)
    set(CMAKE_C_COMPILER clang)
endif()

# Include and link
add_executable(MyVulkanApp src/main.cpp)
target_include_directories(MyVulkanApp PRIVATE /opt/homebrew/include/)
target_include_directories(MyVulkanApp PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(MyVulkanApp PRIVATE ${Vulkan_LIBRARIES})
target_link_libraries(MyVulkanApp PRIVATE glfw)
