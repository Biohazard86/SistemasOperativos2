// LIBRERIAS
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>

int main (int argc, char *argv[]){
    
    int numero, valor_devuelto, i, estado;

    // Hacemos las comprobaciones 
    if(argc == 1){
        fprintf(stdout, "No se han introducido parametros\n");
    }
    else if(argc == 2){
        numero = atoi(argv[1]);
        fprintf(stdout, "El numero introducido es %d\n", numero);
        if((numero >30) || (numero <5)){
            fprintf(stderr, "Fuera de rango\nSALIENDO\n");
            return 0;
        }
    }
    else{
        fprintf(stdout, "Se han introducido parametros de mas\n");
    }



    // Bucle para crear los hijos
    for(i=0;i<numero;i++){
        
        valor_devuelto = fork();
        switch (valor_devuelto){
        case -1:
            fprintf(stderr, "ERROR al crear el proceso %d \n", i);
            break;

        // Codigo del hijo
        case 0:
            fprintf(stdout, "-----------------------------------------\n");
            fprintf(stdout, "\t Soy el hijo número %d y mi PID es %d\n", i, getpid());
            
            sleep(1);
            return 0;
            break;
        
        default:
            
            waitpid( -1, &valor_devuelto, 0);

            fprintf(stdout, "\t El hijo numero %d con PID %d ha acabado con estado %d\n", i, valor_devuelto, WIFEXITED( estado));
            //return 0;
            

            }
        
    }
}