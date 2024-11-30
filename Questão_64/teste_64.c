#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Definição da estrutura MinHeap
typedef struct {
    int cost, x, y;
} Node;

typedef struct {
    Node *data;
    int size;
    int capacity;
} MinHeap;

MinHeap* createHeap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->data = (Node*)malloc(sizeof(Node) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap(Node* a, Node* b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap* heap, int index) {
    while (index > 0 && heap->data[index].cost < heap->data[(index - 1) / 2].cost) {
        swap(&heap->data[index], &heap->data[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

void heapifyDown(MinHeap* heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->data[left].cost < heap->data[smallest].cost)
        smallest = left;
    if (right < heap->size && heap->data[right].cost < heap->data[smallest].cost)
        smallest = right;

    if (smallest != index) {
        swap(&heap->data[index], &heap->data[smallest]);
        heapifyDown(heap, smallest);
    }
}

void insertHeap(MinHeap* heap, int cost, int x, int y) {
    if (heap->size == heap->capacity) return;
    heap->data[heap->size++] = (Node){cost, x, y};
    heapifyUp(heap, heap->size - 1);
}

Node extractMin(MinHeap* heap) {
    Node minNode = heap->data[0];
    heap->data[0] = heap->data[--heap->size];
    heapifyDown(heap, 0);
    return minNode;
}

int isEmpty(MinHeap* heap) {
    return heap->size == 0;
}

int minPathSum(int** grid, int gridSize, int* gridColSize) {
    int direcoes[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    int linhas = gridSize, colunas = *gridColSize;

    int** dist = (int**)malloc(linhas * sizeof(int*));
    for (int i = 0; i < linhas; ++i) {
        dist[i] = (int*)malloc(colunas * sizeof(int));
        for (int j = 0; j < colunas; ++j) {
            dist[i][j] = INT_MAX;
        }
    }
    dist[0][0] = grid[0][0];

    MinHeap* heap = createHeap(linhas * colunas);
    insertHeap(heap, grid[0][0], 0, 0);

    while (!isEmpty(heap)) {
        Node current = extractMin(heap);
        int x = current.x, y = current.y, cost = current.cost;

        if (x == linhas - 1 && y == colunas - 1) {
            for (int i = 0; i < linhas; ++i) free(dist[i]);
            free(dist);
            free(heap->data);
            free(heap);
            return cost;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = x + direcoes[i][0];
            int ny = y + direcoes[i][1];

            if (nx >= 0 && ny >= 0 && nx < linhas && ny < colunas) {
                int newCost = cost + grid[nx][ny];
                if (newCost < dist[nx][ny]) {
                    dist[nx][ny] = newCost;
                    insertHeap(heap, newCost, nx, ny);
                }
            }
        }
    }

    return -1;
}
