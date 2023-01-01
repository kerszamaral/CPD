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
// #include <bits/stdc++.h>
#include <stack>
#include <chrono>

using namespace std;

typedef int array_size_t;                     // Tipo para especificar tamanho do array
typedef int *array_t;                         // Tipo para especificar formato do array
typedef std::tuple<size_t, size_t> loginfo_t; // armazena contagem de <trocas , comparacoes>
typedef std::mt19937 MyRNG;                   // Gerador de números aleatórios do tipo Mersenne Twister Random Generator

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

//Implementações para a utilização da biblioteca cpd
typedef void (*Functions_t)(array_t, array_size_t, loginfo_t &); // Tipo para especificar funções de ordenação

template <>
void cpd::Tester<Functions_t, loginfo_t>::TestFunction(Functions_t Function, array_t Array, array_size_t ASize, loginfo_t &loginfo)
{
    Function(Array, ASize, loginfo); // executa a função de ordenação
}

template <>
void cpd::Tester<Functions_t, loginfo_t>::DisplayLogNames(std::ostream &Output, const std::string &Separator)
{
    Output << Separator << std::setw(Spacer) << "Trocas" 
           << Separator << std::setw(Spacer) << "Comparacoes"; // exibe os nomes das métricas de log
}

void TestLoop(int runs, bool automatic, bool crash)
{
    cpd::Timer timer;
    cpd::Tester<Functions_t, loginfo_t> tester = cpd::Tester<Functions_t, loginfo_t>({{insertion_sort, "Insertionsort"}}); // instancia o objeto de teste

    if (crash)
    {
        tester = cpd::Tester<Functions_t, loginfo_t>({{insertion_sort, "Insertionsort"},
                                                      {bubblesort, "Bubblesort"},
                                                      {shakesort, "Shakesort"},
                                                      {combsort, "Combsort"},
                                                      {quicksortIter, "Quicksort Iterative"},
                                                      {quicksortPoint, "Quicksort Original"},
                                                      {quicksortRand, "Quicksort Random"}}); // instancia o objeto de teste
    }
    else
    {
        tester = cpd::Tester<Functions_t, loginfo_t>({{insertion_sort, "Insertionsort"},
                                                      {bubblesort, "Bubblesort"},
                                                      {shakesort, "Shakesort"},
                                                      {combsort, "Combsort"},
                                                      {quicksortIter, "Quicksort Iterative"}}); // instancia o objeto de teste
    }
    tester.Spacer = 28;
    timer.Start();
    tester.BatchTests(automatic, runs, 100);
    timer.Stop();
    std::cout << "Tempo de execução do codigo:" << timer.Duration().count() << endl;
}

int main(void)
{
    while (true)
    {
        int runs;
        bool automatic, crash;
        cout << "Digite o numero de execucoes (0 para sair): ";
        cin >> runs;
        if (runs <= 0)
        {
            break;
        }
        cout << "Digite 1 para automatico ou 0 para manual: ";
        cin >> automatic;
        cout << "Digite 1 para possivel estouro de pilha ou 0 para sem as funcoes recursivas: ";
        cin >> crash;
        TestLoop(runs, automatic, crash);
    }

    return 0;
}

void quicksortIter(array_t array, array_size_t array_size, loginfo_t &loginfo)
{
    for (int i = 1; i < array_size; i++) // Verifica se o array já está ordenado
    {
        get<1>(loginfo)++; // Incrementa a contagem de comparações
        if (array[i - 1] > array[i])
        {
            break; // Se não estiver, sai do loop
        }

        if (i == array_size - 1)
        {
            return; // Se estiver, retorna
        }
    }

    int p, i, f;                // Declara as variáveis
    stack<int> pilha;           // Cria a pilha
    pilha.push(0);              // Empilha o primeiro elemento
    pilha.push(array_size - 1); // Empilha o último elemento
    while (!pilha.empty())      // Enquanto a pilha não estiver vazia
    {
        f = pilha.top(); // Desempilha o último elemento
        pilha.pop();     // Desempilha o último elemento
        i = pilha.top(); // Desempilha o último elemento
        pilha.pop();     // Desempilha o último elemento
        if (f > i)
        {
            p = particiona(array, i, f, loginfo); // Particiona o array
            pilha.push(i);                        // Empilha o primeiro elemento
            pilha.push(p - 1);                    // Empilha o último elemento
            pilha.push(p + 1);                    // Empilha o primeiro elemento
            pilha.push(f);                        // Empilha o último elemento
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
    swap(&array[rand() % (array_size)], &array[0]); // troca o primeiro elemento com um aleatório
    quicksort(array, 0, array_size - 1, loginfo);   // passa início e fim do trecho de processamento (MAX-1)
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
        gap = floor(gap / 1.3); // 1.3 é o fator de redução
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