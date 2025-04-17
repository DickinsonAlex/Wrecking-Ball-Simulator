#include "Engine.h"
#include <iostream>

int main()
{
    try
    {
        Engine::Init("Mobile Crane Test", 1280, 720);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return 0;
    }

    Engine::Start();
    return 0;
}
