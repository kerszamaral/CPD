#pragma once
#include "common.hpp"

namespace sort
{
    class Insercao
    {
    private:
        static int busca_binaria(array_t &array, element_t elemento, int inicio, int fim, loginfo_t &loginfo)
        {
            int meio = (fim - inicio) / 2;

            // defina aqui sua versão da função de busca binária
            while (inicio <= fim)
            {
                std::get<1>(loginfo)++;
                if (array[meio] == elemento)
                {
                    break;
                }
                else if (array[meio] < elemento)
                {
                    inicio = meio + 1;
                }
                else
                {
                    fim = meio - 1;
                }
                meio = (inicio + fim) / 2;
            }
            return meio; // retorna posição do elemento e quantidade de operações
        }

    public:
        static void insertionsort(array_t &array, loginfo_t &loginfo)
        {
            for (size_t i = 1; i < array.size(); i++)
            {                          // do segundo ao último
                auto chave = array[i]; // chave a inserir no subarray ordenado
                auto j = i - 1;        // último elemento do subarray ordenado
                std::get<1>(loginfo)++;
                while (j >= 0 && array[j] > chave)
                { // busca linear da direita para a esquerda no subarray ordenado
                    std::get<1>(loginfo)++;
                    array[j + 1] = array[j];
                    j = j - 1;
                    std::get<0>(loginfo)++;
                }
                array[j + 1] = chave;
                std::get<0>(loginfo)++;
            }
        }

        static void insertionsortBinary(array_t &array, loginfo_t &loginfo)
        {
            for (size_t i = 1; i < array.size(); ++i)
            {
                size_t j = i - 1;
                int elemento = array[i];
                int local = busca_binaria(array, elemento, 0, j, loginfo);
                std::get<0>(loginfo)++;
                std::get<1>(loginfo)++;
                while (j >= local)
                {
                    std::get<0>(loginfo)++;
                    array[j + 1] = array[j];
                    j--;
                }
                array[j + 1] = elemento;
                std::get<0>(loginfo)++;
            }
        }

        static void shellsort(array_t &array, loginfo_t &loginfo)
        {
            long long unsigned int i, j;
            long long unsigned int espaco = 1;
            int valor;

            do
            {
                espaco = 3 * espaco + 1;
            } while (espaco < array.size());

            do
            {
                espaco /= 3;
                for (i = espaco; i < array.size(); i++)
                {
                    valor = array[i];
                    j = i - espaco;

                    while (j >= 0 && valor < array[j])
                    {
                        std::get<0>(loginfo)++;
                        array[j + espaco] = array[j];
                        j -= espaco;
                    }
                    std::get<1>(loginfo)++;
                    array[j + espaco] = valor;
                }
            } while (espaco > 1);
        }
    };
}