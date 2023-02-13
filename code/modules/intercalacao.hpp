#pragma once
#include "common.hpp"

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
            array_final.clear();

            // multi way merge
            while (true)
            {
                long long unsigned int i = 0;
                for (auto array : arrays)
                {
                    if (array.empty())
                    {
                        i++;
                        break;
                    }
                    array_final.emplace_back(array[0]);
                    array.erase(array.begin());
                }

                if (i == arrays.size())
                {
                    break;
                }
            }

            heap::buildheap(array_final, loginfo);
            // TODO: atualizar loginfo
        }

        static void mergesort(array_t &array, loginfo_t &loginfo)
        {
            mergeSort(array, 0, array.size() - 1, loginfo);
        }
    };
}