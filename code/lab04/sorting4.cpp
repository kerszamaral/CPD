#include "cpd.hpp"
#include <iostream>
#include <tuple>
#include <string>
#include <vector>
#include <unordered_map>
// #include <bits/stdc++.h>
#include <map>

    using namespace std;

// Definições e variáveis globais necessárias
typedef int element_t;                     // tipo do elemento
typedef vector<element_t> array_t;         // tipo do array
typedef tuple<int, int, double> loginfo_t; // armazena informações de desempenho <trocas, comparações, tempo em ms>

void heapsort(array_t &, loginfo_t &);
void buildheap(array_t &, loginfo_t &);
int filho_e(const array_t &, element_t);
int filho_d(const array_t &, element_t);
int pai(const array_t &, element_t);
void heapify(array_t &, element_t, int, loginfo_t &);
int heap_max(const array_t &, loginfo_t &);
int extract_max(array_t &, loginfo_t &);
void heap_insert(array_t &, element_t, loginfo_t &);

void merge(const array_t &, const array_t &, array_t &, loginfo_t &);
void two_way_merge(const vector<array_t>, array_t &, loginfo_t &);
void multi_way_merge(const vector<array_t>, array_t &, loginfo_t &);
void mergesort(array_t &, loginfo_t &);
void swap(element_t *n1, element_t *n2);
typedef void (*sortfunc_t)(array_t &, loginfo_t &);

template <>
void cpd::Tester<sortfunc_t, loginfo_t>::TestFunction(sortfunc_t Function, array_t &Array, loginfo_t &loginfo)
{
    Function(Array, loginfo); // executa a função de ordenação
}

template <>
void cpd::Tester<sortfunc_t, loginfo_t>::DisplayLogNames(std::ostream &Output, const std::string &Separator)
{
    Output << Separator << std::setw(Spacer) << "Trocas"
           << Separator << std::setw(Spacer) << "Comparacoes"; // exibe os nomes das métricas de log
}

// int main(void)
// {
//     auto tester = cpd::Tester<sortfunc_t, loginfo_t>({{mergesort, "MergeSort"}});

//     tester.BatchTests(1, 1, 100);

//     return 0;
// }

int main(void)
{
    loginfo_t loginfo;
    int qtd;
    array_t array1 = {1, 2, 4, 6, 7, 8, 15, 22};
    array_t array2 = {1, 2, 3, 4, 5, 6, 7, 8, 25, 27, 35, 44};
    array_t array3, array4;

    for (auto e : array1)
        cout << e << " ";
    cout << endl;

    for (auto e : array2)
        cout << e << " ";
    cout << endl;

    merge(array1, array2, array3, loginfo);

    for (auto e : array3)
        cout << e << " ";
    cout << endl;

    vector<array_t> arrays = {array1, array2, array3};
    two_way_merge(arrays, array4, loginfo);
    for (auto e : array4)
        cout << e << " ";
    cout << endl;

    multi_way_merge(arrays, array4, loginfo);
    for (auto e : array4)
        cout << e << " ";
    cout << endl;

    // TODO: testar mergesort e mostrar log de operações

    return 0;
}

// ################################################
// Algoritmos de intercalação
// ################################################
void merge(const array_t &array1, const array_t &array2, array_t &array_final, loginfo_t &loginfo)
{
    int i = 0, j = 0, trocas = 0, comparacoes = 0;
    int qtd_a1 = array1.size();
    int qtd_a2 = array2.size();

    bool elementos = true;
    array_final.clear();
    while (i < qtd_a1 && j < qtd_a2)
    {
        if (array1[i] <= array2[j])
            array_final.push_back(array1[i++]);
        else
            array_final.push_back(array2[j++]);
    }

    if (j < qtd_a2 && i >= qtd_a1) // array 1 terminou
        for (auto e = j; e < qtd_a2; e++)
            array_final.push_back(array2[e]);

    if (i < qtd_a1 && j >= qtd_a2) // array 2 terminou
        for (auto e = i; e < qtd_a1; e++)
            array_final.push_back(array1[e]);

    // TODO: atualizar loginfo
}

// Recebe uma lista de arrays e intercala-os 2 a 2
// retorna um array com o resultado da intercalação
void two_way_merge(const vector<array_t> arrays, array_t &array_final, loginfo_t &loginfo)
{
    array_final.clear();

    for (auto i = 0; i < arrays.size(); i += 2)
    {
        array_t array1 = arrays[i];
        array_t array2 = arrays[i + 1];
        array_t array_final_temp;
        merge(array1, array2, array_final_temp, loginfo);
        array_final = array_final_temp;
    }
    // TODO: atualizar loginfo
}

//Recebe uma lista de arrays e intercala - os usando estrutura similar a heap - min
//retorna um array com o resultado da intercalação
void multi_way_merge(const vector<array_t> arrays, array_t &array_final, loginfo_t &loginfo)
{
    array_final.clear();

    //multi way merge
    std::vector<int> Array;

    auto i = 0;
    while (true)
    {
        for(auto array : arrays)
        {
            if(i >= array.size())
            {
                break;
            }
            Array.emplace_back(array[i]);
        }
        i++;
        
    }

    cout << "Array: ";
    for(auto elem : Array)
    {
        cout << elem << " ";
    }	
    cout << endl;

    buildheap(Array, loginfo);
    // TODO: atualizar loginfo
}

void intercala(int array[], int left, int mid, int right, loginfo_t &loginfo)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;

    // Create temp arrays
    auto *leftArray = new int[subArrayOne],
         *rightArray = new int[subArrayTwo];

    // Copy data to temp arrays leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];

    auto indexOfSubArrayOne = 0,   // Initial index of first sub-array
        indexOfSubArrayTwo = 0;    // Initial index of second sub-array
    int indexOfMergedArray = left; // Initial index of merged array

    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo)
    {
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo])
        {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else
        {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne)
    {
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo)
    {
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
    delete[] leftArray;
    delete[] rightArray;
}

void mergeSort(array_t &array, int const begin, int const end, loginfo_t &loginfo)
{
    if (begin >= end)
        return; // Returns recursively

    auto mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid, loginfo);
    mergeSort(array, mid + 1, end, loginfo);
    intercala(array.data(), begin, mid, end, loginfo);
}

void mergesort(array_t &array, loginfo_t &loginfo)
{
    mergeSort(array, 0, array.size() - 1, loginfo);
}

// ################################################
// Algoritmos de ordenação por Seleção
// ################################################

// Heapsort e funções auxiliares
void heapsort(array_t &array, loginfo_t &loginfo)
{
    int trocas = 0;
    int comparacoes = 0;
    int heap_size = array.size();
    int qtd_elementos = heap_size - 1;

    buildheap(array, loginfo);

    for (auto i = qtd_elementos; i > 0; i--)
    {
        trocas++;
        swap(array[i], array[0]);
        heap_size--;
        heapify(array, 0, heap_size, loginfo);
    }

    get<0>(loginfo) += trocas;
    get<1>(loginfo) += comparacoes;
}

// usada no heapsort
void buildheap(array_t &array, loginfo_t &loginfo)
{
    int ultimo_pai = (array.size() / 2) - 1;
    for (auto i = ultimo_pai; i >= 0; i--)
        heapify(array, i, array.size(), loginfo);
}

int filho_e(const array_t &array, element_t elemento)
{
    return elemento * 2 + 1;
}

int filho_d(const array_t &array, element_t elemento)
{
    return elemento * 2 + 2;
}

int pai(const array_t &array, element_t elemento)
{
    return (elemento / 2);
}

// ################################################
// Implementação dos seus algoritmos:

// heapify: verifica se o elemento na posição passada é um heap e se não for transforma-o em um
// parâmetros: array, índice do elemento a heapificar, tamanho do heap, dicionário de logs
void heapify(array_t &array, element_t elemento, int heap_size, loginfo_t &loginfo)
{
    auto e = filho_e(array, elemento);
    auto d = filho_d(array, elemento);
    auto maior = elemento;

    if (e < heap_size && array[e] > array[maior])
        maior = e;

    if (d < heap_size && array[d] > array[maior])
        maior = d;

    if (maior != elemento)
    {
        swap(array[maior], array[elemento]);
        heapify(array, maior, heap_size, loginfo);
    }
}

int heap_max(array_t &heap, loginfo_t &loginfo)
{
    auto max = heap[0];
    return max;
}

int extract_max(array_t &heap, loginfo_t &loginfo)
{
    if (heap.size() < 1)
        return -1;

    auto max = heap[0];
    heap[0] = heap[heap.size() - 1];
    heap.resize(heap.size() - 1);
    heapify(heap, 0, heap.size(), loginfo);
    return max;
}

void heap_insert(array_t &heap, element_t elemento, loginfo_t &loginfo)
{
    heap.resize(heap.size() + 1);
    auto i = heap.size();
    while (i > 0 && heap[pai(heap, i)] < elemento)
    {
        heap[i] = heap[pai(heap, i)];
        i = pai(heap, i);
    }
    heap[i] = elemento;
}