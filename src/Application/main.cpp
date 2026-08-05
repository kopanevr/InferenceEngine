#include "Application.hpp"

int main(int argc, char* argv[])
{
    return Application::getInstance(argc, argv)->exec();
}
