#include "ian.hpp"

#include <random>
#include <climits>
#include <cstring>
#include <iomanip>
#include <fstream>
#define CHARWIDTH 14 // largura de cada coluna

typedef std::mt19937 MyRNG; // Gerador de números aleatórios do tipo Mersenne Twister Random Generator

MyRNG rng;                                                           // gerador de números aleatórios
uint32_t seed_val;                                                   // semente de geração de números
std::string testNames[] = {"Crescente", "Aleatorio", "Decrescente"}; // nomes dos testes

void displayFuncStats(std::ostream &output, SortFunctions_t sortFunctions[], loginfo_t loginfo[], int tests, std::string testName, array_size_t arraySize)
{
    output << std::endl
           << "Teste " << testName << " de tamanho " << arraySize << ":" << std::endl; // exibe tipo de teste
    output << std::setw(CHARWIDTH) << "Nome da funcao"
           << " | " << std::setw(CHARWIDTH) << "Trocas"
           << " | " << std::setw(CHARWIDTH) << "Comparacoes"
           << " | " << std::setw(CHARWIDTH) << "Tempo" << std::endl;
    for (auto i = 0; i < tests; i++)
    {
        output
            << std::setw(CHARWIDTH) << std::get<1>(sortFunctions[i])
            << " | " << std::setw(CHARWIDTH) << (float)std::get<0>(loginfo[i])
            << " | " << std::setw(CHARWIDTH) << (float)std::get<1>(loginfo[i])
            << " | " << std::setw(CHARWIDTH) << std::get<2>(loginfo[i]).count() << "s" << std::endl;
    }
}

void displayFuncStatsCSV(std::ostream &output, SortFunctions_t sortFunctions[], loginfo_t loginfo[], int tests, std::string testName, array_size_t arraySize)
{
    output << std::endl
           << testName << ";Tamanho " << arraySize << std::endl; // exibe tipo de teste
    output << "Nome da funcao"
           << ";"
           << "Trocas"
           << ";"
           << "Comparacoes"
           << ";"
           << "Tempo" << std::endl;
    for (auto i = 0; i < tests; i++)
    {
        output << std::get<1>(sortFunctions[i])
               << ";" << std::get<0>(loginfo[i])
               << ";" << std::get<1>(loginfo[i])
               << ";" << std::get<2>(loginfo[i]).count() << "s" << std::endl;
    }
}

void tests(SortFunctions_t sortFunctions[], int type, array_t array, array_size_t array_size, loginfo_t &loginfo)
{
    auto start = std::chrono::high_resolution_clock::now();       // inicia timer
    std::get<0>(sortFunctions[type])(array, array_size, loginfo); // executa função de ordenação
    auto end = std::chrono::high_resolution_clock::now();         // finaliza timer
    std::get<2>(loginfo) = end - start;                           // armazena tempo de execução
}

void runs(SortFunctions_t sortFunctions[], int num_of_functions, array_size_t array_size, int type, bool display, loginfo_t *log)
{
    rng.seed(seed_val); // inicializa semente de geração de números aleatórios

    std::uniform_int_distribution<> distrib(0, INT_MAX);  // cria gerador com distribuição uniforme entre 0 e MAX_INT
    loginfo_t *loginfo = new loginfo_t[num_of_functions]; // armazena contadores de comparações e trocas (ver typedef acima)

    int **array = new int *[num_of_functions]; // array dinâmico que armazena os números
    for (int i = 0; i < num_of_functions; ++i)
        array[i] = new int[array_size]; // aloca espaço para cada função de ordenação

    // testar com as 3 versões de array (aleatório, crescente e decrescente):
    switch (type)
    {
    case 0:
        for (auto i = 0; i < array_size; i++)
            array[0][i] = i + 1; // gera números em ordem crescente
        break;
    case 1:
        for (auto i = 0; i < array_size; i++)
            array[0][i] = distrib(rng); // gera números aleatórios para o array
        break;
    case 2:
        for (auto i = 0; i < array_size; i++)
            array[0][i] = array_size - i; // gera números em ordem decrescente
        break;
    default:
        std::cout << "Tipo de array inválido" << std::endl;
        return;
    }

#pragma omp parallel for
    for (auto i = 0; i < num_of_functions; i++)
        memmove(array[i], array[0], array_size * sizeof(int)); // copia array gerado para os outros arrays

#pragma omp parallel for
    for (auto i = 0; i < num_of_functions; i++)
        tests(sortFunctions, i, array[i], array_size, loginfo[i]); // executa as funções de ordenação

    if (display && type != 1 && array_size <= 100)
    {
        for (auto j = 0; j < num_of_functions; j++)
        {
            std::cout << std::endl
                      << "Array " << testNames[type] << " Ordenado pelo " << std::get<1>(sortFunctions[j]) << ":" << std::endl; // exibe array ordenado
            for (auto i = 0; i < array_size; i++)
                std::cout << array[j][i] << " ";
            std::cout << std::endl;
        }
    }

#pragma omp parallel for
    for (auto i = 0; i < num_of_functions; i++)
        delete[] array[i];
    delete[] array;

#pragma omp parallel for
    for (auto i = 0; i < num_of_functions; i++)
        log[i] = loginfo[i]; // armazena contadores de comparações e trocas
}

// Executa os testes de ordenação, com o numero de passadas especificado
// e o tamanho inicial do array, o qual é multiplicado por 10 toda a passada
// e o numero de funções de ordenação a serem testadas, as quais sao especificadas em sortFunctions
void runBatchTests(bool automatic, int NumberOfPasses, int NumberOfFunctionsToTest, array_size_t InitialArraySize, bool ShowFirstArraySize, SortFunctions_t sortFunctions[])
{
    int arraySizes[NumberOfPasses] = {InitialArraySize}; // armazena o tamanho dos arrays de cada passada
    for (auto i = 1; i < NumberOfPasses; i++)
        arraySizes[i] = arraySizes[i - 1] * 10; // calcula o tamanho dos arrays de cada passada

    loginfo_t log[NumberOfPasses][3][NumberOfFunctionsToTest]; // armazena contadores de comparações e trocas (ver typedef acima)

#pragma omp parallel for
    for (auto k = 0; k < NumberOfPasses; k++)
    {
#pragma omp parallel for
        for (auto i = 0; i < 3; i++)
            runs(sortFunctions, NumberOfFunctionsToTest, arraySizes[k], i, ShowFirstArraySize, log[k][i]); // executa as funções de ordenação
        ShowFirstArraySize = false;                                                                        // não mostra mais a primeira passada dos algoritmos
    }

    int x = 0;

    if (!automatic)
    {
        std::cout << std::endl
                  << "Entre com o modo de saida" << std::endl
                  << "1 - Saida Terminal" << std::endl
                  << "2 - Saida Arquivo" << std::endl
                  << "3 - Saida CSV" << std::endl;

        std::cin >> x; // espera o usuário pressionar enter para exibir os resultados
    }
    else
    {
        x = 1;
    }

    if (x == 1)
    {
        for (auto k = 0; k < NumberOfPasses; k++)
            for (auto i = 0; i < 3; i++)
                displayFuncStats(std::cout, sortFunctions, log[k][i], NumberOfFunctionsToTest, testNames[i], arraySizes[k]); // exibe contadores de comparações e trocas
    }
    else if (x == 2)
    {
        std::ofstream myFile;
        myFile.open("saida.txt");
        for (auto k = 0; k < NumberOfPasses; k++)
            for (auto i = 0; i < 3; i++)
                displayFuncStats(myFile, sortFunctions, log[k][i], NumberOfFunctionsToTest, testNames[i], arraySizes[k]); // exibe contadores de comparações e trocas
        myFile.close();
    }
    else if (x == 3)
    {
        std::ofstream myFile;
        myFile.open("saida.csv");
        for (auto k = 0; k < NumberOfPasses; k++)
            for (auto i = 0; i < 3; i++)
                displayFuncStatsCSV(myFile, sortFunctions, log[k][i], NumberOfFunctionsToTest, testNames[i], arraySizes[k]);
        myFile.close();
    }
    else
    {
        std::cout << "Modo de saida invalido" << std::endl;
    }

    if (!automatic)
    {
        std::cout << std::endl
                  << "Dados prontos! Pressione enter para sair" << std::endl;
        std::cin.ignore();
        std::cin.get();
    }
}