// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

// FUNCIONA, LISTO PARA ENTREGAR


/*
Ejercicio 12

Realiza un programa que admita un argumento, que será el nombre de un directorio, el cual se deberá abrir y recorrer mostrando todas las 
entradas que contenga, al menos el nombre de archivo, y el iNode (consultar función “opendir” y similares).

Amplia después el programa para que pueda admitir un argumento opcional que será “-r”, y que si aparece indicará que la exploración 
del directorio será recursiva, es decir, que si alguna entrada de las existentes en el directorio indicado en el primer argumento, 
es a su vez, de tipo directorio (excluyendo las entradas “.” y “..”), deberá recorrerlo para mostrar, su contenido, y así sucesivamente.

*** En la exploración recursiva te puede ser muy útil la función “chdir”.

*** Atención: Ten especial cuidado si pruebas el programa con directorios que contengan enlaces (hard o soft) a directorios formando 
ciclos, pues seguramente provocará que el programa acabe bruscamente por un fallo de memoria en la pila de ejecución (STACK).


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
#include <dirent.h>     // Para operar con los directorios


// Main
int main(int argc, char *argv[]){

    DIR *directorio;        // El puntero a directorio
    struct dirent *direntp; //Para recorrer el directorio


    // admitimos el nombre de un unico fichero
    if(argc != 2){
        fprintf(stderr, "Numero de parametros erroneo. Nada que hacer.\n");
        exit -1;
    }else{

        // Abrimos el directorio
        directorio = opendir(argv[1]);
        if (directorio == NULL){
            fprintf(stderr, "ERROR. No se puede abrir el directorio. Saliendo.\n");
            exit -1;
        }else{
        // prototipo para leer el directorio 
        // struct dirent *readdir(DIR *dirp);
        fprintf(stdout,"i-nodo\t\tlongitud\tnombre\n");
        fprintf(stdout,"------------------------------------------\n");
        //Leemos el directorio y guardamos en la estructura direntp
        while ((direntp = readdir(directorio)) != NULL) {
            //Mostramos por la salida estandar el resultado de la operación
            fprintf(stdout,"%ld\t\t%d\t\t%s\n", direntp->d_ino, direntp->d_reclen, direntp->d_name);
        }

        //Cerramos el directorio
        closedir(directorio);
        }
    }

}// END MAIN