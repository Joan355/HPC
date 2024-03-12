#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Función para generar una matriz cuadrada de tamaño n con números aleatorios del 1 al 10
int **generarMatriz(int n) {
    int **matriz = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        matriz[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            matriz[i][j] = rand() % 10 + 1; // Números aleatorios del 1 al 10
        }
    }
    return matriz;
}

// Función para multiplicar dos matrices cuadradas de tamaño n
int **multiplicarMatrices(int **matriz1, int **matriz2, int n) {
    int **resultado = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        resultado[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            resultado[i][j] = 0;
            for (int k = 0; k < n; k++) {
                resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }
    return resultado;
}

// Función para imprimir una matriz cuadrada de tamaño n
void imprimirMatriz(int **matriz, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if (argc < 2 || argc > 3 || atoi(argv[1]) == 0) {
        printf("Uso: %s <tamaño de matriz> [-p]\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int **matriz1 = generarMatriz(n);
    int **matriz2 = generarMatriz(n);

    clock_t inicio = clock(); // Tiempo de inicio

    int **resultado = multiplicarMatrices(matriz1, matriz2, n);

    clock_t fin = clock(); // Tiempo de fin
    double tiempo_total = (double)(fin - inicio) / CLOCKS_PER_SEC; // Tiempo total en segundos

    printf("%.6f\n", tiempo_total);

    if (argc == 3 && strcmp(argv[2], "-p") == 0) {
        printf("\nMatriz 1:\n");
        imprimirMatriz(matriz1, n);
        printf("\nMatriz 2:\n");
        imprimirMatriz(matriz2, n);
        printf("\nResultado de la multiplicación:\n");
        imprimirMatriz(resultado, n);
    }

    // Liberar memoria
    for (int i = 0; i < n; i++) {
        free(matriz1[i]);
        free(matriz2[i]);
        free(resultado[i]);
    }
    free(matriz1);
    free(matriz2);
    free(resultado);

    return 0;
}
