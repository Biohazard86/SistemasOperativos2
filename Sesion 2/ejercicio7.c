// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI


/*
Ejercicio 7

Realiza un programa que deberá admitir el nombre de un fichero como argumento. Con el nombre de fichero, lo abrirá para lectura 
y creará uno para escritura con el mismo nombre pero acabado en ".copia". Tras esto, hará una copia completa del fichero incial 
y lo cerrará. Con el fichero de copia, se situará al inicio del mismo, y lo recorrerá sustituyendo el carácter ‘a’ por ‘o’, 
pero realizando esta tarea con proyección de memoria en bloques de 256 bytes (o del tamaño mínimo que permita el sistema operativo), 
no del tamaño del archivo completo. Al acabar, deshará la proyección y cerrará el archivo

*/

// Librerias usadas
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>   // Para los numeros aleatorios
#include <sys/mman.h>
#define TAM_BUFF 64



// Main
int main(int argc, char *argv[]){
    FILE *fich_original, *fich_destino;
    int fd1, fd2;     // File descriptor, vamos a guardar el descriptor del archivos que abrimos
    int fin_fichero, *map_memoria;
    char extension[] = ".copia";
    char nombre[100], arr;
    char buff[TAM_BUFF];
    int posicion_actual = 0, retorno_lectura_escritura, tam_leido, tam_escrito;
    strcpy(nombre, argv[1]);
    fprintf(stdout, "1 - %s FIN\n", nombre);
    strcat(nombre, extension);
    fprintf(stdout, "2 - %s FIN\n", nombre);
    

    if( argc !=2){
        fprintf(stderr, "Numero de parametros incorrecto. Saliendo.\n");
        exit -1;
    }
    else{
        fd1 = open(argv[1], O_RDONLY);      // abrimos el archivo en modo lectura.
        if(fd1 == -1){
            fprintf(stderr, "No se ha podido abrir el primer archivo. Saliendo.\n");
            exit -1;
        }
        else{
            fin_fichero = lseek( fd1, 0, SEEK_END); // Ponemos el puntero de archivo al final de este y guardamos dicha direccion
            //fprintf(stdout, "FIN DEL FICHERO: %d\n", fin_fichero);
            /*
            map_memoria = mmap ( NULL, fin_fichero, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 ); // Proyectamos en memoria el archivo y guardamos la direccion de memoria donde lo guradamos
            if(map_memoria == MAP_FAILED){      // Si el mapeo sale mal
                fprintf(stderr, "ERROR en el mapeo. Saliendo.\n");     // Mostramos un mensaje de error por el canal de errores
                exit -1;    // y salimos del programa
            }
            else{
                fprintf(stdout, "Mapeo realizado con exito\n");
            }
            */
            fd2 = open(nombre , O_CREAT || O_WRONLY);
            //if(fd2 = open(nombre , O_CREAT || O_WRONLY) == -1);      // Creamos el archivo con nombre "nombre".
            //{
            //    fprintf(stderr, "ERROR al crear el archivo %s\n", nombre);
            //}
            if(fchmod(fd2, 0777) == -1){ //cambiamos los permisos del archivo
                // Si ha habido algun error mostramos el mensaje siguiente por el canal de errores
                fprintf(stderr, "ERROR al modificar los permisos del archivo %s\n", nombre);
            }
            else{
                // Vamos a leer cadenas de 64 caracteres de cada vez

                lseek( fd1, 0, SEEK_SET);

                posicion_actual = 0;
                while((tam_leido = read(fd1, buff, TAM_BUFF)) > 0){
                    
                    if((tam_leido < TAM_BUFF) && (tam_leido >0)){
                        // Informamos cual es la utima vez que se lee elarchivo y cual es su extension
                        fprintf(stdout, "Se prevee como ultima vez leida, de %d/%d posiciones\n", tam_leido, TAM_BUFF);
                    }
                    if(tam_leido == -1){
                        fprintf(stderr, "Error al LEER del primer archivo\n");
                        //   exit -1;
                    }
                    if(tam_leido == TAM_BUFF){
                        fprintf(stdout, "Se han leido %d\n", tam_leido);
                        // Si el tam leido es igual al del buff significa que va a haber mas lecturas
                        posicion_actual = posicion_actual + TAM_BUFF;
                        lseek(fd1,posicion_actual, SEEK_SET);
                        lseek(fd2,posicion_actual, SEEK_SET);
                    }
                    //fprintf(stdout, "Se han leido %d\n", tam_leido);
                    tam_escrito = write(fd2, buff, tam_leido);

                    if(tam_escrito == -1){
                        fprintf(stderr, "Error al ESCRIBIR en el primer archivo\n");
                        //   exit -1;
                    }
                    if(tam_escrito == tam_leido){
                        fprintf(stdout, "\nExito en la escritura.\n");
                    }
                    
                    

                }    
                


                close(fd1);      // Cerramos el archivo
                close(fd2);     // Cerramos el segundo archivo
            }   



            
        }
        //strcat(nombre,extension);
        
        // Hay que cambiar permisos despues de crear el archivo
        


        
        if(fd2 == -1){
            fprintf(stderr, "No se ha podido abrir el segundo archivo. Saliendo.\n");

            exit -1;
        }

    }
    
           
}// END MAIN