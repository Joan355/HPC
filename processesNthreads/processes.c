#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

int p = 0; // imprimir matriz resultado


int isNumber(char *cadena); // Es numero
int verifyArg(int args,char *argsv[]); //verificar argumentos
int **genMtx(int n) ;//generar matriz
void printMtx(int **matriz, int n); //imprimir matriz
void freeMemory(int **matriz, int n); //liberar memoria
int **mulMtx(int **matrizA, int **matrizB, int n); 
//------------------------------------MAIN---------------------------------------
int main(int args, char *argsv[]) {

    if(verifyArg(args, argsv) == 0){
        return 0;
    }
    int n;
    n = atoi(argsv[1]);
    int **matrizA = genMtx(n);
    int **matrizB = genMtx(n);
    int **result = NULL;     

    //TODO:
    /*
        1. Multiplicar matrices
            1.1 Testing
        2. Implementar fork y mmap para compartir memoria entre procesos
        3. implementar funcion que registre los tiempos en un csv
        4. desarrollar shell script para testing de multiplicacion de matrices
    */
    result = mulMtx(matrizA,matrizB,n);
    if(p == 1){
        printMtx(result,n);        
    }

    freeMemory(matrizA,n);
    freeMemory(matrizB,n);
    freeMemory(result,n);
    return 0;
}
//------------------------------------END MAIN---------------------------------------
int isNumber(char *cadena){
    char *aux = cadena;
    if (*cadena == '+' || *cadena == '-')
        cadena++;
    // Verificar si todos los caracteres restantes son dígitos
    while (*cadena != '\0') {
        if (!isdigit(*cadena))
            return 0;
        cadena++;
    }
    if(atoi(aux) < 0){
        return -1;
    }else return 1;
    
}
int verifyArg(int args,char *argsv[]){

    if(args > 1){
        int pos;
        if(!(pos = isNumber(argsv[1]))){
            fprintf(stderr,"El argumento debe ser un numero.\n");
            return 0;
        }
        
        if(pos < 0){
            fprintf(stderr,"Se espera que el argumento sea mayor a cero\n");
            return 0;
        }

        if(args > 2){
            if(strcmp(argsv[2],"p") != 0){
                fprintf(stderr,"El comando %s no está definido.\n",argsv[2]);
            }else p = 1;//Se imprime la matriz resultante.
        }
    }else{
        fprintf(stderr, "Se espera como argumento el tamaño de la matriz.\n");
        return 0;
    }

    return 1;

}
int **genMtx(int n) {
    // Reservar memoria para la matriz
    int **matriz = (int **)malloc(n * sizeof(int *));
    if (matriz == NULL) {
        fprintf(stderr,"Error: No se pudo asignar memoria para la matriz.\n");
        exit(1);
    }

    // Reservar memoria para cada fila de la matriz
    for (int i = 0; i < n; i++) {
        matriz[i] = (int *)malloc(n * sizeof(int));
        if (matriz[i] == NULL) {
            fprintf(stderr,"Error: No se pudo asignar memoria para la fila %d de la matriz.\n", i);
            exit(1);
        }
    }

    
    // Llenar la matriz con valores aleatorios
    srand(time(NULL)); // Inicializar la semilla para generar valores aleatorios
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matriz[i][j] = rand() % 10; // Generar un valor aleatorio entre 0 y 99
        }
    }

    return matriz;
}
void printMtx(int **matriz, int n) {
    printf("Matriz %dx%d:\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
    }
}
void freeMemory(int **matriz, int n) {
    for (int i = 0; i < n; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

int **mulMtx(int **matrizA, int **matrizB, int n) {
    int **resultado = (int **)malloc(n * sizeof(int *));
    if (resultado == NULL) {
        fprintf(stderr,"Error: No se pudo asignar memoria para la matriz resultado.\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        resultado[i] = (int *)malloc(n * sizeof(int));
        if (resultado[i] == NULL) {
            fprintf(stderr,"Error: No se pudo asignar memoria para la fila %d de la matriz resultado.\n", i);
            exit(1);
        }

        for (int j = 0; j < n; j++) {
            resultado[i][j] = 0;
            for (int k = 0; k < n; k++) {
                resultado[i][j] += matrizA[i][k] * matrizB[k][j];
            }
        }
    }

    return resultado;
}