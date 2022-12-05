// Atenção: usa código C++11
// para saber se o seu compilador tem suporte, execute: 
// cout << __cplusplus;
// O resultado deve ser 201103L ou maior.
// o do google collab é C++14 
// A grande maioria dos compiladores atuais suporta nativamente c++11. 
// Outros exigem a configuração de parâmetros de compilação... Verifique a documentação do seu.

#include<iostream>
#include<tuple>
#include<random>
#include<bits/stdc++.h>

//TODO: executar várias vezes os algoritmos, com tamanhos diferentes (e.g., 100, 1000 e 10000)

#define MAX 50                                                                  // quantidade de números no array

using namespace std;

typedef int array_size_t;                                                       // Tipo para especificar tamanho do array
typedef int* array_t;                                                           // Tipo para especificar formato do array
typedef std::tuple<int, int> loginfo_t;                                         // armazena contagem de comparações e trocas  
typedef std::mt19937 MyRNG;                                                     // Gerador de números aleatórios do tipo Mersenne Twister Random Generator 

MyRNG rng;                                                                      // gerador de números aleatórios
uint32_t seed_val;                                                              // semente de geração de números

loginfo_t insertion_sort(array_t, array_size_t);
loginfo_t insertion_sortBB(array_t, array_size_t);
loginfo_t shellsort(array_t, array_size_t);
std::tuple<int, int, int> busca_binaria(array_t, int, int, int);                // retorna uma tupla contendo <posicao, qtd de trocas, qtd de comparações>

int main(void){    
    // cout << __cplusplus << endl;                                             // verifica versão do compilador
    rng.seed(seed_val);                                                         // inicializa semente de geração de números aleatórios    
    uniform_int_distribution<> distrib(0, INT_MAX);                             // cria gerador com distribuição uniforme entre 0 e MAX_INT
    loginfo_t loginfo;                                                          // armazena contadores de comparações e trocas (ver typedef acima)

    int* array = new int[MAX];                                                  // array dinâmico que armazena os números

    // testar com as 3 versões de array (aleatório, crescente e decrescente):
    // for(auto i=0;i<MAX;i++) array[i] = distrib(rng);                         // gera números aleatórios para o array
    //for(auto i=0;i<MAX;i++) array[i] = i;                                     // gera números em ordem crescente
    for(auto i=0;i<MAX;i++) array[i] = MAX-i;                                   // gera números em ordem decrescente
    
    cout << "Array gerado: ";
    for(auto i=0;i<MAX;i++) cout << array[i] << " ";

    // TODO: testar os outros algoritmos (insertion_sortBB e shellsort)
    loginfo = insertion_sort(array, MAX);

    //TODO: armazenar essas informações em um matriz ou hashtable

    cout << endl << "Array ordenado: ";                               
    for(auto i=0;i<MAX;i++) cout << array[i] << " ";

    cout << endl;
    cout << "Quantidade de trocas: " << get<0>(loginfo) << endl;
    cout << "Quantidade de comparações: " << get<1>(loginfo) << endl;

    // TODO: mostrar informações de execução de todos os algoritmos

    delete[] array;
    return 0;
}

// Função de Inserção Direta com Busca Linear
loginfo_t insertion_sort(array_t array, array_size_t array_size){
    int trocas = 0, comparacoes = 0;
    for(int i=1;i<array_size;i++){                                              // do segundo ao último
        auto chave = array[i];                                                  // chave a inserir no subarray ordenado
        auto j = i-1;                                                           // último elemento do subarray ordenado         
        comparacoes = comparacoes + 1;
        while(j >= 0 && array[j] > chave){                                      // busca linear da direita para a esquerda no subarray ordenado   
            comparacoes = comparacoes + 1;
            array[j+1] = array[j];
            j = j - 1;
            trocas = trocas + 1;
        }
        if(j+1 != i){ 
           array[j+1] = chave;
           trocas = trocas + 1;
       }
    }
    return make_tuple(trocas, comparacoes);                                     // retorna quantidade de operações
}

loginfo_t insertion_sortBB(array_t array, array_size_t array_size){    
   int trocas = 0, comparacoes = 0;
   std::tuple<int, int, int> info;
   int i, local, j, k, elemento;
   for(i = 1; i < array_size; ++i) {
      j = i - 1;
      elemento = array[i];
      info = busca_binaria(array, elemento, 0, j);
      local = get<0>(info);
      trocas += get<1>(info);
      comparacoes += get<2>(info)+1;
      while (j >= local) {
         trocas++;
         array[j+1] = array[j];
         j--;
      }
      if(j+1 != i){ 
           array[j+1] = elemento;
           trocas = trocas + 1;
       }
   }
    return make_tuple(trocas, comparacoes);                                     // retorna quantidade de operações
}

// *****************************************************
//  TODO: Implementação dos seus algoritmos (a seguir)

// Faz busca binária do 'elemento' no 'array', entre os índices 'inicio' e 'fim'
// retorna posição do elemento, quantidade de trocas e quantidade de comparações
std::tuple<int, int, int> busca_binaria(array_t array, int elemento, int inicio, int fim){
   int comparacoes = 0, trocas = 0, meio = (fim-inicio)/2;

      // defina aqui sua versão da função de busca binária

   return make_tuple(meio, trocas, comparacoes);                                // retorna posição do elemento e quantidade de operações
}

loginfo_t shellsort(array_t array, array_size_t array_size){
    int trocas = 0, comparacoes = 0;
    
    // defina aqui sua versão da função shellsort    
    
    return make_tuple(trocas, comparacoes);                                     // retorna quantidade de operações
}