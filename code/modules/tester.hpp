#pragma once
#include "common.hpp"

namespace cpd
{
    // Definições e variáveis globais necessárias
    typedef sort::element_t element_t;                        // tipo do elemento
    typedef size_t array_size_t;                  // tipo do tamanho do array
    typedef std::vector<element_t> array_t;       // Tipo para especificar formato do array

    class Timer // classe para medir tempo de execução
    {
    private:
        std::chrono::time_point<std::chrono::steady_clock> start, end;                   // variáveis para armazenar tempo inicial e final
        std::chrono::duration<double> _duration = std::chrono::duration<double>::zero(); // variável para armazenar duração

    public:
        void Start()
        {
            start = std::chrono::steady_clock::now();
        }

        void Stop()
        {
            auto end = std::chrono::steady_clock::now();
            _duration = end - start;
        }

        const std::chrono::duration<double> &Duration()
        {
            return _duration; // retorna duração
        }
    };

    template <typename FuncType, typename LogType>
    class Tester // classe para testar funções de ordenação
    {
    private:
#define TestSize 4                                                                             // número de testes a serem realizados
        std::mt19937 rng;                                                                      // gerador de números aleatórios
        uint32_t seed_val;                                                                     // semente de geração de números
        std::string testNames[TestSize] = {"Crescente", "Aleatorio", "Decrescente", "Iguais"}; // nomes dos testes
        std::vector<FuncType> Functions;                                                       // array de funções de ordenação
        int Size;                                                                              // tamanho do array de funções de ordenação
        std::vector<std::string> FuncNames;                                                    // array de nomes das funções de ordenação

    private:
        void testTimer(FuncType sort, array_t &A, LogType &loginfo, Timer &time) // função para testar tempo de execução
        {
            time.Start();                   // inicia timer
            TestFunction(sort, A, loginfo); // executa função de ordenação
            time.Stop();                    // para timer
        }

        void runs(array_size_t Array_Size, int TestType, LogType log[], Timer Timers[])
        {
            std::uniform_int_distribution<> distrib(0, INT_MAX);   // cria gerador com distribuição uniforme entre 0 e MAX_INT
            std::vector<array_t> array(Size, array_t(Array_Size)); // array dinâmico que armazena os números

            // testar com as 3 versões de array (aleatório, crescente e decrescente):
            switch (TestType)
            {
            case 0:
                for (size_t i = 0; i < Array_Size; i++)
                    array[0][i] = i + 1; // gera números em ordem crescente
                break;
            // case 1:
            //     for (size_t i = 0; i < Array_Size; i++)
            //         array[0][i] = distrib(Tester::rng); // gera números aleatórios para o array
            //     break;
            case 2:
                for (size_t i = 0; i < Array_Size; i++)
                    array[0][i] = Array_Size - i; // gera números em ordem decrescente
                break;
            case 3:
                for (size_t i = 0; i < Array_Size; i++)
                    array[0][i] = 1; // gera números iguais
                break;
            default:
                std::cout << "Tipo de array inválido" << std::endl;
                return;
            }

#pragma omp parallel for
            for (auto i = 0; i < Size; i++)
            {
                std::memmove(array[i].data(), array[0].data(), Array_Size * sizeof(int)); // copia array gerado para os outros arrays
            }

#pragma omp parallel for
            for (auto i = 0; i < Size; i++)
            {
                testTimer(Functions[i], array[i], log[i], Timers[i]); // executa as funções de ordenação
            }

            if (TestType != 1 && Array_Size <= 100)
            {
                for (auto j = 0; j < Size; j++)
                {
                    std::cout << std::endl
                              << "Array " << Tester::testNames[TestType] << " Ordenado pelo " << FuncNames[j] << ":" << std::endl; // exibe array ordenado
                    for (size_t i = 0; i < Array_Size; i++)
                        std::cout << array[j][i] << " ";
                    std::cout << std::endl;
                }
            }

            for (int i = 0; i < Size; i++)
            {
                array[i].clear();
                array[i].shrink_to_fit();
            }
            array.clear();
            array.shrink_to_fit();
        }

        void displayStats(std::ostream &output, const std::string &separator, LogType loginfo[], std::string testName, array_size_t S, Timer Timers[])
        {
            output << std::endl
                   << std::setw(Spacer) << testName << separator << "Tamanho " << S << ":" << std::endl // exibe tipo de teste
                   << std::setw(Spacer) << "Nome da funcao";
            DisplayLogNames(output, separator);
            output << separator << std::setw(Spacer) << "Tempo" << std::endl;

            for (auto i = 0; i < Size; i++)
            {
                output << std::setw(Spacer) << FuncNames[i];

                std::apply([this, &output, &separator](auto &&...args)
                           { ((output << separator << std::setw(Spacer) << args), ...); },
                           loginfo[i]);

                output << separator << std::setw(Spacer) << Timers[i].Duration().count() << "s" << std::endl;
            }
        }

        void output(bool &Automatic, int &Passes, LogType **log[], int arraySizes[], Timer **timers[])
        {
            int outputMode = 0;
            std::ostream *out = &std::cout; // ponteiro para stream de saída
            std::ofstream myFile;           // arquivo de saída
            std::string separator = " | ";  // separador de colunas

            if (!Automatic) // se modo automático não estiver ativado
            {
                std::cout.flush(); // limpa buffer de saída
                std::cout << std::endl
                          << "Testes Concluidos!" << std::endl
                          << "1 - Terminal" << std::endl
                          << "2 - TXT" << std::endl
                          << "3 - CSV" << std::endl
                          << "Entre com o modo de saida: ";

                std::cin >> outputMode; // lê modo de saída
                do
                {
                    switch (outputMode)
                    {
                    case 1:
                        break;
                    case 2:
                        myFile.open("saida.txt"); // abre arquivo de saída
                        out = &myFile;            // aponta ponteiro para stream de saída para o arquivo
                        break;
                    case 3:
                        myFile.open("saida.csv"); // abre arquivo de saída
                        out = &myFile;            // aponta ponteiro para stream de saída para o arquivo
                        separator = ";";          // altera separador de colunas
                        break;
                    default:
                        std::cout << "Modo de saida invalido" << std::endl;
                        break;
                    }
                } while (outputMode < 1 || outputMode > 3);
            }

            for (auto i = 0; i < Passes; i++)
            {
                for (auto j = 0; j < TestSize; j++)
                    displayStats(*out, separator, log[i][j], testNames[j], arraySizes[i], timers[i][j]); // exibe contadores de comparações e trocas
                *out << std::endl;
                for (size_t j = 0; j < (Spacer + 3); j++)
                {
                    *out << "=="; // exibe linha de separação
                    for (auto k = 0; k < (int)std::tuple_size<LogType>::value; k++)
                        *out << "="; // exibe linha de separação
                }
                *out << std::endl;
            }
            if (!Automatic)
            {
                std::cout << std::endl
                          << "Dados prontos! Pressione enter para sair";
                std::cin.ignore();
                std::cin.get();
            }
        }

    public:
        size_t Spacer = 14; // largura de caracteres para exibição de dados
        Tester(std::initializer_list<FuncType> functions, std::initializer_list<std::string> funcNames)
        {
            if (functions.size() != funcNames.size())
                throw std::invalid_argument("Erro: Numero de funcoes e nomes de funcoes nao coincidem"); // verifica se número de funções e nomes de funções coincidem

            Functions = functions;   // inicializa vetor de funções de ordenação
            FuncNames = funcNames;   // inicializa vetor de nomes de funções de ordenação
            Size = functions.size(); // inicializa tamanho do vetor de funções de ordenação
            rng.seed(seed_val);      // inicializa semente de geração de números aleatórios
        }

        Tester(const std::vector<FuncType> &functions, const std::vector<std::string> &funcNames)
        {
            if (functions.size() != funcNames.size())
                throw std::invalid_argument("Erro: Numero de funcoes e nomes de funcoes nao coincidem"); // verifica se número de funções e nomes de funções coincidem

            Functions = functions;   // inicializa vetor de funções de ordenação
            FuncNames = funcNames;   // inicializa vetor de nomes de funções de ordenação
            Size = functions.size(); // inicializa tamanho do vetor de funções de ordenação
            rng.seed(seed_val);      // inicializa semente de geração de números aleatórios
        }

        Tester(std::initializer_list<std::pair<FuncType, std::string>> functions)
        {
            Functions.reserve(functions.size()); // reserva espaço para vetor de funções de ordenação
            FuncNames.reserve(functions.size()); // reserva espaço para vetor de nomes de funções de ordenação

            for (auto &f : functions)
            {
                Functions.push_back(f.first);  // inicializa vetor de funções de ordenação
                FuncNames.push_back(f.second); // inicializa vetor de nomes de funções de ordenação
            }

            Size = functions.size(); // inicializa tamanho do vetor de funções de ordenação
            rng.seed(seed_val);      // inicializa semente de geração de números aleatórios
        }

        Tester(const std::vector<std::pair<FuncType, std::string>> &functions)
        {
            Functions.reserve(functions.size()); // reserva espaço para vetor de funções de ordenação
            FuncNames.reserve(functions.size()); // reserva espaço para vetor de nomes de funções de ordenação

            for (auto &f : functions)
            {
                Functions.push_back(f.first);  // inicializa vetor de funções de ordenação
                FuncNames.push_back(f.second); // inicializa vetor de nomes de funções de ordenação
            }

            Size = functions.size(); // inicializa tamanho do vetor de funções de ordenação
            rng.seed(seed_val);      // inicializa semente de geração de números aleatórios
        }

        ~Tester()
        {
            Functions.clear(); // limpa vetor de funções de ordenação
            FuncNames.clear(); // limpa vetor de nomes de funções de ordenação
        }

        // Define como a função de ordenação deve ser chamada, com os parâmetros necessários
        void virtual TestFunction(FuncType Function, array_t &Array, LogType &loginfo)
        {
            Function(Array, loginfo); // executa a função de ordenação
        }

        // Exibe os nomes das colunas do log
        void virtual DisplayLogNames(std::ostream &Output, const std::string &Separator)
        {
            Output << Separator << std::setw(Spacer) << "Trocas"
                   << Separator << std::setw(Spacer) << "Comparacoes"; // exibe os nomes das métricas de log
        }

        // Executa os testes de ordenação, com o numero de passadas especificado
        // e o tamanho inicial do array, o qual é multiplicado por 10 toda a passada
        // automatic: se true, dados são exibidos direto no terminal
        void BatchTests(bool Automatic, int Passes, array_size_t Initial)
        {
            int *arraySizes = new int[Passes];       // armazena o tamanho dos arrays de cada passada
            arraySizes[0] = Initial;                 // define o tamanho do primeiro array
            LogType ***log = new LogType **[Passes]; // armazena contadores de comparações e trocas (ver typedef acima)
            Timer ***timers = new Timer **[Passes];  // armazena contadores de tempo de execução

            for (int i = 0; i < Passes; i++)
            {
                if (i > 0)
                    arraySizes[i] = arraySizes[i - 1] * 10; // calcula o tamanho dos arrays de cada passada

                log[i] = new LogType *[TestSize];  // aloca espaço para cada função de ordenação
                timers[i] = new Timer *[TestSize]; // aloca espaço para cada função de ordenação
                for (int j = 0; j < TestSize; ++j)
                {
                    log[i][j] = new LogType[Size];  // aloca espaço para cada função de ordenação
                    timers[i][j] = new Timer[Size]; // aloca espaço para cada função de ordenação
                }
            }

#pragma omp parallel for
            for (auto i = 0; i < Passes; i++)
            {
#pragma omp parallel for
                for (auto j = 0; j < TestSize; j++)
                    runs(arraySizes[i], j, log[i][j], timers[i][j]); // executa as funções de ordenação                                                                  // não mostra mais a primeira passada dos algoritmos
            }

            output(Automatic, Passes, log, arraySizes, timers); // exibe os resultados

            for (int i = 0; i < Passes; i++)
            {
                for (int j = 0; j < TestSize; ++j)
                {
                    delete[] log[i][j];
                    delete[] timers[i][j];
                }
                delete[] log[i];
                delete[] timers[i];
            }
            delete[] log;
            delete[] timers;
            delete[] arraySizes;
        }
    };
} // namespace CPD