// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

// FUNCIONA, LISTO PARA ENTREGAR


/*
Ejercicio 13



Realizar un programa que admita dos argumentos en la línea de ordenes. Primero debe crear un directorio en la ruta activa 
cuyo nombre será el dado como primer argumento, y dentro deberá crear tantos archivos como el número indicado en el 
segundo argumento (mínimo 3, máximo 30). El nombre de los archivos tendrá como patrón el nombre “DatosN.txt”, siendo N el 
numero de secuencia de creación del archivo (1, 2, ..., N-2, N-1, N). En para cada archivo creado se escribirá dentro 10 * N 
veces el conjunto de caracteres de la “A” a la “Z”. Tras esto, se debe listar el contenido del directorio, indicando el 
nombre de cada archivo y su longitud en bytes.

*** En la creación de archivos dentro del directorio creado te puede ser muy útil la función “chdir”.
 

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

    int estado, i, j, num;
    char numero[1];
    char datos[1024] = "Datos .txt";
    char prueba[1] = "A";
    char directorio[2048];

    
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
        estado = mkdir(argv[1], S_IRUSR || S_IWUSR || S_IXUSR || S_IRGRP || S_IWGRP || S_IXGRP || S_IROTH || S_IWOTH || S_IXOTH);
        if(estado == -1){
            // Si no se puede crear el directorio, entonces mostramos mensaje de error y salimos.
            fprintf(stderr, "No se ha podido crear el directorio %s. Saliendo.\n", argv[1]);
            exit -1;
        }
        else{
            //fprintf(stdout, "Se ha creado el directorio '%s' con exito\n", argv[1]);
            strcpy(numero, argv[2]);
            num = atoi(numero);


            //fprintf(stdout, "DATOS>  %s\n", datos);
            //fprintf(stdout, "PRUEBA>  %c\n", prueba[0]);
            //fprintf(stdout, "NUMERO> %s\n", numero);
            //fprintf(stdout, "NUMERO> %d\n", num);
            //fprintf(stdout, "------------------------------\n");
            
            // Copiamos el primer agumento a la cadena directorio
            stpcpy(directorio, argv[1]);
            fprintf(stdout, "Se ha creado el directorio '%s' con exito\n", directorio);
            // Comprobamos que el segundo parametro sea correcto
            // Si el numero introducido como segundo parametro esta entre 3 y 30 entonces >> 
            if((num >2) && (num <31)){

                //Con un for recorremos 
                for(i=0; i<(num); i++){
                // Creamos los archivos desde 0 hasta argv[2]
                

                // Limpiamos la cadena 
                memset(directorio, 0, strlen(directorio));
                stpcpy(directorio, argv[1]);
                /*
                for(j=0; j<(strlen(directorio)); j++){
                    directorio[0] = void;
                }
                */
                datos[5] = '0'+i;
                strcat(directorio, datos);
                 
                //Protipo de la funcion para crear un archivo
                // int creat(const char *pathname, mode_t mode);
                
                if((creat(directorio, O_CREAT)) == -1){
                    fprintf(stdout, "DIR> %s\n", directorio);
                    fprintf(stderr, "ERROR al crear el archivo %s en el directorio %s\n", datos, directorio);
                }else{
                    // Si no ha errores, imprimimos el mensaje de que se ha creado el arcvhivo
                    fprintf(stdout, "Se ha creado el archivo %s \n", datos);
                    chmod(datos, 0777);
                    
                }
            }

            }else{
                fprintf(stderr, "Error en el segundo parametro. Saliendo\n");
                exit -1;
            }

            
        }


    } // END ELSE(IF)

           
}// END MAIN