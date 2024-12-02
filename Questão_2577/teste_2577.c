#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// c�lula de um quadrante da quest�o
typedef struct {
    int tempo, linha, coluna;
} Celula;

typedef struct {
    Celula* dados;
    int tamanho;
    int capacidade;
} Heap;

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
        if (heap->dados[indice].tempo >= heap->dados[pai].tempo) break;
        swap(&heap->dados[indice], &heap->dados[pai]);
        indice = pai;
    }
}

void heapify(Heap* heap, int indice) {
    while (2 * indice + 1 < heap->tamanho) {
        int esquerda = 2 * indice + 1;
        int direita = 2 * indice + 2;
        int menor = esquerda;
        if (direita < heap->tamanho && heap->dados[direita].tempo < heap->dados[esquerda].tempo) {
            menor = direita;
        }
        if (heap->dados[indice].tempo <= heap->dados[menor].tempo) break;
        swap(&heap->dados[indice], &heap->dados[menor]);
        indice = menor;
    }
}

void inserirHeap(Heap* heap, Celula celula) {
    heap->dados[heap->tamanho] = celula;
    heap->tamanho++;
    shiftUp(heap, heap->tamanho - 1);
}

Celula remove(Heap* heap) {
    Celula min = heap->dados[0];
    heap->dados[0] = heap->dados[heap->tamanho - 1];
    heap->tamanho--;
    heapify(heap, 0);
    return min;
}

int verificaNo(int i, int j, int linhas, int colunas) {
    return i < 0 || i >= linhas || j < 0 || j >= colunas;
}

int minimumTime(int** grid, int gridSize, int* gridColSize) {
    if (grid[1][0] > 1 && grid[0][1] > 1) return -1; // verifica o caso da c�lulas adjacentes serem > 1, ou seja, n�o tem como andar

    //inicializa a vari�vel "linhas" com o tamanho das linhas e "colunas" com o tamanho das colunas para usar no loop
    int linhas = gridSize, colunas = gridColSize[0];

    //cria uma matriz "tempo" para armazenar o menor tempo ao andar para cada c�lula.
    int** tempo = (int**)malloc(linhas * sizeof(int*));
    for (int i = 0; i < linhas; i++) {
        tempo[i] = (int*)malloc(colunas * sizeof(int));
        for (int j = 0; j < colunas; j++) {
            // inicializa todas as posi��es com `INT_MAX` (tempo infinito).
            tempo[i][j] = INT_MAX;
        }
    }

    // cria uma heap com ordem crescente para realizar a busca com base no menor tempo.
    Heap* heap = criarHeap(linhas * colunas);

    // insere a c�lula inicial come�ado pela c�lula do topo esquerdo com tempo 0 na heap e inicializa o tempo na matriz.
    inserirHeap(heap, (Celula){0, 0, 0});
    tempo[0][0] = 0;

    // define os deslocamentos poss�veis (direita, baixo, esquerda, cima).
    static const int deslocamento[5] = {0, 1, 0, -1, 0};

    // a fun��o vai continuar iterando enquanto tiver elementos na heap.
    while (heap->tamanho > 0) {
        // retira a c�lula com o menor tempo.
        Celula atual = remove(heap);
        int t = atual.tempo, i = atual.linha, j = atual.coluna;

        // se a c�lula retirada for o destino for a do canto inferior direito(n-1, m-1) retorna o tempo e libera a mem�ria.
        if (i == linhas - 1 && j == colunas - 1) {
            for (int k = 0; k < linhas; k++) free(tempo[k]);
            free(tempo);
            free(heap->dados);
            free(heap);
            return t;
        }

        // explora as 4 c�lulas adjacentes.
        for (int a = 0; a < 4; a++) {
            int novaLinha = i + deslocamento[a], novaColuna = j + deslocamento[a + 1];

            // verifica se as c�lulas estiverem dentro da matriz.
            if (verificaNo(novaLinha, novaColuna, linhas, colunas)) continue;

            // Calcula o tempo necess�rio para alcan�ar a c�lula adjacente.
            // Se `grid[r][s]` � maior que o tempo atual `t`:
            // - O tempo de chegada deve respeitar `grid[r][s]`, que representa o tempo m�nimo de entrada permitido.
            int ajuste = ((grid[novaLinha][novaColuna] - t) & 1) ? 0 : 1;
            // - Adiciona 1 ao tempo se o pr�ximo tempo de chegada `t + 1` for �mpar (ajuste para sincronizar com o tempo dispon�vel).
            int proximoTempo = (t + 1 > grid[novaLinha][novaColuna]) ? t + 1 : grid[novaLinha][novaColuna] + ajuste;

            // atualiza o tempo se o novo caminho for mais r�pido.
            if (proximoTempo < tempo[novaLinha][novaColuna]) {
                tempo[novaLinha][novaColuna] = proximoTempo;
                inserirHeap(heap, (Celula){proximoTempo, novaLinha, novaColuna});
            }
        }
    }

    // Caso n�o seja poss�vel alcan�ar o destino, libera a mem�ria e retorna -1.
    for (int i = 0; i < linhas; i++) free(tempo[i]);
    free(tempo);
    free(heap->dados);
    free(heap);
    return -1;
}

//caso teste
/*
int main() {
    int n = 3, m = 4;
    int gridData[3][4] = {
        {0, 1, 3, 2},
        {5, 1, 2, 5},
        {4, 3, 8, 6}
    };

    int* grid[n];
    for (int i = 0; i < n; i++) {
        grid[i] = gridData[i];
    }

    int result = minimumTime(grid, n, m);
    printf("Minimum time to visit the cell: %d\n", result);

    return 0;
}
*/