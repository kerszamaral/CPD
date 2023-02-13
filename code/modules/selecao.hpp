#pragma once
#include "common.hpp"

namespace sort
{
    class Selecao
    {
    public:
        static void selectionsort(array_t &array, loginfo_t &loginfo)
        {
            int qtd_elementos = array.size() - 1;

            for (auto i = 0; i < qtd_elementos; i++)
            {
                auto menorchave = i;
                for (auto j = i; j < qtd_elementos + 1; j++)
                {
                    std::get<1>(loginfo)++;
                    if (array[j] < array[menorchave])
                    {
                        menorchave = j;
                    }
                }
                if (menorchave != i)
                {
                    swap(&array[i], &array[menorchave]);
                    std::get<0>(loginfo)++;
                }
            }
        }

        static void heapsort(array_t &array, loginfo_t &loginfo)
        {
            int heap_size = array.size();
            int qtd_elementos = heap_size - 1;

            heap::buildheap(array, loginfo);

            for (auto i = qtd_elementos; i > 0; i--)
            {
                std::get<0>(loginfo)++;
                swap(&array[i], &array[0]);
                heap_size--;
                heap::heapify(array, 0, heap_size, loginfo);
            }
        }
    };
}
