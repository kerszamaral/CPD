#pragma once
#include "common.hpp"
#include <limits.h>

namespace sort
{
    class Intercalacao
    {
    private:
        static void intercala(int array[], int left, int mid, int right, loginfo_t &loginfo)
        {
            int i = left, j = mid, k = 0, qtelem = right - left;
            int *aux = (int *)malloc((qtelem) * sizeof(int));

            while (i < mid && j < right)
            {
                if (array[i] <= array[j])
                {
                    aux[k++] = array[i++];
                }
                else
                {
                    aux[k++] = array[j++];
                }
            }

            while (i < mid)
            {
                aux[k++] = array[i++];
            }

            while (j < right)
            {
                aux[k++] = array[j++];
            }

            // copia do vetor auxiliar para o original
            memcpy((void *)&array[left], (void *)aux, qtelem * sizeof(int));

            free(aux);
        }

        static void mergeSort(array_t &array, int const begin, int const end, loginfo_t &loginfo)
        {
            if (begin >= end)
                return; // Returns recursively

            auto mid = (begin - end) / 2;
            mergeSort(array, begin, mid, loginfo);
            mergeSort(array, mid, end, loginfo);
            intercala(array.data(), begin, mid, end, loginfo);
        }

        inline const static element_t filho_e(element_t elemento) { return elemento * 2 + 1; }

        inline const static element_t filho_d(element_t elemento) { return elemento * 2 + 2; }

        inline const static element_t pai(element_t elemento) { return (elemento / 2); }

        typedef std::pair<int, int> heap_element_t;

        static void heapifyMin(std::vector<heap_element_t> &array, element_t elemento, int heap_size, loginfo_t &loginfo)
        {
            auto e = filho_e(elemento);
            auto d = filho_d(elemento);
            auto menor = elemento;

            if (e < heap_size && std::get<0>(array[e]) < std::get<0>(array[menor]))
                menor = e;

            if (d < heap_size && std::get<0>(array[d]) < std::get<0>(array[menor]))
                menor = d;

            if (menor != elemento)
            {
                std::swap(array[menor], array[elemento]);
                heapifyMin(array, menor, heap_size, loginfo);
            }
        }

        static void buildheapMin(std::vector<heap_element_t> &array, loginfo_t &loginfo)
        {
            int ultimo_pai = (array.size() / 2) - 1;         // ultimo pai
            for (auto i = ultimo_pai; i >= 0; i--)           // percorre os pais
                heapifyMin(array, i, array.size(), loginfo); // transforma o pai em heap
        }

    public:
        static void merge(const array_t &array1, const array_t &array2, array_t &array_final, loginfo_t &loginfo)
        {
            int i = 0, j = 0;
            int qtd_a1 = array1.size();
            int qtd_a2 = array2.size();

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

        static void two_way_merge(const std::vector<array_t> arrays, array_t &array_final, loginfo_t &loginfo)
        {
            array_final.clear();

            for (long long unsigned int i = 0; i < arrays.size(); i += 2)
            {
                array_t array1 = arrays[i];
                array_t array2 = arrays[i + 1];
                array_t array_final_temp;
                merge(array1, array2, array_final_temp, loginfo);
                array_final = array_final_temp;
            }
            // TODO: atualizar loginfo
        }

        static void multi_way_merge(const std::vector<array_t> arrays, array_t &array_final, loginfo_t &loginfo)
        {
            array_final.clear(); // limpa o array final

            // Cria um heap para fazer a intercalacao, com uma tupla de <valor, posicao, array>
            std::vector<heap_element_t> heap(arrays.size(), std::make_pair(-1, -1));
            std::vector<int> indexes(arrays.size(), 0); // vetor de indices

            // multi way merge
            while (true)
            {
                size_t j = 0; // contador de arrays vazios

                for (size_t i = 0; i < arrays.size(); i++)
                {
                    auto value = INT_MAX;
                    if (indexes[i] == -1) // se o array estiver vazio
                    {
                        j++; // incrementa o contador de arrays vazios
                    }
                    else
                    {
                        value = arrays[i][indexes[i]];
                    }

                    heap[i] = std::make_pair(value, i); // insere o valor no heap
                }

                if (j == arrays.size()) // se todos os arrays estiverem vazios
                {
                    break; // sai do loop
                }

                buildheapMin(heap, loginfo); // constroi o heap

                auto &orgArray = heap[0].second; // pega o array original

                array_final.emplace_back(heap[0].first); // insere o menor valor no array final

                if (indexes[orgArray] == (int)arrays[orgArray].size() - 1)
                {
                    indexes[orgArray] = -1; // marca o array como vazio
                }
                else
                {
                    indexes[orgArray]++; // incrementa a posicao do array
                }
            }
            // TODO: atualizar loginfo
        }

        static void mergesort(array_t &array, loginfo_t &loginfo)
        {
            mergeSort(array, 0, array.size() - 1, loginfo);
        }
    };
}