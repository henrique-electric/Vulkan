#include <window.hpp>
#include <vulkanEng.hpp>

void loop(void) {

}


int main(int argc, char const *argv[])
{   
    win::Window window("Test", 1280, 720, loop);
    vkEng::VulkanEng eng("Test", "Engine");
    window.run();
    return 0;
}
