/*
Ian Kersz Amaral -- 00338368
UFRGS 2022
Header para os trabalhos de laboratório de CPD
*/
#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <tuple>
#include <chrono>
#include <random>
#include <climits>
#include <cstring>
#include <iomanip>
#include <fstream>
#define CHARWIDTH 14

namespace cpd
{
    typedef int array_size_t;                                        // Tipo para especificar tamanho do array
    typedef int *array_t;                                            // Tipo para especificar formato do array
    typedef std::chrono::duration<double> timer;                     // Tipo para especificar o timer do programa
    typedef std::tuple<int, int, timer> loginfo_t;                   // armazena contagem de <trocas , comparacoes, tempo de execucao>
    typedef void (*Functions_t)(array_t, array_size_t, loginfo_t &); // Tipo para especificar funções de ordenação
    typedef std::pair<Functions_t, std::string> SortFunctions_t;     // Tipo para especificar funções de ordenação e seus nomes

    SortFunctions_t pair(Functions_t func, std::string name)
    {
        return std::make_pair(func, name);
    }

    template <class Type>
    class Tester
    {
        std::mt19937 rng;                                                     // gerador de números aleatórios
        uint32_t seed_val;                                                    // semente de geração de números
        std::string testNames[3] = {"Crescente", "Aleatorio", "Decrescente"}; // nomes dos testes
        std::vector<Type> sortFunctions;                                      // array de funções de ordenação
        size_t Size;                                                          // tamanho do array de funções de ordenação

    private:
        void displayStats(std::ostream &output, std::string separator, loginfo_t loginfo[], std::string testName, array_size_t S)
        {
            output << std::endl
                   << "Teste " << testName << separator << "Tamanho " << S << ":" << std::endl // exibe tipo de teste
                   << std::setw(CHARWIDTH) << "Nome da funcao"
                   << separator << std::setw(CHARWIDTH) << "Trocas"
                   << separator << std::setw(CHARWIDTH) << "Comparacoes"
                   << separator << std::setw(CHARWIDTH) << "Tempo" << std::endl;
            for (size_t i = 0; i < Size; i++)
            {
                output
                    << std::setw(CHARWIDTH) << std::get<1>(sortFunctions[i])
                    << separator << std::setw(CHARWIDTH) << std::get<0>(loginfo[i])
                    << separator << std::setw(CHARWIDTH) << std::get<1>(loginfo[i])
                    << separator << std::setw(CHARWIDTH) << std::get<2>(loginfo[i]).count() << "s" << std::endl;
            }
        }

        void testTimer(Type sort, array_t A, array_size_t S, loginfo_t &loginfo)
        {
            auto start = std::chrono::high_resolution_clock::now(); // inicia timer
            std::get<0>(sort)(A, S, loginfo);                       // executa função de ordenação
            auto end = std::chrono::high_resolution_clock::now();   // finaliza timer
            std::get<2>(loginfo) = end - start;                     // armazena tempo de execução
        }

        void runs(array_size_t S, int T, loginfo_t *log)
        {
            std::uniform_int_distribution<> distrib(0, INT_MAX); // cria gerador com distribuição uniforme entre 0 e MAX_INT

            int **array = new int *[Size]; // array dinâmico que armazena os números
            for (size_t i = 0; i < Size; ++i)
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
                    array[0][i] = distrib(Tester::rng); // gera números aleatórios para o array
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
            for (size_t i = 0; i < Size; i++)
                std::memmove(array[i], array[0], S * sizeof(int)); // copia array gerado para os outros arrays

#pragma omp parallel for
            for (size_t i = 0; i < Size; i++)
                testTimer(sortFunctions[i], array[i], S, log[i]); // executa as funções de ordenação

            if (T != 1 && S <= 100)
            {
                for (size_t j = 0; j < Size; j++)
                {
                    std::cout << std::endl
                              << "Array " << Tester::testNames[T] << " Ordenado pelo " << std::get<1>(sortFunctions[j]) << ":" << std::endl; // exibe array ordenado
                    for (auto i = 0; i < S; i++)
                        std::cout << array[j][i] << " ";
                    std::cout << std::endl;
                }
            }

            for (size_t i = 0; i < Size; i++)
                delete[] array[i];
            delete[] array;
        }

    public:
        Tester(std::initializer_list<Type> functions) : Tester(std::vector<Type>(functions))
        {
            sortFunctions = functions;
            Size = functions.size();
            rng.seed(seed_val); // inicializa semente de geração de números aleatórios
        }

        Tester(std::vector<Type> functions)
        {
            sortFunctions = functions;
            Size = functions.size();
            rng.seed(seed_val); // inicializa semente de geração de números aleatórios
        }

        ~Tester()
        {
            sortFunctions.clear();
        }

        // Executa os testes de ordenação, com o numero de passadas especificado
        // e o tamanho inicial do array, o qual é multiplicado por 10 toda a passada
        // e o numero de funções de ordenação a serem testadas, as quais sao especificadas em sortFunctions
        void BatchTests(bool automatic, size_t Passes, array_size_t Initial)
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
                    log[i][j] = new loginfo_t[Size]; // aloca espaço para cada função de ordenação
            }

#pragma omp parallel for
            for (size_t i = 0; i < Passes; i++)
            {
#pragma omp parallel for
                for (size_t j = 0; j < 3; j++)
                    runs(arraySizes[i], j, log[i][j]); // executa as funções de ordenação                                                                  // não mostra mais a primeira passada dos algoritmos
            }

            int outputMode = 1;
            if (!automatic)
            {
                std::cout.flush();
                std::cout << std::endl
                          << "Testes Concluidos!" << std::endl
                          << "1 - Terminal" << std::endl
                          << "2 - TXT" << std::endl
                          << "3 - CSV" << std::endl
                          << "Entre com o modo de saida: ";

                std::cin >> outputMode;
            }

            switch (outputMode)
            {
            case 1:
            {
                for (size_t k = 0; k < Passes; k++)
                    for (size_t i = 0; i < 3; i++)
                        displayStats(std::cout, " | ", log[k][i], testNames[i], arraySizes[k]); // exibe contadores de comparações e trocas
                break;
            }
            case 2:
            {
                std::ofstream myFile;
                myFile.open("saida.txt");
                for (size_t k = 0; k < Passes; k++)
                    for (size_t i = 0; i < 3; i++)
                        displayStats(myFile, " | ", log[k][i], testNames[i], arraySizes[k]);
                myFile.close();
                break;
            }
            case 3:
            {
                std::ofstream myFile;
                myFile.open("saida.csv");
                for (size_t k = 0; k < Passes; k++)
                    for (size_t i = 0; i < 3; i++)
                        displayStats(myFile, ";", log[k][i], testNames[i], arraySizes[k]);
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
    };
} // namespace CPD