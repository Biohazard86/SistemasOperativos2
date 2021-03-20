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

    int estado, i, num;
    char numero[1];

    
    // Comprobamos que se introducen dos argumentos en la linea de ordenes
    if(argc != 3){
        fprintf(stdout, "No se han introducido los parametros de forma correcta. Saliendo.\n");
        exit -1;
    }
    else{
        // Creamos un directorio con el nombre del primer parametro.
        // Prototipo de la funcion mkdir
        // int mkdirat(int dirfd, const char *pathname, mode_t mode);
        // Damos permisos de lectura, escritura y ejecucion para usuario, grupo y otros.
        estado = mkdir(argv[1], S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
        if(estado == -1){
            // Si no se puede crear el directorio, entonces mostramos mensaje de error y salimos.
            fprintf(stderr, "No se ha podido crear el directorio %s. Saliendo.\n", argv[1]);
            exit -1;
        }
        else{
            fprintf(stdout, "Se ha creado el directorio '%s' con exito\n", argv[1]);

            strcpy(numero, argv[2]);
            num = atoi(numero);
            // Comprobamos que el segundo parametro sea correcot
            if((num >2) && (num <31)){
                for(i=0; i<num; i++){
                // Creamos los archivos desde 0 hasta argv[2]
                fprintf(stdout, "Se ha creado el archivo %d\n", i);
            }

            }else{
                fprintf(stderr, "Error en el segundo parametro. Saliendo\n");
                exit -1;
            }

            
        }


    } // END ELSE(IF)

           
}// END MAIN
