#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Definição de constantes
#define TAM_POPULACAO 10
#define VALOR_ALVO 500
#define MIN_VALOR 1
#define MAX_VALOR 100000

// Gera um número aleatório entre MIN e MAX
int gerar_aleatorio(int min, int max)
{
    return min + rand() % (max - min + 1);
}

// Avalia o "fitness" ou pontuação (quanto mais proximo do VALOR_ALVO, melhor)
int fitness(int valor)
{
    return abs(VALOR_ALVO - valor);
}

// Inicializa população com o número de indivíduos definido por TAM_POPULACAO
void inicializar(int pop[])
{
    for (int i = 0; i < TAM_POPULACAO; i++)
    {
        pop[i] = gerar_aleatorio(MIN_VALOR, MAX_VALOR);
    }
}

// Exibe a população da geração atual
void mostrar_populacao(int pop[], int geracao)
{
    printf("Geracao %d: ", geracao);
    for (int i = 0; i < TAM_POPULACAO; i++)
    {
        printf("%d ", pop[i]);
    }
    printf("\n");
}

// Verifica se algum indivíduo alcançou a pontuação desejada
int encontrou_solucao(int pop[])
{
    for (int i = 0; i < TAM_POPULACAO; i++)
    {
        if (pop[i] == VALOR_ALVO)
            return 1;
    }
    return 0;
}

// Compara dois indivíduos para ordenação crescente por fitness
int comparar(const void *a, const void *b)
{
    int f1 = fitness(*(int *)a); // Faz o casting de void * para int * e, após isso, acessa o valor do elemento
    int f2 = fitness(*(int *)b);
    return f1 - f2;
}

// Altera o valor da taxa de mutação mediante o número da geração
int taxa_mutacao_dinamica(int geracao)
{
    if (geracao < 10)
        return 30;
    if (geracao < 20)
        return 20;
    return 10;
}

// Talvez aplica uma taxa de mutação adaptativa (intervalo aumenta proporcionalemente à distância da pontuação desejada)
void mutacao_adaptativa(int *valor, int geracao)
{
    int taxa = taxa_mutacao_dinamica(geracao);
    int chance = rand() % 100;
    if (chance < taxa)
    {
        int dist = abs(*valor - VALOR_ALVO);
        int max_mut = dist / 10 + 1;
        if (max_mut > 50)
            max_mut = 50;
        int mut = (rand() % (2 * max_mut + 1)) - max_mut; // -max...+max
        *valor += mut;

        if (*valor < MIN_VALOR)
            *valor = MIN_VALOR;
        if (*valor > MAX_VALOR)
            *valor = MAX_VALOR;
    }
}

// Realiza a seleção por torneio binário entre os melhores
int torneio(int candidatos[], int tamanho)
{
    int a = candidatos[gerar_aleatorio(0, tamanho - 1)];
    int b = candidatos[gerar_aleatorio(0, tamanho - 1)];
    return (fitness(a) < fitness(b)) ? a : b; // Retorna o indivíduo com a maior pontuação
}

// Gera uma nova geração
void nova_geracao(int pop_atual[], int nova_pop[], int geracao)
{
    int temp[TAM_POPULACAO];
    for (int i = 0; i < TAM_POPULACAO; i++)
        temp[i] = pop_atual[i];

    // Ordena por pontuação (do melhor para o pior)
    qsort(temp, TAM_POPULACAO, sizeof(int), comparar);

    // Mantém o melhor indivíduo da população anterior
    nova_pop[0] = temp[0];

    // Seleciona os melhores 50% como requisito para cruzamento
    int melhores[TAM_POPULACAO / 2];
    for (int i = 0; i < TAM_POPULACAO / 2; i++)
        melhores[i] = temp[i];

    // Cruzamento aleatório entre os melhores e aplicação da mutação adaptativa com a seleção por torneio
    for (int i = 1; i < TAM_POPULACAO; i++)
    {
        int pai = torneio(melhores, TAM_POPULACAO / 2);
        int mae = torneio(melhores, TAM_POPULACAO / 2);
        int min = (pai < mae) ? pai : mae;
        int max = (pai > mae) ? pai : mae;
        int filho = gerar_aleatorio(min, max); // Gera um filho com valor entre o mínimo e máximo dos pais

        mutacao_adaptativa(&filho, geracao);
        nova_pop[i] = filho;
    }
    // Substitui o último filho por um novo valor aleatório - Diversidade Genética
    nova_pop[TAM_POPULACAO - 1] = gerar_aleatorio(MIN_VALOR, MAX_VALOR);
}

int main()
{
    srand(time(NULL)); // Inicializa o randomizador com uma semente diferente a cada execução

    int populacao[TAM_POPULACAO];
    int nova_populacao[TAM_POPULACAO];
    int geracao = 0;

    inicializar(populacao);
    mostrar_populacao(populacao, geracao);

    while (!encontrou_solucao(populacao))
    {
        geracao++;
        nova_geracao(populacao, nova_populacao, geracao);

        // Reposição da população antiga pela nova
        for (int i = 0; i < TAM_POPULACAO; i++)
            populacao[i] = nova_populacao[i];

        mostrar_populacao(populacao, geracao);
    }

    printf("\nSolucao encontrada na geracao %d\n", geracao);
    return 0;
}