/*
Ian Kersz Amaral -- 00338368
UFRGS 2022
Header para os trabalhos de laboratório de CPD
*/
#pragma once

#include <iostream>
#include <tuple>
#include <chrono>

typedef int array_size_t;                                        // Tipo para especificar tamanho do array
typedef int *array_t;                                            // Tipo para especificar formato do array
typedef std::chrono::duration<double> timer;                     // Tipo para especificar o timer do programa
typedef std::tuple<int, int, timer> loginfo_t;                   // armazena contagem de <trocas , comparacoes, tempo de execucao>
typedef void (*Functions_t)(array_t, array_size_t, loginfo_t &); // Tipo para especificar funções de ordenação
typedef std::tuple<Functions_t, std::string> SortFunctions_t;    // Tipo para especificar funções de ordenação e seus nomes

// Executa os testes de ordenação, com o numero de passadas especificado
// e o tamanho inicial do array, o qual é multiplicado por 100 toda a passada
// e o numero de funções de ordenação a serem testadas, as quais sao especificadas em sortFunctions
void runBatchTests(int NumberOfPasses, int NumberOfFunctionsToTest, array_size_t InitialArraySize, bool ShowFirstArraySize, SortFunctions_t sortFunctions[]);