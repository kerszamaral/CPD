/*
Ian Kersz Amaral -- 00338368
UFRGS 2022
Header para os trabalhos de laboratório de CPD
*/

#include <iostream>
#include <tuple>
#include <random>
#include <bits/stdc++.h>
#include <chrono>

typedef int array_size_t;                      // Tipo para especificar tamanho do array
typedef int *array_t;                          // Tipo para especificar formato do array
typedef std::chrono::duration<double> timer;   // Tipo para especificar o timer do programa
typedef std::tuple<int, int, timer> loginfo_t; // armazena contagem de <trocas , comparacoes, tempo de execucao>
typedef std::mt19937 MyRNG;                    // Gerador de números aleatórios do tipo Mersenne Twister Random Generator
typedef void (*Functions_t)(array_t, array_size_t, loginfo_t &); // Tipo para especificar funções de ordenação
typedef std::tuple<Functions_t, std::string> SortFunctions_t;  // Tipo para especificar funções de ordenação e seus nomes

MyRNG rng;     // gerador de números aleatórios
uint32_t seed_val; // semente de geração de números
std::string testNames[] = {"Crescente", "Aleatorio", "Decrescente"}; // nomes dos testes

void displayArray(std::string &name, loginfo_t &loginfo)
{
    std::cout
        << "\t\t" << name << ":" << std::endl
        << "\t\t\tQuantidade de trocas: " << std::get<0>(loginfo) << std::endl
        << "\t\t\tQuantidade de comparações: " << std::get<1>(loginfo) << std::endl
        << "\t\t\tTempo de execucao: " << std::get<2>(loginfo).count() << "s" << std::endl;
}

void tests(SortFunctions_t sortFunctions[], int type, array_t array, array_size_t array_size, loginfo_t &loginfo)
{
    auto start = std::chrono::high_resolution_clock::now(); // inicia timer
    std::get<0>(sortFunctions[type])(array, array_size, loginfo); // executa função de ordenação
    auto end = std::chrono::high_resolution_clock::now();  // finaliza timer
    std::get<2>(loginfo) = end - start; // armazena tempo de execução
}

void runs(SortFunctions_t sortFunctions[], int num_of_functions, array_size_t array_size, int type, bool display, loginfo_t *log)
{
    rng.seed(seed_val); // inicializa semente de geração de números aleatórios

    std::uniform_int_distribution<> distrib(0, INT_MAX); // cria gerador com distribuição uniforme entre 0 e MAX_INT
    loginfo_t *loginfo = new loginfo_t[num_of_functions];           // armazena contadores de comparações e trocas (ver typedef acima)

    int **array = new int *[num_of_functions]; // array dinâmico que armazena os números
    for (int i = 0; i < num_of_functions; ++i)
        array[i] = new int[array_size]; // aloca espaço para cada função de ordenação

    // testar com as 3 versões de array (aleatório, crescente e decrescente):
    if (type == 1)
        for (auto i = 0; i < array_size; i++)
            array[0][i] = distrib(rng); // gera números aleatórios para o array
    else if (type == 2)
        for (auto i = 0; i < array_size; i++)
            array[0][i] = array_size - i; // gera números em ordem decrescente
    else
        for (auto i = 0; i < array_size; i++)
            array[0][i] = i + 1; // gera números em ordem crescente

    for (auto i = 0; i < num_of_functions; i++)
        memmove(array[i], array[0], array_size * sizeof(int)); // copia array gerado para os outros arrays

    if (display)
    {
        std::cout << std::endl
                  << "Array gerado: ";
        for (auto i = 0; i < array_size; i++)
            std::cout << array[0][i] << " "; // exibe array gerado
        std::cout << std::endl;
    }

    for (auto i = 0; i < num_of_functions; i++)
        tests(sortFunctions, i, array[i], array_size, loginfo[i]); // executa as funções de ordenação

    if (display)
    {
        for (auto j = 0; j < num_of_functions; j++)
        {
            std::cout << std::endl
                      << "Array ordenado " << std::get<1>(sortFunctions[j]) << ": "; // exibe array ordenado
            for (auto i = 0; i < array_size; i++)
                std::cout << array[j][i] << " ";
            std::cout << std::endl;
        }
    }

    for (auto i = 0; i < num_of_functions; i++)
        delete[] array[i];
    delete[] array; 

    for (auto i = 0; i < num_of_functions; i++)
        log[i] = loginfo[i]; // armazena contadores de comparações e trocas
}

// Executa os testes de ordenação, com o numero de passadas especificado
// e o tamanho inicial do array, o qual é multiplicado por 100 toda a passada
// e o numero de funções de ordenação a serem testadas, as quais sao especificadas em sortFunctions
void runBatchTests(int NumberOfPasses, int NumberOfFunctionsToTest, array_size_t InitialArraySize, SortFunctions_t sortFunctions[])
{
    bool show = true; // Mostra a primeira passada dos algoritmos

    for (auto k = 0; k < NumberOfPasses; k++)
    {
        std::cout << std::endl
                  << "Tamanho do Array testado: " << InitialArraySize << std::endl; // exibe tamanho do array

        loginfo_t log[3][NumberOfFunctionsToTest]; // armazena contadores de comparações e trocas (ver typedef acima)

        for (auto i = 0; i < 3; i++)
            runs(sortFunctions, NumberOfFunctionsToTest, InitialArraySize, i, show, log[i]); // executa as funções de ordenação

        for (auto i = 0; i < 3; i++)
        {
            std::cout << std::endl 
                      << "\tTeste " << testNames[i] << ":" << std::endl; // exibe tipo de teste
            for (auto j = 0; j < NumberOfFunctionsToTest; j++)
                displayArray(std::get<1>(sortFunctions[j]), log[i][j]); // exibe contadores de comparações e trocas
        }

        InitialArraySize *= 10; // multiplica tamanho do array por 10
        show = false; // não mostra mais a primeira passada dos algoritmos
    }
}