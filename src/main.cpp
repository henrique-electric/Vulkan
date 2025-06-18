#include <window.hpp>


void loop(void) {

}


int main(int argc, char const *argv[])
{   
    win::Window mainWin("Test", 1280, 720, loop);
    mainWin.run();

    return 0;
}
