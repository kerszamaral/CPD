#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <random>
#include <cstring>
#include <iomanip>
#include <fstream>

namespace sort
{
    // Definições e variáveis globais necessárias
    typedef int element_t;                          // tipo do elemento
    typedef size_t array_size_t;                    // tipo do tamanho do array
    typedef std::vector<element_t> array_t;         // Tipo para especificar formato do array
    typedef std::tuple<size_t, size_t> loginfo_t; // armazena informações de desempenho <trocas, comparações, tempo em ms>
    typedef void (*sortfunc_t)(array_t &, loginfo_t &);

    static void swap(element_t *n1, element_t *n2)
    {
        auto troca = *n1;
        *n1 = *n2;
        *n2 = troca;
    }

    class heap
    {
    public:
        inline const static element_t filho_e(const array_t &array, element_t elemento) { return elemento * 2 + 1; }

        inline const static element_t filho_d(const array_t &array, element_t elemento) { return elemento * 2 + 2; }

        inline const static element_t pai(const array_t &array, element_t elemento) { return (elemento / 2); }
        
        inline const static element_t heap_max(const array_t &array, loginfo_t &loginfo) { return array[0]; }

        // heapify: verifica se o elemento na posição passada é um heap e se não for transforma-o em um
        static void heapify(array_t &array, element_t elemento, int heap_size, loginfo_t &loginfo)
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
                swap(&array[maior], &array[elemento]);
                heapify(array, maior, heap_size, loginfo);
            }
        }

        static void buildheap(array_t &array, loginfo_t &loginfo)
        {
            int ultimo_pai = (array.size() / 2) - 1;
            for (auto i = ultimo_pai; i >= 0; i--)
                heapify(array, i, array.size(), loginfo);
        }

        static element_t extract_max(array_t &array, loginfo_t &loginfo)
        {
            if (array.size() < 1)
                return -1;

            auto max = array[0];
            array[0] = array[array.size() - 1];
            array.resize(array.size() - 1);
            heapify(array, 0, array.size(), loginfo);
            return max;
        }

        static void heap_insert(array_t &array, element_t elemento, loginfo_t &loginfo)
        {
            array.resize(array.size() + 1);
            auto i = array.size();
            while (i > 0 && array[pai(array, i)] < elemento)
            {
                array[i] = array[pai(array, i)];
                i = pai(array, i);
            }
            array[i] = elemento;
        }
    };
}