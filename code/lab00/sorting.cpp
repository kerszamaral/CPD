#include "../cpd.hpp"

#include <random>

using namespace std;

typedef int array_size_t;                                        // Tipo para especificar tamanho do array
typedef int *array_t;                                            // Tipo para especificar formato do array
typedef std::tuple<size_t, size_t> loginfo_t;                    // armazena contagem de <trocas , comparacoes>

void bubblesort(array_t, array_size_t, loginfo_t &);
void quicksortPoint(array_t, array_size_t, loginfo_t &);
void quicksort(array_t, int, int, loginfo_t &);
int particiona(array_t array, int i, int f, loginfo_t &);
void swap(int *n1, int *n2);
void combsort(array_t array, array_size_t array_size, loginfo_t &loginfo);
void shakesort(array_t array, array_size_t array_size, loginfo_t &loginfo);

#define RUNS 1 // quantidade de vezes que cada teste será executado
typedef void (*Functions_t)(array_t, array_size_t, loginfo_t &); // Tipo para especificar funções de ordenação

template <>
void cpd::Tester<Functions_t, loginfo_t>::TestFunction(Functions_t Function, array_t &Array, loginfo_t &loginfo)
{
    auto array = Array.data();
    auto ASize = Array.size();
    Function(array, ASize, loginfo);
}

template <>
void cpd::Tester<Functions_t, loginfo_t>::DisplayLogNames(std::ostream &Output, const std::string &Separator)
{
    Output << Separator << std::setw(Spacer) << "Trocas"
           << Separator << std::setw(Spacer) << "Comparacoes";
}

main(void)
{
    cout << __cplusplus << endl;                                             // verifica versão do compilador

    cpd::Tester<Functions_t, loginfo_t> tester = cpd::Tester<Functions_t, loginfo_t>(
        {bubblesort, quicksortPoint, shakesort, combsort},
        {"Bubblesort", "Quicksort", "Shakesort", "Combsort"}); // instancia o objeto de teste

    tester.BatchTests(1, 1, 100); // executa os testes (automatico, funções de ordenação, tamanho inicial do array

    return 0;
}

void quicksortPoint(array_t array, array_size_t array_size, loginfo_t &loginfo)
{
    quicksort(array, 0, array_size - 1, loginfo); // passa início e fim do trecho de processamento (MAX-1)
}

// Função de quicksort
void quicksort(array_t array, int i, int f, loginfo_t &loginfo)
{
    int p;
    if (f > i)
    {
        p = particiona(array, i, f, loginfo);
        quicksort(array, i, p - 1, loginfo);
        quicksort(array, p + 1, f, loginfo);
    }
}

int particiona(array_t array, int esq, int dir, loginfo_t &loginfo)
{
    int i = esq + 1, j = dir, pivo = esq;
    while (j > i)
    {
        while (array[i] < array[pivo] && i < dir)
        {
            i++;
            get<1>(loginfo)++;
        }
        while (array[j] >= array[pivo] && j > esq)
        {
            j--;
            get<1>(loginfo)++;
        }

        if (i < j && array[i] > array[j])
        {
            swap(&array[i], &array[j]);
            get<0>(loginfo)++;
            get<1>(loginfo)++;
        }
    }
    // get<1>(loginfo)++;
    if (array[j] < array[pivo])
    {
        swap(&array[pivo], &array[j]);
        get<0>(loginfo)++;
        get<1>(loginfo)++;
    }
    return j;
}

void swap(int *n1, int *n2)
{
    int troca = *n1;
    *n1 = *n2;
    *n2 = troca;
}

void bubblesort(array_t array, array_size_t array_size, loginfo_t &loginfo)
{
    int trocas = 0;
    int comparacoes = 0;
    int pos_troca = 0;
    bool troca = true;
    int qtd_elementos = array_size - 1;

    while (troca)
    {
        troca = false;
        for (auto i = 0; i < qtd_elementos; i++)
        {
            comparacoes = comparacoes + 1;
            if (array[i] > array[i + 1])
            {
                swap(array[i], array[i + 1]);
                troca = true;
                pos_troca = i;
                trocas = trocas + 1;
            }
        }
        qtd_elementos = pos_troca;
    }

    get<0>(loginfo) = trocas;
    get<1>(loginfo) = comparacoes;
}

void shakesort(array_t array, array_size_t array_size, loginfo_t &loginfo)
{
    int trocas = 0;
    int comparacoes = 0;
    int pos_troca = 0;
    bool troca = true;
    int qtd_elementos = array_size - 1;

    while (troca)
    {
        troca = false;
        for (auto i = 0; i < qtd_elementos; i++)
        {
            comparacoes++;
            if (array[i] > array[i + 1])
            {
                swap(array[i], array[i + 1]);
                troca = true;
                pos_troca = i;
                trocas++;
            }
        }
        qtd_elementos = pos_troca;
        for (auto i = qtd_elementos; i > 0; i--)
        {
            comparacoes = comparacoes + 1;
            if (array[i] < array[i - 1])
            {
                swap(array[i], array[i - 1]);
                troca = true;
                pos_troca = i;
                trocas++;
            }
        }
    }

    get<0>(loginfo) = trocas;
    get<1>(loginfo) = comparacoes;
}

void combsort(array_t array, array_size_t array_size, loginfo_t &loginfo)
{
    int trocas = 0;
    int comparacoes = 0;
    bool troca = true;
    int qtd_elementos = array_size - 1;
    int gap = qtd_elementos;

    while (troca)
    {
        gap = floor(gap / 1.3);
        if (gap < 1)
        {
            troca = false;
            gap = 1;
        }

        for (auto i = 0; i + gap <= qtd_elementos; i++)
        {
            comparacoes++;
            if (array[i] > array[i + gap])
            {
                swap(array[i], array[i + gap]);
                troca = true;
                trocas++;
            }
        }
    }

    get<0>(loginfo) = trocas;
    get<1>(loginfo) = comparacoes;
}