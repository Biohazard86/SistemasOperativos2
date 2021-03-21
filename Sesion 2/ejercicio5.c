// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI


/*
Ejercicio 5

Realizar un programa que deberá admitir tres argumentos, el primero será el nombre de un archivo que se abrirá para lectura 
(con lo cual deberá existir para que el programa siga adelante), el segundo será el nombre de un archivo que se creará 
(si no existe) y abrirá para escritura, y el tercero será un carácter indicador de la operación a realizar, siendo:

    A = El primer archivo se copiará íntegramente en el segundo, leyendo 1 byte del primero y escribiéndolo en el segundo 
    desde el principio del fichero hasta el final.
    B = Como en la operación “A”, pero usando un buffer de 1KByte, es decir, leyendo y escribiendo en bloques de 1024 
    bytes hasta el final.
    C = El primer archivo se copiará en el segundo como en “A”, pero en orden inverso, es decir, el útimo byte 
    del primer archivo será el primer byte del segundo archivo. Usad para ello el posicionamiento del puntero del archivo, 
    porque... ¡¡No se puede leer el archivo entero en memoria e invertirlo ahí!!

*/

// LE QUEDA POR HACER LO DE LAS OPCIONES


// Librerias usadas
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <time.h>   // Para los numeros aleatorios

// Copia A en B con un buffer de 1 o de 1024
int cosa_que_hace_cosas(char archivo1[], char archivo2[], int numero){
    char buff[numero];
    int fd1, fd2;
    int tam_leido = 1, tam_escrito, posicion_actual=0;
    
    fprintf(stdout, "%s se va a copiar en %s con tamanio de buffer de %d\n", archivo1, archivo2, numero);
    
    fd1 = open(archivo1, O_RDONLY);      // abrimos el archivo en modo lectura.
    fd2 = open(archivo2, O_WRONLY);      // abrimos el archivo en modo lectura.
    if((fchmod(fd2, 0777)) == -1){
        fprintf(stderr, "Error al cambiar los permisos\n");
    }
    lseek( fd1, 0, SEEK_SET);
    lseek( fd2, 0, SEEK_SET);

    do{
        lseek(fd1,posicion_actual, SEEK_SET);
        lseek(fd2,posicion_actual, SEEK_SET);
        
        tam_leido = (read(fd1, buff, numero));
        if(tam_leido != numero){
            exit (1);
        }

        tam_escrito = write(fd2, buff, numero);
        if(numero != tam_escrito){
            fprintf(stderr, "Error en la escritura\n");
        }           
        
        posicion_actual = posicion_actual + numero;
    }while(numero == tam_leido);

    close(fd1);
    close(fd2);
}


int cosa_que_hace_cosas_pero_del_reves(char archivo1[], char archivo2[], int numero){
    char buff[numero];
    int fd1, fd2;
    int tam_leido = 1, tam_escrito, posicion_actual=0;
    int posicion_final;
    
    fprintf(stdout, "%s se va a copiar en %s con tamanio de buffer de %d\n", archivo1, archivo2, numero);
    
    fd1 = open(archivo1, O_RDONLY);      // abrimos el archivo en modo lectura.
    fd2 = open(archivo2, O_WRONLY);      // abrimos el archivo en modo lectura.
    if((fchmod(fd2, 0777)) == -1){
        fprintf(stderr, "Error al cambiar los permisos\n");
    }
    
    posicion_final = lseek( fd1, 0, SEEK_END);
    lseek( fd2, 0, SEEK_SET);

    do{
        //fprintf(stdout, "POSICION %d\n", posicion_final);
        lseek(fd1,posicion_actual, SEEK_SET);
        lseek(fd2,posicion_final, SEEK_SET);
        
        tam_leido = (read(fd1, buff, numero));
        if(tam_leido != numero){
            exit (1);
        }
        
        tam_escrito = write(fd2, buff, numero);

        if(numero != tam_escrito){
            fprintf(stderr, "Error en la escritura\n");
        }           
        posicion_final = posicion_final - numero;
        posicion_actual = posicion_actual + numero;
    }while(numero == tam_leido);

    close(fd1);
    close(fd2);
}

// Main
int main(int argc, char *argv[]){
    int fd1, fd2;     // File descriptor, vamos a guardar el descriptor del archivos que abrimos
    int i;
    char opcion[1];
    
    
    if(argc != 4){
        // Si no son 4 parametros, salimos del programa
        fprintf(stderr, "No se han introducido los parametros requeridos. Saliendo.\n");
        exit -1;
    }else{
        // Si se han introducido 4 parametros entonces:
        // Comprobamos que el primero existe.
        fd1 = open(argv[1], O_RDONLY);      // abrimos el archivo en modo lectura.
        if(fd1 == -1){
            // Si el archivo no existe se sale, ya que es un requisito
            fprintf(stderr, "No se puede abrir el primer archivo.\n");
            exit -1;
        }
        fd2 = open(argv[2], O_WRONLY | O_CREAT);      // abrimos el archivo en modo escritura.
        if(fd1 == -1){
            // Si el archivo existe, se sale, ya que no debe existir.
            fprintf(stderr, "ERROR al tratar el archivo 2.\n");
            exit -1;
        }else{
            // -----------------------------------------------------------------------------------------------

            //fprintf(stderr, "Opcion introducida: %s\n", argv[3]);
            strcpy(opcion, argv[3]);
            opcion[0] = toupper(opcion[0]);
            if((opcion[0] == 'A') || (opcion[0] == 'B') || (opcion[0] == 'C')){
                fprintf(stdout, "Opcion introducida: %s\n", opcion);

                if(opcion[0] == 'A'){
                    // Vamos a leer caracter a caracter
                    cosa_que_hace_cosas(argv[1], argv[2], 1);

                }

                if(opcion[0] == 'B'){
                    // Vamos a leer blqoues de 1024
                    cosa_que_hace_cosas(argv[1], argv[2], 1024);
                }

                if(opcion[0] == 'C'){
                    // Vamos a leer blqoues de 1 del reves
                    cosa_que_hace_cosas_pero_del_reves(argv[1], argv[2], 1);
                }

            }
            else{
                // Si la opcion no es correcta, salimos del programa con un mensaje de error
                fprintf(stderr, "La opcion introducida no es correcta. Saliendo.\n");
                exit -1;
            }

            // -----------------------------------------------------------------------------------------------
        }
        

    }
           
}// END MAIN
