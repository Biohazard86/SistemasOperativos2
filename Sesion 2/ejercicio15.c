// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

// NO ESTA ACABADO


/*
Ejercicio 14

Realiza un programa que admita un argumento, que será el nombre de un directorio, 
el cual se deberá abrir y recorrer mostrando TODA la información de estado de las 
entradas que contenga (consultar función “opendir” y similares, la función “stat” 
y alguna función de conversión de tiempos a cadena como “ctime”).  

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
#include <dirent.h>



// Main
int main(int argc, char *argv[]){

    DIR *dirp;
    struct dirent *direntp;

    // Abrimos el directorio que nos han pasado como primer parametro
    // prototipo de la funcion opendir
    // DIR *opendir(const char *name);
    dirp = opendir(argv[1]);
    if (dirp == NULL){
        fprintf(stderr,"Error: No se puede abrir el directorio\n");
        exit -1;
    }
    
     /* Leemos las entradas del directorio */
    fprintf(stdout, "i-nodo\toffset\t\tlong\tnombre\n");
    while ((direntp = readdir(dirp)) != NULL) {
        fprintf(stdout, "%lu\t%ld\t%d\t%s\n", direntp->d_ino, direntp->d_off, direntp->d_reclen, direntp->d_name);
    }
           
}// END MAIN