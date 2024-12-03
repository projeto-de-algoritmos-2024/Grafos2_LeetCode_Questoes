#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// struct da aresta
typedef struct {
    int destino, peso;
} Aresta;

// struct do Grafo
typedef struct {
    Aresta** adj;
    int* tamanhos;
    int nos;
} Graph;

// struct para cada célula de heap
typedef struct {
    int distancia, no;
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
        if (heap->dados[indice].distancia >= heap->dados[pai].distancia) break;
        swap(&heap->dados[indice], &heap->dados[pai]);
        indice = pai;
    }
}

void heapify(Heap* heap, int indice) {
    while (2 * indice + 1 < heap->tamanho) {
        int esquerda = 2 * indice + 1;
        int direita = 2 * indice + 2;
        int menor = esquerda;
        if (direita < heap->tamanho && heap->dados[direita].distancia < heap->dados[esquerda].distancia) {
            menor = direita;
        }
        if (heap->dados[indice].distancia <= heap->dados[menor].distancia) break;
        swap(&heap->dados[indice], &heap->dados[menor]);
        indice = menor;
    }
}

void redimensionarHeap(Heap* heap) {
    heap->capacidade *= 2;
    heap->dados = (Celula*)realloc(heap->dados, heap->capacidade * sizeof(Celula));
}

void inserirHeap(Heap* heap, Celula celula) {
    if (heap->tamanho == heap->capacidade) {
        redimensionarHeap(heap);
    }
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

// Funções do grafo
    Graph* graphCreate(int n, int** edges, int edgesSize, int* edgesColSize) {
        Graph* grafo = (Graph*)malloc(sizeof(Graph)); // aloca memoria do grafo
        grafo->nos = n;
        grafo->adj = (Aresta**)malloc(n * sizeof(Aresta*)); // aloca memoria da lista de adjacencia das arestas
        grafo->tamanhos = (int*)calloc(n, sizeof(int));  // inicializa os tamanhos das listas de adjacência com zero

        // inicializa as listas de adjacência com NULL 
        for (int i = 0; i < n; i++) {
            grafo->adj[i] = NULL;
        }

        // adiciona arestas iniciais no grafo
        for (int i = 0; i < edgesSize; i++) {
            int u = edges[i][0], v = edges[i][1], peso = edges[i][2];
            grafo->adj[u] = (Aresta*)realloc(grafo->adj[u], (grafo->tamanhos[u] + 1) * sizeof(Aresta));
            grafo->adj[u][grafo->tamanhos[u]++] = (Aresta){v, peso};
        }

        return grafo;
    }

    void graphAddEdge(Graph* obj, int* edge, int edgeSize) {
        int u = edge[0], v = edge[1], peso = edge[2]; // lê origem, destino e peso da nova aresta

        // Expande a lista de adjacência para adicionar a nova aresta
        obj->adj[u] = (Aresta*)realloc(obj->adj[u], (obj->tamanhos[u] + 1) * sizeof(Aresta));
        obj->adj[u][obj->tamanhos[u]++] = (Aresta){v, peso}; // Adiciona a aresta ao vetor
    }
    int graphShortestPath(Graph* obj, int node1, int node2) {
        // cria vetor par armazenar distancias
        int* distancia = (int*)malloc(obj->nos * sizeof(int));
        for (int i = 0; i < obj->nos; i++) distancia[i] = INT_MAX;
        distancia[node1] = 0;

        Heap* heap = criarHeap(obj->nos * 2); // Capacidade inicial maior
        inserirHeap(heap, (Celula){0, node1}); // Insere o nó inicial no heap

        while (heap->tamanho > 0) {
            Celula atual = extrairMin(heap);
            int u = atual.no, d = atual.distancia;

            if (d > distancia[u]) continue; //verifica se chegou no destino

            // explora as arestas
            for (int i = 0; i < obj->tamanhos[u]; i++) {
                Aresta aresta = obj->adj[u][i];
                int v = aresta.destino, peso = aresta.peso;
                // atualiza a distancia
                if (distancia[u] + peso < distancia[v]) {
                    distancia[v] = distancia[u] + peso;
                    inserirHeap(heap, (Celula){distancia[v], v});
                }
            }
        }

        int resultado = distancia[node2] == INT_MAX ? -1 : distancia[node2];
        free(distancia);
        free(heap->dados);
        free(heap);
        return resultado;
    }


    void graphFree(Graph* obj) {
        for (int i = 0; i < obj->nos; i++) {
            free(obj->adj[i]);
        }
        free(obj->adj);
        free(obj->tamanhos);
        free(obj);
}
