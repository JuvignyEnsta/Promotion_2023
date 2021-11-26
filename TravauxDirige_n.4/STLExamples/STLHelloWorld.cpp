#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>

int main()
{
    std::vector<std::string> v = { "Hello ",
                                   "World ! "};
    std::for_each(std::execution::par, v.begin(), v.end(),
        [](std::string &s) { std::cout << s << std::endl; } );
    return 0;
}