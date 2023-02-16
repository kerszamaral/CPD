#include "cpd.hpp"

using namespace std;

void resetStats(sort::loginfo_t &loginfo)
{
    std::get<0>(loginfo) = 0;
    std::get<1>(loginfo) = 0;
}

void printStats(sort::array_t &array, sort::loginfo_t &loginfo, cpd::Timer &timer)
{
    for (auto i : array)
        std::cout << i << " ";
    std::cout << std::endl
              << "Trocas: " << std::get<0>(loginfo)
              << " Comparacoes: " << std::get<1>(loginfo)
              << " Tempo: " << timer.Duration().count() << "s" << endl << endl;
}

int main(void)
{
    cpd::Tester<sort::sortfunc_t, sort::loginfo_t> tester({
        {sort::Selecao::heapsort, "HeapSort"},
        {sort::Intercalacao::mergesort, "MergeSort"},
        {sort::Linear::radixsortLSD, "RadixSortLSD"},
        {sort::Linear::radixsortMSD, "RadixSortMSD"},
        {sort::Linear::countingsort, "CountingSort"}
    });
    tester.BatchTests(0, 3, 100);
    // Para estes testes, a funcao de numeros aleatorio foi desabilidata, igualmente a de numeros iguais.

    cout << endl;

    //! Testes de merge
    sort::loginfo_t loginfo; // Trocas, Comparacoes
    cpd::Timer timer;        // Tempo
    sort::array_t array1 = {1, 2, 4, 6, 7, 8, 15, 22};
    sort::array_t array2 = {1, 2, 3, 4, 5, 6, 7, 8, 25, 27, 35, 44};
    sort::array_t array3, array4;

    cout << "Array 1: " << endl;
    for (auto e : array1)
        cout << e << " ";
    cout << endl
         << endl;

    cout << "Array 2: " << endl;
    for (auto e : array2)
        cout << e << " ";
    cout << endl
         << endl;

    //! Merge
    timer.Start();
    sort::Intercalacao::merge(array1, array2, array3, loginfo);
    timer.Stop();
    cout << "Merge: " << endl;
    printStats(array3, loginfo, timer);

    //! Two way merge
    vector<sort::array_t> arrays = {array1, array2, array3};

    resetStats(loginfo);
    timer.Start();
    sort::Intercalacao::two_way_merge(arrays, array4, loginfo);
    timer.Stop();
    cout << "Two way merge: " << endl;
    printStats(array4, loginfo, timer);

    //! Multi way merge
    resetStats(loginfo);
    timer.Start();
    cout << "Multi way merge: " << endl;
    sort::Intercalacao::multi_way_merge(arrays, array4, loginfo);
    timer.Stop();
    printStats(array4, loginfo, timer);

    return 0;
}
