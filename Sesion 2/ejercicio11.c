// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

// FUNCIONA, LISTO PARA ENTREGAR


/*
Ejercicio 11

Realiza un programa que admita uno o varios argumentos, los cuales serán 
nombres de archivo, y para cada uno de ellos muestre en pantalla su nombre 
y longitud en bytes. Programadlo sin utilizar el posicionamiento de 
archivos (consultar función “stat” y similares). 

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


// Main
int main(int argc, char *argv[]){

    int i;
    struct stat dat_archivo; // Estructura donde almacenamos los datos del archivo

    if(argc == 1){
        fprintf(stderr, "No se han introducido ningun parametro. Saliendo.\n");
        exit -1;
    }

    for(i=1;i<argc;i++){

        // Definicion de lstat en la pagina de man
        //int lstat(const char *pathname, struct stat *statbuf);

        if (lstat(argv[i], &dat_archivo) == -1) {
               fprintf(stderr, "Error en el archivo %s\n", argv[i]);
               exit -1;
        }else{
            printf("Tamanio del archivo:\t %lld bytes del archivo %s \n",(long long) dat_archivo.st_size, argv[i]);
        }


    }

           
}// END MAIN