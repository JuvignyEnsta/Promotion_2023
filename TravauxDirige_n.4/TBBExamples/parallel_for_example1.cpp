#include <iostream>
#include <tbb/parallel_for.h>
#include <tbb/tbb.h>

void print_hello( int i )
{
    std::cout << i << " : Hello world !" << std::endl;
}

int main()
{
    tbb::parallel_for(0, 100, 2, [](int i) { print_hello(i); });
    return 0;
}
