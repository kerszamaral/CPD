// Atenção: usa código C++11
// para saber se o seu compilador tem suporte, execute:
// cout << __cplusplus;
// O resultado deve ser 201103L ou maior.
// o do google collab é C++14
// A grande maioria dos compiladores atuais suporta nativamente c++11.
// Outros exigem a configuração de parâmetros de compilação... Verifique a documentação do seu.
#include "../cpd.hpp"

#include <iostream>
#include <tuple>
#include <random>
#include <bits/stdc++.h>
#include <chrono>

using namespace std;

typedef int array_size_t;                       // Tipo para especificar tamanho do array
typedef int *array_t;                           // Tipo para especificar formato do array
typedef std::tuple<size_t, size_t> loginfo_t; // armazena contagem de <trocas , comparacoes>
typedef std::mt19937 MyRNG;                     // Gerador de números aleatórios do tipo Mersenne Twister Random Generator

MyRNG rng;         // gerador de números aleatórios
uint32_t seed_val; // semente de geração de números

void bubblesort(array_t, array_size_t, loginfo_t &);
void quicksort(array_t, int, int, loginfo_t &);
int particiona(array_t array, int i, int f, loginfo_t &);
void swap(int *n1, int *n2);
void combsort(array_t array, array_size_t array_size, loginfo_t &loginfo);
void shakesort(array_t array, array_size_t array_size, loginfo_t &loginfo);
void insertion_sort(array_t array, array_size_t array_size, loginfo_t &loginfo);
void quicksortPoint(array_t array, array_size_t array_size, loginfo_t &loginfo);
void quicksortRand(array_t array, array_size_t array_size, loginfo_t &loginfo);
void quicksortIter(array_t array, array_size_t array_size, loginfo_t &loginfo);

#define RUNS 1                                                   // quantidade de vezes que cada teste será executado
typedef void (*Functions_t)(array_t, array_size_t, loginfo_t &); // Tipo para especificar funções de ordenação

template <>
void cpd::Tester<Functions_t, loginfo_t>::TestFunction(Functions_t Function, array_t Array, array_size_t ASize, loginfo_t &loginfo)
{
    Function(Array, ASize, loginfo);
}

template <>
void cpd::Tester<Functions_t, loginfo_t>::DisplayLogNames(std::ostream &Output, const std::string &Separator)
{
    Output << Separator << std::setw(Spacer) << "Trocas"
           << Separator << std::setw(Spacer) << "Comparacoes";
}

int main(void)
{
    cpd::Timer timer;

    cpd::Tester<Functions_t, loginfo_t> tester = cpd::Tester<Functions_t, loginfo_t>(
        {bubblesort, quicksortPoint, shakesort, combsort, insertion_sort, quicksortIter, quicksortRand},
        {"Bubblesort", "Quicksort", "Shakesort", "Combsort", "Insertionsort", "quicksort Iterative", "Quicksort Random"}); // instancia o objeto de teste

    tester.Spacer = 28;
    timer.Start();
    tester.BatchTests(false, 3, 100);
    timer.Stop();
    std::cout << timer.Duration().count() << endl;
    return 0;
}

void quicksortIter(array_t array, array_size_t array_size, loginfo_t &loginfo)
{
    int p, i, f;
    stack<int> pilha;
    pilha.push(0);
    pilha.push(array_size - 1);
    while (!pilha.empty())
    {
        f = pilha.top();
        pilha.pop();
        i = pilha.top();
        pilha.pop();
        if (f > i)
        {
            p = particiona(array, i, f, loginfo);
            pilha.push(i);
            pilha.push(p - 1);
            pilha.push(p + 1);
            pilha.push(f);
        }
    }
}

// Função de quicksort
void quicksort(array_t array, int i, int f, loginfo_t &loginfo)
{
    int p;
    if (f > i)
    {
        p = particiona(array, i, f, loginfo);
        quicksort(array, i, p - 1, loginfo);
        quicksort(array, p + 1, f, loginfo);
    }
}

void quicksortRand(array_t array, array_size_t array_size, loginfo_t &loginfo)
{
    swap(&array[rand() % (array_size)], &array[0]);
    quicksort(array, 0, array_size - 1, loginfo); // passa início e fim do trecho de processamento (MAX-1)
}

void quicksortPoint(array_t array, array_size_t array_size, loginfo_t &loginfo)
{
    quicksort(array, 0, array_size - 1, loginfo); // passa início e fim do trecho de processamento (MAX-1)
}

int particiona(array_t array, int esq, int dir, loginfo_t &loginfo)
{
    int i = esq + 1, j = dir, pivo = esq;
    while (j > i)
    {
        while (array[i] < array[pivo] && i < dir)
        {
            i++;
            get<1>(loginfo)++;
        }
        while (array[j] >= array[pivo] && j > esq)
        {
            j--;
            get<1>(loginfo)++;
        }

        if (i < j && array[i] > array[j])
        {
            swap(&array[i], &array[j]);
            get<0>(loginfo)++;
            get<1>(loginfo)++;
        }
    }
    // get<1>(loginfo)++;
    if (array[j] < array[pivo])
    {
        swap(&array[pivo], &array[j]);
        get<0>(loginfo)++;
        get<1>(loginfo)++;
    }
    return j;
}

void swap(int *n1, int *n2)
{
    int troca = *n1;
    *n1 = *n2;
    *n2 = troca;
}

void bubblesort(array_t array, array_size_t array_size, loginfo_t &loginfo)
{
    int trocas = 0;
    int comparacoes = 0;
    int pos_troca = 0;
    bool troca = true;
    int qtd_elementos = array_size - 1;

    while (troca)
    {
        troca = false;
        for (auto i = 0; i < qtd_elementos; i++)
        {
            comparacoes = comparacoes + 1;
            if (array[i] > array[i + 1])
            {
                swap(array[i], array[i + 1]);
                troca = true;
                pos_troca = i;
                trocas = trocas + 1;
            }
        }
        qtd_elementos = pos_troca;
    }

    get<0>(loginfo) = trocas;
    get<1>(loginfo) = comparacoes;
}

void shakesort(array_t array, array_size_t array_size, loginfo_t &loginfo)
{
    int trocas = 0;
    int comparacoes = 0;
    int pos_troca = 0;
    bool troca = true;
    int qtd_elementos = array_size - 1;

    while (troca)
    {
        troca = false;
        for (auto i = 0; i < qtd_elementos; i++)
        {
            comparacoes++;
            if (array[i] > array[i + 1])
            {
                swap(array[i], array[i + 1]);
                troca = true;
                pos_troca = i;
                trocas++;
            }
        }
        qtd_elementos = pos_troca;
        for (auto i = qtd_elementos; i > 0; i--)
        {
            comparacoes = comparacoes + 1;
            if (array[i] < array[i - 1])
            {
                swap(array[i], array[i - 1]);
                troca = true;
                pos_troca = i;
                trocas++;
            }
        }
    }

    get<0>(loginfo) = trocas;
    get<1>(loginfo) = comparacoes;
}

void combsort(array_t array, array_size_t array_size, loginfo_t &loginfo)
{
    int trocas = 0;
    int comparacoes = 0;
    bool troca = true;
    int qtd_elementos = array_size - 1;
    int gap = qtd_elementos;

    while (troca)
    {
        gap = floor(gap / 1.3);
        if (gap < 1)
        {
            troca = false;
            gap = 1;
        }

        for (auto i = 0; i + gap <= qtd_elementos; i++)
        {
            comparacoes++;
            if (array[i] > array[i + gap])
            {
                swap(array[i], array[i + gap]);
                troca = true;
                trocas++;
            }
        }
    }

    get<0>(loginfo) = trocas;
    get<1>(loginfo) = comparacoes;
}
// Função de insertion Sort
void insertion_sort(array_t array, array_size_t array_size, loginfo_t &loginfo)
{
    int trocas = 0;
    int comparacoes = 0;
    for (int i = 1; i < array_size; i++)
    {                          // do segundo ao último
        auto chave = array[i]; // chave a inserir no subarray ordenado
        auto j = i - 1;        // último elemento do subarray ordenado
        comparacoes = comparacoes + 1;
        while (j >= 0 && array[j] > chave)
        { // busca linear da direita para a esquerda no subarray ordenado
            comparacoes = comparacoes + 1;
            array[j + 1] = array[j];
            j = j - 1;
            trocas = trocas + 1;
        }
        if (j + 1 != i)
        {
            array[j + 1] = chave;
            trocas = trocas + 1;
        }
    }
    get<0>(loginfo) = trocas;
    get<1>(loginfo) = comparacoes;
}