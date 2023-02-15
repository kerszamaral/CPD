#include <iostream>
#include "intercalacao.hpp"

int main()
{
    sort::loginfo_t loginfo;
    sort::array_t arrayFinal;
    sort::array_t array1 = {4, 8, 12, 16, 17};
    sort::array_t array2 = {1, 5, 9, 13};
    sort::array_t array3 = {3, 7, 11, 15};
    sort::array_t array4 = {2, 6, 10, 14, 18, 19};
    std::vector<sort::array_t> arrays = {array1, array2, array3, array4};

    sort::Intercalacao::multi_way_merge(arrays, arrayFinal, loginfo);

    for (auto num : arrayFinal)
    {
        std::cout << num << ", ";
    }
}