#include "cpd.hpp"

int main()
{
    auto tester = cpd::Tester<sort::sortfunc_t, sort::loginfo_t>({
        {sort::Trocas::bubblesort, "Bubblesort"},
        {sort::Trocas::quicksort, "Quicksort"}
    });

    tester.BatchTests(1, 1, 100);
    
    return 0;
}