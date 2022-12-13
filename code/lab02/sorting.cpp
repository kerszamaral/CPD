// Atenção: usa código C++11
// para saber se o seu compilador tem suporte, execute:
// cout << __cplusplus;
// O resultado deve ser 201103L ou maior.
// o do google collab é C++14
// A grande maioria dos compiladores atuais suporta nativamente c++11.
// Outros exigem a configuração de parâmetros de compilação... Verifique a documentação do seu.

#include <iostream>
#include <tuple>
#include <random>
#include <bits/stdc++.h>
#include <chrono>

#define MAX 100 // quantidade de números no array

using namespace std;

typedef int array_size_t;               // Tipo para especificar tamanho do array
typedef int *array_t;                   // Tipo para especificar formato do array
typedef std::tuple<int, int> loginfo_t; // armazena contagem de <trocas , comparacoes>
typedef std::mt19937 MyRNG;             // Gerador de números aleatórios do tipo Mersenne Twister Random Generator

MyRNG rng;         // gerador de números aleatórios
uint32_t seed_val; // semente de geração de números

void bubblesort(array_t, array_size_t, loginfo_t &);
void quicksort(array_t, int, int, loginfo_t &);
int particiona(array_t array, int i, int f, loginfo_t &);
void swap(int *n1, int *n2);

int main(void)
{
    // cout << __cplusplus << endl;                                             // verifica versão do compilador (descomente, se necessário avaliar)
    rng.seed(seed_val);                             // inicializa semente de geração de números aleatórios
    uniform_int_distribution<> distrib(0, INT_MAX); // cria gerador com distribuição uniforme entre 0 e MAX_INT
    loginfo_t loginfo;                              // armazena contadores de comparações e trocas (ver typedef acima)

    int *array = new int[MAX]; // array dinâmico que armazena os números

    // for(auto i=0;i<MAX;i++) array[i] = MAX-i;                                 // gera números em ordem decrescente
    for (auto i = 0; i < MAX; i++)
        array[i] = i; // gera números em ordem crescente

    cout << "Array desordenado: ";
    for (auto i = 0; i < MAX; i++)
        cout << array[i] << " ";

    auto start = std::chrono::steady_clock::now();
    bubblesort(array, MAX, loginfo); // passa tamanho do array
    // quicksort(array, 0, MAX-1, loginfo);                                      // passa início e fim do trecho de processamento (MAX-1)
    auto finish = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed_seconds = finish - start;

    // TODO: armazenar essas informações em um matriz, hashtable ou arquivo

    cout << endl
         << "Array ordenado: ";
    for (auto i = 0; i < MAX; i++)
        cout << array[i] << " ";

    cout << endl;
    cout << "Quantidade de trocas: " << get<0>(loginfo) << endl;
    cout << "Quantidade de comparações: " << get<1>(loginfo) << endl;
    cout << "Tempo total: " << elapsed_seconds.count() << "s" << endl;

    delete[] array;
    return 0;
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