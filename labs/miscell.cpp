#include <unordered_map>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

const std::unordered_map<int, std::string> WEEK = {
    {1, "Monday"},
    {2, "Tuesday"}
};

int main() {
    std::srand(std::time(0));
    std::cout << std::rand() % 10 << "\n";
    std::cout << WEEK.at(1);

    int* ptr = new int[10]{};
    std::cout << *ptr;
    delete[] ptr;

    int from[]{1, 2, 3};
    int to[5];

    std::memcpy(to, from, sizeof(from));

    std::memset(to, 0, sizeof(to));

    
}