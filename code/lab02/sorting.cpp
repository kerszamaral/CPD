// Atenção: usa código C++11
// para saber se o seu compilador tem suporte, execute:
// cout << __cplusplus;
// O resultado deve ser 201103L ou maior.
// o do google collab é C++14
// A grande maioria dos compiladores atuais suporta nativamente c++11.
// Outros exigem a configuração de parâmetros de compilação... Verifique a documentação do seu.

#include <iostream>
#include <tuple>
#include <random>
#include <bits/stdc++.h>
#include <chrono>

using namespace std;

typedef int array_size_t;               // Tipo para especificar tamanho do array
typedef int *array_t;                   // Tipo para especificar formato do array
typedef std::tuple<int, int, std::chrono::duration<double>> loginfo_t; // armazena contagem de <trocas , comparacoes>
typedef std::mt19937 MyRNG;             // Gerador de números aleatórios do tipo Mersenne Twister Random Generator

MyRNG rng;         // gerador de números aleatórios
uint32_t seed_val; // semente de geração de números

void bubblesort(array_t, array_size_t, loginfo_t &);
void quicksort(array_t, int, int, loginfo_t &);
int particiona(array_t array, int i, int f, loginfo_t &);
void swap(int *n1, int *n2);
void runs(int MAX, int type, bool display, loginfo_t *log);
void displayArray(std::string &name, loginfo_t &loginfo);
void combsort(array_t array, array_size_t array_size, loginfo_t &loginfo);
void shakesort(array_t array, array_size_t array_size, loginfo_t &loginfo);

std::string testNames[] = {"Crescente", "Aleatorio", "Decrescente"};
std::string sortNames[] = {"Bubblesort", "Quicksort", "Shakesort", "Combsort"};

#define TESTS 4 // quantidade de testes a serem executados
#define RUNS 4  // quantidade de vezes que cada teste será executado

int main(void)
{
    int MAX = 50; // quantidade de números no array
    bool show = true;
    // cout << __cplusplus << endl;                                             // verifica versão do compilador
    rng.seed(seed_val); // inicializa semente de geração de números aleatórios

    for (auto k = 0; k < RUNS; k++)
    {
        cout << endl << "Tamanho do Array testado: " << MAX << endl;

        loginfo_t log[3][TESTS];

        for (auto i = 0; i < 3; i++)
            runs(MAX, i, show, log[i]);

        // TODO: armazenar essas informações em um matriz ou hashtable
        //! DONE

        for (auto i = 0; i < 3; i++)
        {
            // TODO: mostrar informações de execução de todos os algoritmos
            //! DONE
            cout << endl << "\tTeste " << testNames[i] << ":" << endl;
            for(auto j = 0; j < TESTS; j++)
                displayArray(sortNames[j], log[i][j]);
        }

        if (k == 0)
        {
            show = false;
            MAX += 50;
        }
        else
            MAX *= 10;
    }

    return 0;
}

void displayArray(std::string &name, loginfo_t &loginfo)
{
    cout << "\t\t" << name << ":" << endl;
    cout << "\t\t\tQuantidade de trocas: " << get<0>(loginfo) << endl;
    cout << "\t\t\tQuantidade de comparações: " << get<1>(loginfo) << endl;
    cout << "\t\t\tTempo de execucao: " << get<2>(loginfo).count() << "s" << endl;
}

void runs(int MAX, int type, bool display, loginfo_t *log)
{
    uniform_int_distribution<> distrib(0, INT_MAX); // cria gerador com distribuição uniforme entre 0 e MAX_INT
    loginfo_t *loginfo = new loginfo_t[TESTS];      // armazena contadores de comparações e trocas (ver typedef acima)

    int **array = new int *[TESTS]; // array dinâmico que armazena os números
    for (int i = 0; i < TESTS; ++i)
        array[i] = new int[MAX];

    // testar com as 3 versões de array (aleatório, crescente e decrescente):
    if (type == 1)
        for (auto i = 0; i < MAX; i++)
            array[0][i] = distrib(rng); // gera números aleatórios para o array
    else if (type == 2)
        for (auto i = 0; i < MAX; i++)
            array[0][i] = MAX - i; // gera números em ordem decrescente
    else
        for (auto i = 0; i < MAX; i++)
            array[0][i] = i + 1; // gera números em ordem crescente

    for (auto j = 0; j < TESTS; j++)
        for (auto i = 0; i < MAX; i++)
            array[j][i] = array[0][i];

    if (display)
    {
        cout << endl
             << "Array gerado: ";
        for (auto i = 0; i < MAX; i++)
            cout << array[0][i] << " ";
        cout << endl;
    }

    // TODO: testar os outros algoritmos (insertion_sortBB e shellsort)
    //! DONE

    switch (TESTS)
    {
    case 5:

    case 4:
        combsort(array[3], MAX, loginfo[3]);
    case 3:
        shakesort(array[2], MAX, loginfo[2]);
    case 2:
        quicksort(array[1], 0, MAX - 1, loginfo[1]); // passa início e fim do trecho de processamento (MAX-1)
    case 1:
        bubblesort(array[0], MAX, loginfo[0]); // passa tamanho do array
        break;
    }

    if (display)
    {
        for (auto j = 0; j < TESTS; j++)
        {
            cout << endl
                 << "Array ordenado " << sortNames[j] << ": ";
            for (auto i = 0; i < MAX; i++)
                cout << array[j][i] << " ";
            cout << endl;
        }
    }

    for (auto i = 0; i < TESTS; i++)
        delete[] array[i];

    delete[] array;

    for (auto i = 0; i < TESTS; i++)
        log[i] = loginfo[i];
}

// Função de quicksort
void quicksort(array_t array, int i, int f, loginfo_t &loginfo)
{
    auto start = std::chrono::steady_clock::now();
    int p;
    if (f > i)
    {
        p = particiona(array, i, f, loginfo);
        quicksort(array, i, p - 1, loginfo);
        quicksort(array, p + 1, f, loginfo);
    }
    auto finish = std::chrono::steady_clock::now();
    get<2>(loginfo) = finish - start;
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
    auto start = std::chrono::steady_clock::now();

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
    auto finish = std::chrono::steady_clock::now();
    get<2>(loginfo) = finish - start;
}

void shakesort(array_t array, array_size_t array_size, loginfo_t &loginfo)
{
    auto start = std::chrono::steady_clock::now();

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
    auto finish = std::chrono::steady_clock::now();
    get<2>(loginfo) = finish - start;
}

void combsort(array_t array, array_size_t array_size, loginfo_t &loginfo)
{
    auto start = std::chrono::steady_clock::now();

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
    auto finish = std::chrono::steady_clock::now();
    get<2>(loginfo) = finish - start;
}