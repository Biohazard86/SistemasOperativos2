// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

// FUNCIONA, LISTO PARA ENTREGAR


/*
Ejercicio 8

Realiza un programa que deberá admitir un mínimo de dos argumentos y sin un máximo definido,
siendo todos ellos nombres de fichero. El primer argumento será el nombre de un fichero que
será enlazado por una serie de archivos. Se creará por cada segundo y siguientes argumentos 
(nombres de archivo), enlaces físicos y simbólicos alternadamente hacia el primer fichero.

*/

// Librerias usadas. No todas
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>   // Para los numeros aleatorios
#include <sys/mman.h>
#define MAX_PARAMETROS 5



// Main
int main(int argc, char *argv[]){

    int i;

    if((argc < 3) || (argc > MAX_PARAMETROS)){
        // Si el numero de argumentos es menor a 2, mostramos el siguiete mensaje de error
        fprintf(stderr, "El numero de parametros es incorrecto.\n");
        exit -1;
    }
    else{
    // Si se cumplen los requisitos    
    for(i=2;i<(argc); i++){
        if(((i+2)%2) == 0){
            // PARA LOS ENLACES FISICOS
            fprintf(stdout, "Se procede a crear un enlace fisico del archivo %s al archivo %s\n",argv[i], argv[1]);
            // Prototipo de la funcion link
            // int link(const char *oldpath, const char *newpath);
            if((link(argv[1], argv[i])) == -1){
                // Si la funcion retorna -1 significa que hubo algun error. 
                fprintf(stderr, "Error al escribir.\n");    // Lo presentamos por el canal de errores
            }

        }else{
            // PARA LOS ENLACES SIMBOLICOS
            fprintf(stdout, "Se procede a crear un enlace simbolico del archivo %s al archivo %s\n",argv[i], argv[1]);
            // Prototipo de la funcion symlink
            // int symlink(const char *oldpath, const char *newpath);
            if((symlink(argv[1], argv[i])) == -1){
                // Si la funcion retorna -1 significa que hubo algun error. 
                fprintf(stderr, "Error al escribir.\n");    // Lo presentamos por el canal de errores
            }
        }



    }//END FOR

    }//END ELSE
    
           
}// END MAIN