#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define N 100
#define P 6

typedef struct
{
    int **matriz;
    int n;
    int step;
    int rstep;
    int lim;
    double *shared_array;
} Data;

void imprimir(int **matriz)
{

    int size = sizeof(matriz) / sizeof(int *);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%i  ", matriz[i][j]);
        }
        printf("\n");
    }
}

int main()
{

    time_t t;
    srand(time(&t));
    double *shared_array;
    int shmid;
    int **matriz = (int **)malloc(sizeof(int *) * N);

    for (int i = 0; i < N; i++)
    {
        matriz[i] = (int *)malloc(sizeof(int) * N);
        for (int j = 0; j < N; j++)
        {
            matriz[i][j] = (int)(rand() % 100);
        }
    }
    //Compartiendo memoria
    if ((shmid = shmget(IPC_PRIVATE, P * sizeof(double), IPC_CREAT | 0666)) < 0) {
        perror("Error al crear el segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Adjuntar el segmento de memoria compartida al espacio de direcciones del proceso
    if ((shared_array = (double *)shmat(shmid, NULL, 0)) == (double *) -1) {
        perror("Error al adjuntar el segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    // distribucion de filas
    int p_r = (int)floor(N / P);   // 1.66
    int paso = p_r == 0 ? 1 : p_r; // 1 --> una fila por nucleo
    int pasor = N % P;             // 6
    int lim = (p_r == 0) ? pasor : P;

    Data data;
    data.matriz = matriz;
    data.lim = lim;
    data.step = paso;
    data.rstep = pasor;
    data.shared_array = shared_array;

    printf("p_r   -> %i\n", p_r);
    printf("paso  -> %i\n", paso);
    printf("pasor -> %i\n", pasor);
    printf("lim   -> %i\n", lim);

    pid_t pid = 1;
    for (int i = 0; i < lim; i++)
    {
        
        if(pid){
            printf("Proceso padre %i\n", getpid());        
            if ((P - 1) == i)
            {
                paso += pasor;
            }
            
            pid = fork();

        }
        
        if(pid == 0){
            printf("Proceso hijo {%i : %i} -> %i\n", i, paso, getpid());
            data.shared_array[i] = i;
            exit(0);
        }

    }


    for(int i = 0 ; i < P; i++){
        printf("%f\n", shared_array[i]);
    }


    return 0;
}
