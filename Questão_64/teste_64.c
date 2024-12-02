#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Célula de um quadrante da matriz
typedef struct {
    int custo, linha, coluna;
} Celula;

typedef struct {
    Celula* dados;
    int tamanho;
    int capacidade;
} Heap;

// Funções auxiliares para a heap
Heap* criarHeap(int capacidade) {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->dados = (Celula*)malloc(capacidade * sizeof(Celula));
    heap->tamanho = 0;
    heap->capacidade = capacidade;
    return heap;
}

void swap(Celula* a, Celula* b) {
    Celula temp = *a;
    *a = *b;
    *b = temp;
}

void shiftUp(Heap* heap, int indice) {
    while (indice > 0) {
        int pai = (indice - 1) / 2;
        if (heap->dados[indice].custo >= heap->dados[pai].custo) break;
        swap(&heap->dados[indice], &heap->dados[pai]);
        indice = pai;
    }
}

void heapify(Heap* heap, int indice) {
    while (2 * indice + 1 < heap->tamanho) {
        int esquerda = 2 * indice + 1;
        int direita = 2 * indice + 2;
        int menor = esquerda;
        if (direita < heap->tamanho && heap->dados[direita].custo < heap->dados[esquerda].custo) {
            menor = direita;
        }
        if (heap->dados[indice].custo <= heap->dados[menor].custo) break;
        swap(&heap->dados[indice], &heap->dados[menor]);
        indice = menor;
    }
}

void inserirHeap(Heap* heap, Celula celula) {
    heap->dados[heap->tamanho] = celula;
    heap->tamanho++;
    shiftUp(heap, heap->tamanho - 1);
}

Celula extrairMin(Heap* heap) {
    Celula min = heap->dados[0];
    heap->dados[0] = heap->dados[heap->tamanho - 1];
    heap->tamanho--;
    heapify(heap, 0);
    return min;
}

int verificaNo(int i, int j, int linhas, int colunas) {
    return i < 0 || i >= linhas || j < 0 || j >= colunas;
}

int minPathSum(int** grid, int gridSize, int* gridColSize) {
    int linhas = gridSize, colunas = gridColSize[0]; //inicializa as linhas e colunas fornecidas

    // cria a matriz para armazenar o menor custo, com o tamanho das linhas e das colunas fornecidas
    int** custo = (int**)malloc(linhas * sizeof(int*));
    for (int i = 0; i < linhas; i++) {
        custo[i] = (int*)malloc(colunas * sizeof(int));
        for (int j = 0; j < colunas; j++) {
            custo[i][j] = INT_MAX;
        }
    }

    // cria uma heap de ordem crescente para buscar o menor custo
    Heap* heap = criarHeap(linhas * colunas);

    // insere a célula inicial (0, 0) com o seu custo
    inserirHeap(heap, (Celula){grid[0][0], 0, 0});
    custo[0][0] = grid[0][0];

    // define os deslocamentos poss�veis (direita, baixo, esquerda, cima)
    static const int deslocamento[5] = {0, 1, 0, -1, 0};

    // continua a iteração enquanto tiver caminhos na heap
    while (heap->tamanho > 0) {
        Celula atual = extrairMin(heap); // célula com menor custo
        int c = atual.custo, i = atual.linha, j = atual.coluna;

        // verifica se chegou na célula do canto inferior esquerdo
        if (i == linhas - 1 && j == colunas - 1) {
            for (int k = 0; k < linhas; k++) free(custo[k]);
            free(custo);
            free(heap->dados);
            free(heap);
            return c;
        }

        // explora as 4 opções de caminho adjacentes
        for (int a = 0; a < 2; a++) {
            int novaLinha = i + deslocamento[a], novaColuna = j + deslocamento[a + 1]; // cria a localizacao (linha e coluna) da proxima célula caminho

            // verifica se a localizacao está dentro da matriz
            if (verificaNo(novaLinha, novaColuna, linhas, colunas)) continue;

            // calcula o custo para alcançar a célula adjacente
            int novoCusto = c + grid[novaLinha][novaColuna];

            // atualiza o custo se for menor
            if (novoCusto < custo[novaLinha][novaColuna]) {
                custo[novaLinha][novaColuna] = novoCusto;
                inserirHeap(heap, (Celula){novoCusto, novaLinha, novaColuna});
            }
        }
    }

    // Caso não seja possível alcançar o destino
    for (int i = 0; i < linhas; i++) free(custo[i]);
    free(custo);
    free(heap->dados);
    free(heap);
    return -1;
}
