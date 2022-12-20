#include "cpd.hpp"

#include <random>
#include <climits>
#include <cstring>
#include <iomanip>
#include <fstream>
#define CHARWIDTH 14 // largura de cada coluna

namespace cpd
{
    typedef std::mt19937 MyRNG; // Gerador de números aleatórios do tipo Mersenne Twister Random Generator
    typedef int testtype_t;       // Tipo para especificar o tipo de teste a ser executado
    typedef int function_t;       // Tipo para especificar a função de ordenação a ser executada

    MyRNG rng;                                                           // gerador de números aleatórios
    uint32_t seed_val;                                                   // semente de geração de números
    std::string testNames[] = {"Crescente", "Aleatorio", "Decrescente"}; // nomes dos testes

    void displayFuncStats(std::ostream &output, SortFunctions_t sortFunctions[], loginfo_t loginfo[], size_t numTest, std::string testName, array_size_t S)
    {
        output << std::endl
               << "Teste " << testName << " de tamanho " << S << ":" << std::endl; // exibe tipo de teste
        output << std::setw(CHARWIDTH) << "Nome da funcao"
               << " | " << std::setw(CHARWIDTH) << "Trocas"
               << " | " << std::setw(CHARWIDTH) << "Comparacoes"
               << " | " << std::setw(CHARWIDTH) << "Tempo" << std::endl;
        for (size_t i = 0; i < numTest; i++)
        {
            output
                << std::setw(CHARWIDTH) << std::get<1>(sortFunctions[i])
                << " | " << std::setw(CHARWIDTH) << (float)std::get<0>(loginfo[i])
                << " | " << std::setw(CHARWIDTH) << (float)std::get<1>(loginfo[i])
                << " | " << std::setw(CHARWIDTH) << std::get<2>(loginfo[i]).count() << "s" << std::endl;
        }
    }

    void displayFuncStatsCSV(std::ostream &output, SortFunctions_t sortFunctions[], loginfo_t loginfo[], size_t numTest, std::string testName, array_size_t S)
    {
        output << std::endl
               << testName << ";Tamanho " << S << std::endl; // exibe tipo de teste
        output << "Nome da funcao"
               << ";"
               << "Trocas"
               << ";"
               << "Comparacoes"
               << ";"
               << "Tempo" << std::endl;
        for (size_t i = 0; i < numTest; i++)
        {
            output << std::get<1>(sortFunctions[i])
                   << ";" << std::get<0>(loginfo[i])
                   << ";" << std::get<1>(loginfo[i])
                   << ";" << std::get<2>(loginfo[i]).count() << "s" << std::endl;
        }
    }

    void tests(SortFunctions_t sortFunctions[], function_t T, array_t A, array_size_t S, loginfo_t &loginfo)
    {
        auto start = std::chrono::high_resolution_clock::now();       // inicia timer
        std::get<0>(sortFunctions[T])(A, S, loginfo); // executa função de ordenação
        auto end = std::chrono::high_resolution_clock::now();         // finaliza timer
        std::get<2>(loginfo) = end - start;                           // armazena tempo de execução
    }

    void runs(SortFunctions_t sortFunctions[], size_t FunctionsToTest, array_size_t S, testtype_t T, loginfo_t *log)
    {
        rng.seed(seed_val);                                  // inicializa semente de geração de números aleatórios
        std::uniform_int_distribution<> distrib(0, INT_MAX); // cria gerador com distribuição uniforme entre 0 e MAX_INT

        int **array = new int *[FunctionsToTest]; // array dinâmico que armazena os números
        for (size_t i = 0; i < FunctionsToTest; ++i)
            array[i] = new int[S]; // aloca espaço para cada função de ordenação

        // testar com as 3 versões de array (aleatório, crescente e decrescente):
        switch (T)
        {
        case 0:
            for (auto i = 0; i < S; i++)
                array[0][i] = i + 1; // gera números em ordem crescente
            break;
        case 1:
            for (auto i = 0; i < S; i++)
                array[0][i] = distrib(rng); // gera números aleatórios para o array
            break;
        case 2:
            for (auto i = 0; i < S; i++)
                array[0][i] = S - i; // gera números em ordem decrescente
            break;
        default:
            std::cout << "Tipo de array inválido" << std::endl;
            return;
        }

#pragma omp parallel for
        for (size_t i = 0; i < FunctionsToTest; i++)
            memmove(array[i], array[0], S * sizeof(int)); // copia array gerado para os outros arrays

#pragma omp parallel for
        for (size_t i = 0; i < FunctionsToTest; i++)
            tests(sortFunctions, i, array[i], S, log[i]); // executa as funções de ordenação

        if (T != 1 && S <= 100)
        {
            for (size_t j = 0; j < FunctionsToTest; j++)
            {
                std::cout << std::endl
                          << "Array " << testNames[T] << " Ordenado pelo " << std::get<1>(sortFunctions[j]) << ":" << std::endl; // exibe array ordenado
                for (auto i = 0; i < S; i++)
                    std::cout << array[j][i] << " ";
                std::cout << std::endl;
            }
        }

        for (size_t i = 0; i < FunctionsToTest; i++)
            delete[] array[i];
        delete[] array;
    }

    // Executa os testes de ordenação, com o numero de passadas especificado
    // e o tamanho inicial do array, o qual é multiplicado por 10 toda a passada
    // e o numero de funções de ordenação a serem testadas, as quais sao especificadas em sortFunctions
    void runBatchTests(bool automatic, size_t Passes, SortFunctions_t Sorts[], size_t FunctionsToTest, array_size_t Initial)
    {
        int *arraySizes = new int[Passes]; // armazena o tamanho dos arrays de cada passada
        arraySizes[0] = Initial;
        loginfo_t ***log = new loginfo_t **[Passes]; // armazena contadores de comparações e trocas (ver typedef acima)

        for (size_t i = 0; i < Passes; i++)
        {
            if (i > 0)
                arraySizes[i] = arraySizes[i - 1] * 10; // calcula o tamanho dos arrays de cada passada

            log[i] = new loginfo_t *[3]; // aloca espaço para cada função de ordenação
            for (int j = 0; j < 3; ++j)
                log[i][j] = new loginfo_t[FunctionsToTest]; // aloca espaço para cada função de ordenação
        }

#pragma omp parallel for
        for (size_t i = 0; i < Passes; i++)
        {
#pragma omp parallel for
            for (size_t j = 0; j < 3; j++)
                runs(Sorts, FunctionsToTest, arraySizes[i], j, log[i][j]); // executa as funções de ordenação                                                                  // não mostra mais a primeira passada dos algoritmos
        }

        int outputMode = 1;

        if (!automatic)
        {
            std::cout << std::endl
                      << "1 - Saida Terminal" << std::endl
                      << "2 - Saida Arquivo" << std::endl
                      << "3 - Saida CSV" << std::endl
                      << "Entre com o modo de saida: ";

            std::cin >> outputMode;
        }

        switch (outputMode)
        {
        case 1:
            for (size_t k = 0; k < Passes; k++)
                for (size_t i = 0; i < 3; i++)
                    displayFuncStats(std::cout, Sorts, log[k][i], FunctionsToTest, testNames[i], arraySizes[k]); // exibe contadores de comparações e trocas
            break;
        case 2:
        {
            std::ofstream myFile;
            myFile.open("saida.txt");
            for (size_t k = 0; k < Passes; k++)
                for (size_t i = 0; i < 3; i++)
                    displayFuncStats(myFile, Sorts, log[k][i], FunctionsToTest, testNames[i], arraySizes[k]);
            myFile.close();
            break;
        }
        case 3:
        {
            std::ofstream myFile;
            myFile.open("saida.csv");
            for (size_t k = 0; k < Passes; k++)
                for (size_t i = 0; i < 3; i++)
                    displayFuncStatsCSV(myFile, Sorts, log[k][i], FunctionsToTest, testNames[i], arraySizes[k]);
            myFile.close();
            break;
        }
        default:
            std::cout << "Modo de saida invalido" << std::endl;
            break;
        }

        if (!automatic)
        {
            std::cout << std::endl
                      << "Dados prontos! Pressione enter para sair";
            std::cin.ignore();
            std::cin.get();
        }

        delete[] arraySizes;
        for (size_t i = 0; i < Passes; i++)
        {
            for (int j = 0; j < 3; ++j)
                delete[] log[i][j];
            delete[] log[i];
        }
        delete[] log;
    }
}