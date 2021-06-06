// LIBRERIAS
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int i, valor_devuelto, pid_padre, estado;
    pid_t pid;

    
    printf("\n\n");
    fprintf(stdout, "ID Proceso que llama a los hijos... %d \n\n", getpid());

    for(i=0;i<7;i++){
        
        valor_devuelto = fork();
        switch (valor_devuelto){
        case -1:
            fprintf(stderr, "ERROR al crear el proceso %d \n", i);
            break;

        // Codigo del hijo
        case 0:
            fprintf(stdout, "-----------------------------------------\n");
            fprintf(stdout, "\t Soy el hijo %d con ID = %d\n", i, getpid());
            fprintf(stdout, "\t Y mi padre es ID = %d\n", getppid());
            sleep(1);
            return 0;
            break;
        
        default:
            
            fprintf(stdout, "Soy el padre con ID = %d\n", getpid());
            fprintf(stdout, "Y he creado al hijo %d\n", valor_devuelto);
            waitpid( -1, &valor_devuelto, 0);
            //return 0;
            

            }
        
    }
}