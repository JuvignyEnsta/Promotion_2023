#include <iostream>
#include <tbb/tbb.h>

int main()
{
    tbb::parallel_invoke(
        []() { std::cout << "Hello " << std::endl; },
        []() { std::cout << "World ! " << std::endl; }
    );
    return 0;
}