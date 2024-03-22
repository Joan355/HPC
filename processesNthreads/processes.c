#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>
#define MAX_THREADS 6
typedef struct {
    int id;
    int inicio;
    int fin;
    int **matriz1;
    int **matriz2;
    int **resultado;
    int n;
    int num_threads;
} ThreadData;


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
void *multiplicarMatrices(void *d) {
    ThreadData *data = (ThreadData *)d;

    for (int i = data->inicio; i < data->fin; i++) {
        for (int j = 0; j < data->n; j++) {
            data->resultado[i][j] = 0;
            for (int k = 0; k < data->n; k++) {
                data->resultado[i][j] += data->matriz1[i][k] * data->matriz2[k][j];
            }
        }
    }
    
    //exit(0);
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
    int t = atoi(argv[2]);
    int **matriz1 = generarMatriz(n);
    int **matriz2 = generarMatriz(n);
    pthread_t threads[t];
    ThreadData thread_data[t];
    struct timeval start_time, end_time;
    double execution_time;
    int **resultado = (int **)malloc(sizeof(int *) * n);
  
    for(int i = 0; i < n; i++){
        resultado[i] = (int *)malloc(sizeof(int) * n);
    }

    int t_r = (int)floor(n / t);
    int step = t_r == 0 ? 1 : t_r;
    int r_step = n % t;
    int lim = (t_r == 0) ? r_step : t;


    gettimeofday(&start_time, NULL);
    

    for(int i = 0; i < t; i++){
        thread_data[i].id = i;
        thread_data[i].n = n;
        thread_data[i].inicio = i * step;
        thread_data[i].fin = ((t - 1) == i) ? (i*step + r_step + step): (i*step + step);
        thread_data[i].matriz1 = matriz1;
        thread_data[i].matriz2 = matriz2;
        thread_data[i].resultado = resultado;
        thread_data[i].num_threads = t;
        pthread_create(&threads[i], NULL, multiplicarMatrices, (void *)&thread_data[i]);
    }

    for (int i = 0; i < t; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end_time, NULL);
    execution_time = (double)(end_time.tv_sec - start_time.tv_sec) +
                     (double)(end_time.tv_usec - start_time.tv_usec) / 1000000.0;

    printf("%.6f\n", execution_time);

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
