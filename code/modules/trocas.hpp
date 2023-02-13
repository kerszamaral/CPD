#pragma once
#include "common.hpp"
#include <stack>

namespace sort
{
    class Trocas
    {
    private:
        static void quickSort(array_t &array, int i, int f, loginfo_t &loginfo)
        {
            int p;
            if (f > i)
            {
                p = particiona(array, i, f, loginfo);
                quickSort(array, i, p - 1, loginfo);
                quickSort(array, p + 1, f, loginfo);
            }
        }

        static int particiona(array_t array, int left, int right, loginfo_t &loginfo)
        {
            int i = left + 1, j = right, pivo = left;
            while (j > i)
            {
                while (array[i] < array[pivo] && i < right)
                {
                    i++;
                    std::get<1>(loginfo)++;
                }
                while (array[j] >= array[pivo] && j > left)
                {
                    j--;
                    std::get<1>(loginfo)++;
                }

                if (i < j && array[i] > array[j])
                {
                    swap(&array[i], &array[j]);
                    std::get<0>(loginfo)++;
                    std::get<1>(loginfo)++;
                }
            }
            // get<1>(loginfo)++;
            if (array[j] < array[pivo])
            {
                swap(&array[pivo], &array[j]);
                std::get<0>(loginfo)++;
                std::get<1>(loginfo)++;
            }
            return j;
        }

    public:
        static void bubblesort(array_t &array, loginfo_t &loginfo)
        {
            int pos_troca = 0;
            bool troca = true;
            int qtd_elementos = array.size() - 1;

            while (troca)
            {
                troca = false;
                for (auto i = 0; i < qtd_elementos; i++)
                {
                    std::get<1>(loginfo)++;
                    if (array[i] > array[i + 1])
                    {
                        swap(&array[i], &array[i + 1]);
                        troca = true;
                        pos_troca = i;
                        std::get<0>(loginfo)++;
                    }
                }
                qtd_elementos = pos_troca;
            }
        }

        static void quicksort(array_t &array, loginfo_t &loginfo)
        {
            quickSort(array, 0, array.size() - 1, loginfo); // passa início e fim do trecho de processamento (MAX-1)
        }

        static void combsort(array_t &array, loginfo_t &loginfo)
        {
            bool troca = true;
            int qtd_elementos = array.size() - 1;
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
                    std::get<1>(loginfo)++;
                    if (array[i] > array[i + gap])
                    {
                        swap(&array[i], &array[i + gap]);
                        troca = true;
                        std::get<0>(loginfo)++;
                    }
                }
            }
        }

        static void shakesort(array_t &array, loginfo_t &loginfo)
        {
            int pos_troca = 0;
            bool troca = true;
            int qtd_elementos = array.size() - 1;

            while (troca)
            {
                troca = false;
                for (auto i = 0; i < qtd_elementos; i++)
                {
                    std::get<1>(loginfo)++;
                    if (array[i] > array[i + 1])
                    {
                        swap(&array[i], &array[i + 1]);
                        troca = true;
                        pos_troca = i;
                        std::get<0>(loginfo)++;
                    }
                }
                qtd_elementos = pos_troca;
                for (auto i = qtd_elementos; i > 0; i--)
                {
                    std::get<1>(loginfo)++;
                    if (array[i] < array[i - 1])
                    {
                        swap(&array[i], &array[i - 1]);
                        troca = true;
                        pos_troca = i;
                        std::get<0>(loginfo)++;
                    }
                }
            }
        }

        static void quicksortIter(array_t &array, loginfo_t &loginfo)
        {
            for (long long unsigned int i = 1; i < array.size(); i++) // Verifica se o array já está ordenado
            {
                std::get<1>(loginfo)++; // Incrementa a contagem de comparações
                if (array[i - 1] > array[i])
                {
                    break; // Se não estiver, sai do loop
                }

                if (i == array.size() - 1)
                {
                    return; // Se estiver, retorna
                }
            }

            int p, i, f;                  // Declara as variáveis
            std::stack<int> pilha;             // Cria a pilha
            pilha.push(0);                // Empilha o primeiro elemento
            pilha.push(array.size() - 1); // Empilha o último elemento
            while (!pilha.empty())        // Enquanto a pilha não estiver vazia
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
            };
        }
    };
}