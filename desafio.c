#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Para medir o tempo

// Definição do Componente da torre
typedef struct {
    char nome[30];
    char tipo[20]; 
    int prioridade; 
} Componente;

// Protótipos
void cadastrarComponentes(Componente componentes[], int *tamanho);
void mostrarComponentes(Componente componentes[], int tamanho);
void trocar(Componente *a, Componente *b);

// Algoritmos de ordenação
long long selectionSortPrioridade(Componente componentes[], int tamanho);
long long insertionSortTipo(Componente componentes[], int tamanho);
long long bubbleSortNome(Componente componentes[], int tamanho);

// Busca
int buscaBinariaPorNome(Componente componentes[], int tamanho, const char chaveBusca[]);

// Menu
void menuPrincipal(Componente componentes[], int tamanho);

// --- Funções Auxiliares ---

void trocar(Componente *a, Componente *b) {
    Componente temp = *a;
    *a = *b;
    *b = temp;
}

void mostrarComponentes(Componente componentes[], int tamanho) {
    printf("\n*** 🗃️ LISTA DE COMPONENTES ***\n");
    printf("+------------------------------+--------------------+--------------+\n");
    printf("| Nome                         | Tipo               | Prioridade   |\n");
    printf("+------------------------------+--------------------+--------------+\n");
    for (int i = 0; i < tamanho; i++) {
        printf("| %-28s | %-18s | %-12d |\n",
               componentes[i].nome, componentes[i].tipo, componentes[i].prioridade);
    }
    printf("+------------------------------+--------------------+--------------+\n");
}

void cadastrarComponentes(Componente componentes[], int *tamanho) {
    char buffer[100]; 
    int count = 0;
    char resposta;

    printf("--- 📝 Cadastro de Componentes (Máx. 20) ---\n");

    do {
        if (count >= 20) {
            printf("⚠️ Limite máximo de 20 componentes atingido!\n");
            break;
        }

        printf("\nComponente #%d:\n", count + 1);

        printf("   Nome: ");
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, " %29[^\n]", componentes[count].nome);

        printf("   Tipo: ");
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, " %19[^\n]", componentes[count].tipo);

        printf("   Prioridade (1 a 10): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL || sscanf(buffer, "%d", &componentes[count].prioridade) != 1) {
             componentes[count].prioridade = 5; 
        }
        
        // Garante que a prioridade esteja entre 1 e 10
        if (componentes[count].prioridade < 1) componentes[count].prioridade = 1;
        if (componentes[count].prioridade > 10) componentes[count].prioridade = 10;
        
        count++;

        printf("Deseja adicionar outro? (s/n): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL || sscanf(buffer, " %c", &resposta) != 1 || (resposta != 's' && resposta != 'S')) {
            break;
        }

    } while (resposta == 's' || resposta == 'S');

    *tamanho = count;
    printf("\n✅ Cadastro finalizado. %d componentes registrados.\n", *tamanho);
}

// --- Funções de Ordenação ---

// Selection Sort (Prioridade)
long long selectionSortPrioridade(Componente componentes[], int tamanho) {
    long long comparacoes = 0;
    for (int i = 0; i < tamanho - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < tamanho; j++) {
            comparacoes++;
            // Ordem Decrescente de Prioridade (Maior para o menor)
            if (componentes[j].prioridade > componentes[min_idx].prioridade) { 
                min_idx = j;
            }
        }
        if (min_idx != i) {
            trocar(&componentes[i], &componentes[min_idx]);
        }
    }
    return comparacoes;
}

// Insertion Sort (Tipo)
long long insertionSortTipo(Componente componentes[], int tamanho) {
    long long comparacoes = 0;
    for (int i = 1; i < tamanho; i++) {
        Componente chave = componentes[i];
        int j = i - 1;

        while (j >= 0) {
            comparacoes++;
            // Ordem Alfabética (A-Z)
            if (strcmp(componentes[j].tipo, chave.tipo) > 0) { 
                componentes[j + 1] = componentes[j];
                j = j - 1;
            } else {
                break; 
            }
        }
        componentes[j + 1] = chave;
    }
    return comparacoes;
}

// Bubble Sort (Nome)
long long bubbleSortNome(Componente componentes[], int tamanho) {
    long long comparacoes = 0;
    for (int i = 0; i < tamanho - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < tamanho - 1 - i; j++) {
            comparacoes++;
            // Ordem Alfabética (A-Z)
            if (strcmp(componentes[j].nome, componentes[j + 1].nome) > 0) { 
                trocar(&componentes[j], &componentes[j + 1]);
                trocou = 1;
            }
        }
        if (trocou == 0) break; 
    }
    return comparacoes;
}

// --- Função de Busca Binária ---

// Só funciona se a lista estiver ordenada por nome!
int buscaBinariaPorNome(Componente componentes[], int tamanho, const char chaveBusca[]) {
    int esquerda = 0;
    int direita = tamanho - 1;
    long long comparacoes = 0;
    clock_t inicio = clock();
    
    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        int resultado = strcmp(componentes[meio].nome, chaveBusca);
        comparacoes++;

        if (resultado == 0) {
            clock_t fim = clock();
            printf("\n✅ CHAVE ENCONTRADA! (Busca Binária)\n");
            printf("   Tempo de Busca: %.2f ms\n", ((double)(fim - inicio) * 1000.0) / CLOCKS_PER_SEC);
            printf("   Comparações: %lld\n", comparacoes);
            return meio;
        }

        if (resultado < 0) {
            esquerda = meio + 1;
        }
        else {
            direita = meio - 1;
        }
    }

    clock_t fim = clock();
    printf("\n❌ Componente-Chave não encontrado! (Busca Binária)\n");
    printf("   Tempo de Busca: %.2f ms\n", ((double)(fim - inicio) * 1000.0) / CLOCKS_PER_SEC);
    printf("   Comparações: %lld\n", comparacoes);
    return -1;
}

// --- Menu e Main ---

void menuPrincipal(Componente componentes[], int tamanho) {
    int opcao;
    int listaOrdenadaPorNome = 0;
    char buffer[100];
    
    if (tamanho == 0) {
        printf("⚠️ Cadastre os componentes primeiro (Opção 1).\n");
        return;
    }

    do {
        printf("\n\n*** 🕹️ MÓDULO ESTRATÉGICO DE MONTAGEM ***\n");
        printf("1. Exibir Componentes\n");
        printf("2. Ordenar por Prioridade (Selection Sort)\n");
        printf("3. Ordenar por Tipo (Insertion Sort)\n");
        printf("4. Ordenar por Nome (Bubble Sort)\n");
        printf("5. Buscar Componente-Chave (Busca Binária)\n");
        printf("6. Sair\n");
        printf("Escolha sua estratégia: ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL || sscanf(buffer, "%d", &opcao) != 1) {
            opcao = -1; 
        }

        clock_t inicio, fim;
        long long comparacoes = 0;
        double tempo_ms;

        switch (opcao) {
            case 1:
                mostrarComponentes(componentes, tamanho);
                listaOrdenadaPorNome = 0;
                break;

            case 2: // Selection Sort
                printf("\n--- ⬆️ Estratégia: Prioridade (Selection Sort) ---\n");
                inicio = clock();
                comparacoes = selectionSortPrioridade(componentes, tamanho);
                fim = clock();
                tempo_ms = ((double)(fim - inicio) * 1000.0) / CLOCKS_PER_SEC;
                printf("✅ Ordenação concluída.\n");
                printf("📊 Desempenho: Comparações: %lld | Tempo: %.2f ms\n", comparacoes, tempo_ms);
                mostrarComponentes(componentes, tamanho);
                listaOrdenadaPorNome = 0;
                break;

            case 3: // Insertion Sort
                printf("\n--- 🔠 Estratégia: Tipo (Insertion Sort) ---\n");
                inicio = clock();
                comparacoes = insertionSortTipo(componentes, tamanho);
                fim = clock();
                tempo_ms = ((double)(fim - inicio) * 1000.0) / CLOCKS_PER_SEC;
                printf("✅ Ordenação concluída.\n");
                printf("📊 Desempenho: Comparações: %lld | Tempo: %.2f ms\n", comparacoes, tempo_ms);
                mostrarComponentes(componentes, tamanho);
                listaOrdenadaPorNome = 0;
                break;

            case 4: // Bubble Sort
                printf("\n--- 🅰️ Estratégia: Nome (Bubble Sort) ---\n");
                inicio = clock();
                comparacoes = bubbleSortNome(componentes, tamanho);
                fim = clock();
                tempo_ms = ((double)(fim - inicio) * 1000.0) / CLOCKS_PER_SEC;
                printf("✅ Ordenação concluída.\n");
                printf("📊 Desempenho: Comparações: %lld | Tempo: %.2f ms\n", comparacoes, tempo_ms);
                mostrarComponentes(componentes, tamanho);
                listaOrdenadaPorNome = 1; 
                break;

            case 5: // Busca Binária
                if (!listaOrdenadaPorNome) {
                    printf("\n⚠️ ERRO: A Busca Binária precisa que a lista esteja ordenada por **NOME**!\n");
                    printf("   Use a Opção **4** primeiro.\n");
                    break;
                }
                char chaveBusca[30];
                printf("\n--- 🔍 Busca pelo Componente-Chave ---\n");
                printf("   Nome do componente-chave: ");
                fgets(buffer, sizeof(buffer), stdin);
                sscanf(buffer, " %29[^\n]", chaveBusca);
                
                int indice = buscaBinariaPorNome(componentes, tamanho, chaveBusca);
                
                if (indice != -1) {
                    printf("\n✨ Montagem Final: Componente-Chave Encontrado!\n");
                    printf("   ➡️ O componente **'%s'** na posição %d destrava a torre!\n", componentes[indice].nome, indice + 1);
                }
                break;

            case 6: 
                printf("\n👋 Módulo de Organização Encerrado.\n");
                break;

            default:
                printf("\n❌ Opção inválida.\n");
                break;
        }
    } while (opcao != 6);
}

int main() {
    Componente torre[20]; 
    int tamanho = 0; 

    printf("========================================================\n");
    printf("     🚀 Desafio Final: Módulo de Resgate da Torre       \n");
    printf("========================================================\n");

    // Cadastro inicial
    cadastrarComponentes(torre, &tamanho);

    // Inicia o menu
    menuPrincipal(torre, tamanho);

    return 0;
}