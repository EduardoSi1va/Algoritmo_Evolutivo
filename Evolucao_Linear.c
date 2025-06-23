#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Definição de constantes
#define TAM_POPULACAO 10
#define VALOR_ALVO 500
#define MIN_VALOR 1
#define MAX_VALOR 100000
#define TAXA_MUTACAO 10 // em porcentagem (10%)

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
void gerar_populacao(int pop[])
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
    int f1 = fitness(*(int *)a);
    int f2 = fitness(*(int *)b);
    return f1 - f2;
}

// Talvez aplica uma taxa de mutação linear (intervalo nunca é alterado)
void mutacao(int *valor)
{
    int chance = rand() % 100;
    if (chance < TAXA_MUTACAO)
    {
        int mut = (rand() % 5) - 2; // +/- 2
        *valor += mut;
        if (*valor < MIN_VALOR)
            *valor = MIN_VALOR;
        if (*valor > MAX_VALOR)
            *valor = MAX_VALOR;
    }
}

// Gera uma nova geração
void nova_geracao(int pop_atual[], int nova_pop[])
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

    // Cruzamento aleatório entre os melhores e aplicação da mutação
    for (int i = 1; i < TAM_POPULACAO; i++)
    {
        int pai = melhores[gerar_aleatorio(0, TAM_POPULACAO / 2 - 1)];
        int mae = melhores[gerar_aleatorio(0, TAM_POPULACAO / 2 - 1)];
        int filho = (pai + mae) / 2;
        mutacao(&filho);
        nova_pop[i] = filho;
    }
}

int main()
{
    srand(time(NULL));

    int populacao[TAM_POPULACAO];
    int nova_populacao[TAM_POPULACAO];
    int geracao = 0;

    gerar_populacao(populacao);
    mostrar_populacao(populacao, geracao);

    while (!encontrou_solucao(populacao))
    {
        geracao++;
        nova_geracao(populacao, nova_populacao);

        // Reposição da população antiga pela nova
        for (int i = 0; i < TAM_POPULACAO; i++)
            populacao[i] = nova_populacao[i];

        mostrar_populacao(populacao, geracao);
    }

    printf("\nSolucao encontrada na geracao %d\n", geracao);
    return 0;
}