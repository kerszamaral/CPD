/* Ian Kersz - Cartão UFRGS: 00338368
06.12.22
Programa que testa o Insertion Sort, Insertion Sort com Busca Binaria e Shellsort
em um conjunto de dados em ordem crescente, aleatoria e decrescente
para testar a performance dos algoritmos.

O programa expande o numero de dados baseado no numero de RUNS, 
cada vez aumentando em 10 vezes o numero de numeros.
*/

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

// TODO: executar várias vezes os algoritmos, com tamanhos diferentes (e.g., 100, 1000 e 10000)
//! DONE

#define TESTS 3 // quantidade de testes a serem executados
#define RUNS 4  // quantidade de vezes que cada teste será executado

using namespace std;

typedef int array_size_t;               // Tipo para especificar tamanho do array
typedef int *array_t;                   // Tipo para especificar formato do array
typedef std::tuple<int, int> loginfo_t; // armazena contagem de comparações e trocas
typedef std::mt19937 MyRNG;             // Gerador de números aleatórios do tipo Mersenne Twister Random Generator
typedef std::tuple<loginfo_t, loginfo_t, loginfo_t> loginfos;

MyRNG rng;         // gerador de números aleatórios
uint32_t seed_val; // semente de geração de números

loginfo_t insertion_sort(array_t, array_size_t);
loginfo_t insertion_sortBB(array_t, array_size_t);
loginfo_t shellsort(array_t, array_size_t);
std::tuple<int, int, int> busca_binaria(array_t, int, int, int); // retorna uma tupla contendo <posicao, qtd de trocas, qtd de comparações>
loginfos runs(int MAX, int type, bool display);

std::string testNames[] = {"Crescente", "Aleatorio", "Decrescente"};

int main(void)
{
    int MAX = 50; // quantidade de números no array
    bool show = true;
    // cout << __cplusplus << endl;                                             // verifica versão do compilador
    rng.seed(seed_val);                             // inicializa semente de geração de números aleatórios

    for (auto k = 0; k < RUNS; k++)
    {
        cout << endl << "Tamanho do Array testado: " << MAX << endl;

        loginfos log[3];

        log[0] = runs(MAX, 0, show);
        log[1] = runs(MAX, 1, show);
        log[2] = runs(MAX, 2, show);

        // TODO: armazenar essas informações em um matriz ou hashtable
        //! DONE

        for (auto i = 0; i < 3; i++)
        {
            cout << endl << "\tTeste " << testNames[i] << ":" << endl;
            cout << "\t\tInsertion:" << endl;
            cout << "\t\t\tQuantidade de trocas: " << get<0>(get<0>(log[i])) << endl;
            cout << "\t\t\tQuantidade de comparações: " << get<1>(get<0>(log[i])) << endl;

            cout << "\t\tInsertionBB:" << endl;
            cout << "\t\t\tQuantidade de trocas: " << get<0>(get<1>(log[i])) << endl;
            cout << "\t\t\tQuantidade de comparações: " << get<1>(get<1>(log[i])) << endl;

            cout << "\t\tShellsort:" << endl;
            cout << "\t\t\tQuantidade de trocas: " << get<0>(get<2>(log[i])) << endl;
            cout << "\t\t\tQuantidade de comparações: " << get<1>(get<2>(log[i])) << endl;
        }

        if (k == 0)
        {
            show = false;
            MAX += 50;
        }
        else
            MAX *= 10;
    }

    // TODO: mostrar informações de execução de todos os algoritmos
    //! DONE

    return 0;
}

loginfos runs(int MAX, int type, bool display)
{
    uniform_int_distribution<> distrib(0, INT_MAX); // cria gerador com distribuição uniforme entre 0 e MAX_INT
    loginfo_t *loginfo = new loginfo_t[TESTS]; // armazena contadores de comparações e trocas (ver typedef acima)

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

    if(display)
    {
        cout << endl << "Array gerado: ";
        for (auto i = 0; i < MAX; i++)
            cout << array[0][i] << " ";
        cout << endl;
    }

    // TODO: testar os outros algoritmos (insertion_sortBB e shellsort)
    //! DONE
    loginfo[0] = insertion_sort(array[0], MAX);
    loginfo[1] = insertion_sortBB(array[1], MAX);
    loginfo[2] = shellsort(array[2], MAX);

    if(display)
    {
        for (auto j = 0; j < TESTS; j++)
        {
            cout << endl << "Array ordenado " << j + 1 << ": ";
            for (auto i = 0; i < MAX; i++)
                cout << array[j][i] << " ";
            cout << endl;
        }
    }

    for (auto i = 0; i < TESTS; i++)
        delete[] array[i];

    delete[] array;

    return make_tuple(loginfo[0], loginfo[1], loginfo[2]);
}

// Função de Inserção Direta com Busca Linear
loginfo_t insertion_sort(array_t array, array_size_t array_size)
{
    int trocas = 0, comparacoes = 0;
    for (int i = 1; i < array_size; i++)
    {                          // do segundo ao último
        auto chave = array[i]; // chave a inserir no subarray ordenado
        auto j = i - 1;        // último elemento do subarray ordenado
        comparacoes = comparacoes + 1;
        while (j >= 0 && array[j] > chave)
        { // busca linear da direita para a esquerda no subarray ordenado
            comparacoes = comparacoes + 1;
            array[j + 1] = array[j];
            j = j - 1;
            trocas = trocas + 1;
        }
        if (j + 1 != i)
        {
            array[j + 1] = chave;
            trocas = trocas + 1;
        }
    }
    return make_tuple(trocas, comparacoes); // retorna quantidade de operações
}

loginfo_t insertion_sortBB(array_t array, array_size_t array_size)
{
    int trocas = 0, comparacoes = 0;
    std::tuple<int, int, int> info;
    int i, local, j, elemento;
    for (i = 1; i < array_size; ++i)
    {
        j = i - 1;
        elemento = array[i];
        info = busca_binaria(array, elemento, 0, j);
        local = get<0>(info);
        trocas += get<1>(info);
        comparacoes += get<2>(info) + 1;
        while (j >= local)
        {
            trocas++;
            array[j + 1] = array[j];
            j--;
        }
        if (j + 1 != i)
        {
            array[j + 1] = elemento;
            trocas = trocas + 1;
        }
    }
    return make_tuple(trocas, comparacoes); // retorna quantidade de operações
}

// *****************************************************
//  TODO: Implementação dos seus algoritmos (a seguir)
//! DONE

// Faz busca binária do 'elemento' no 'array', entre os índices 'inicio' e 'fim'
// retorna posição do elemento, quantidade de trocas e quantidade de comparações
std::tuple<int, int, int> busca_binaria(array_t array, int elemento, int inicio, int fim)
{
    int comparacoes = 0, trocas = 0, meio = (fim - inicio) / 2;

    // defina aqui sua versão da função de busca binária
    while (inicio <= fim)
    {
        comparacoes++;
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
    return make_tuple(meio, trocas, comparacoes); // retorna posição do elemento e quantidade de operações
}

loginfo_t shellsort(array_t array, array_size_t array_size)
{
    int trocas = 0, comparacoes = 0;

    int i, j, valor;
    int espaco = 1;

    do
    {
        espaco = 3 * espaco + 1;
    } while (espaco < array_size);

    do
    {
        espaco /= 3;
        for (i = espaco; i < array_size; i++)
        {
            valor = array[i];
            j = i - espaco;

            while (j >= 0 && valor < array[j])
            {
                trocas++;
                array[j + espaco] = array[j];
                j -= espaco;
            }
            comparacoes++;
            array[j + espaco] = valor;
        }
    } while (espaco > 1);

    return make_tuple(trocas, comparacoes); // retorna quantidade de operações
}