#include <stdio.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void print(int **matrix, int pid){
    
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            printf("%i  ",*(*(matrix + i) + j));
        }
        printf("\n");
    }
  
}

void countFrom(int n){
    for(int i = n; i < 10; i++){
        printf("%d",i++);
    }
}


int main(int argc, char *argv[])
{

    int **matrix = (int **)malloc(sizeof(int *)*4);
    for(int i = 0; i < 4; i++){
        matrix[i] = (int *)malloc(sizeof(int)*4);
        for(int j = 0; j < 4; j++){
            matrix[i][j] = j + i;
        }
    }


    pid_t pid = fork();
    int count = 1;
    for(int i = 0; i < 4; i++){
        if(pid > 0){
            
            printf("Instancia fork %d\n",i);
            pid = fork();
            count++;

        }else printf("Este es el proceso hijo %i\n",i);
        
    }

    if(pid == 0){
        exit(1);
    }
    
    printf("%d\n",count);


    return 0;
}

