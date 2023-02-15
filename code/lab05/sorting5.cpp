#include "cpd.hpp"

/*
*I) Análise de desempenho de heapsort //!Okay
Faça uma análise de desempenho do heapsort sendo aplicado em arrays de tamanho 100, 1000 e 10000, tanto crescentes quanto decrescentes (verifique a quantidade de trocas, a de comparações e o tempo de processamento).
Nota máxima1
*II) Implementação de two_way_merge
Implementar two_way_merge (que compara 2 a 2, mesmo que receba uma lista de vários arrays)
Nota máxima1
*II) Implementação de multi_way_merge
Implementar multi_way_merge (que recebe vários arrays e compara todos ao mesmo tempo, usando como base uma estrutura auxiliar do tipo min-heap.
Nota máxima1.5
*II) Implementação do merge_sort //!Okay
Implementar merge_sort.
Nota máxima1
*III) Avaliação do desempenho do mergesort em arrays de tamanho 100, 1000 e 10000, tanto crescentes quanto decrescentes (verifique a quantidade de trocas, a de comparações e o tempo de processamento.
Nota máxima1 //!Okay
*IV) Implementação do RADIX-sort LSD //!Okay
Nota máxima0.5
*IV) Implementação do RADIX-sort MSD
Nota máxima0.5
*IV) Implementação do CountingSort
Nota máxima0.5
*/

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
        // {sort::Linear::radixsortMSD, "RadixSortMSD"},
        // {sort::Linear::countingsort, "CountingSort"}
    });
    tester.BatchTests(1, 3, 100);

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
