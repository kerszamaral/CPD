#include "cpd.hpp"


// Using counting sort to sort the elements in the basis of significant places


// Main function to implement radix sort


int main (void)
{
    sort::array_t array = { 43, 271, 2578, 822, 3, 3, 17 };
    sort::loginfo_t loginfo;

    sort::Linear::radixsortMSD(array, loginfo);

    for (auto i : array)
        std::cout << i << " ";
    std::cout << std::endl;

    return 0;
}