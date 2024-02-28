#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

// Estructura para pasar los datos necesarios a los hijos
typedef struct {
    int **matriz1;
    int **matriz2;
    int **resultado;
    int size;
} Data;

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

// Función para multiplicar una porción de la matriz
void multiplicarPorcion(Data *data, int inicio, int fin) {
    int n = data->size;
    for (int i = inicio; i < fin; i++) {
        for (int j = 0; j < n; j++) {
            data->resultado[i][j] = 0;
            for (int k = 0; k < n; k++) {
                data->resultado[i][j] += data->matriz1[i][k] * data->matriz2[k][j];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (argc != 2) {
        printf("Uso: %s <tamaño de matriz>\n", argv[0]);
        return 1;
    }

    int size = atoi(argv[1]);
    if (size <= 1) {
        printf("Tamaño de matriz inválido.\n");
        return 1;
    }

    // Generar las matrices en el proceso padre
    int **matriz1 = generarMatriz(size);
    int **matriz2 = generarMatriz(size);

    // Crear la memoria compartida para el resultado
    int shm_id = shmget(IPC_PRIVATE, size * sizeof(int *) + size * size * sizeof(int), IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("Error en shmget");
        return 1;
    }
    int **resultado = (int **)shmat(shm_id, NULL, 0);
    if (resultado == (int **)-1) {
        perror("Error en shmat");
        return 1;
    }
    for (int i = 0; i < size; i++) {
        resultado[i] = (int *)(resultado + size) + i * size;
    }

    // Inicializar la estructura de datos para compartir con los hijos
    Data data;
    data.matriz1 = matriz1;
    data.matriz2 = matriz2;
    data.resultado = resultado;
    data.size = size;

    clock_t start_time = clock(); // Tiempo de inicio

    // Crear procesos hijos
    int num_hijos = size;
    pid_t pid;
    for (int i = 0; i < num_hijos; i++) {
        pid = fork();
        if (pid == 0) { // Proceso hijo
            multiplicarPorcion(&data, i, i + 1);
            exit(0);
        } else if (pid < 0) {
            perror("Error en fork");
            return 1;
        }
    }

    // Esperar a que todos los procesos hijos terminen
    for (int i = 0; i < num_hijos; i++) {
        wait(NULL);
    }

    clock_t end_time = clock(); // Tiempo de fin
    double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC; // Tiempo total en segundos

    printf("Tiempo de ejecución en el proceso padre: %.6f segundos\n", total_time);

    // Imprimir el resultado en el proceso padre
    /*
    printf("\nResultado de la multiplicación:\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d\t", matriz1[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d\t", matriz2[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d\t", resultado[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    */
    // Liberar la memoria compartida
    shmdt(resultado);
    shmctl(shm_id, IPC_RMID, NULL);

    // Liberar memoria de las matrices
    for (int i = 0; i < size; i++) {
        free(matriz1[i]);
        free(matriz2[i]);
    }
    free(matriz1);
    free(matriz2);

    return 0;
}
