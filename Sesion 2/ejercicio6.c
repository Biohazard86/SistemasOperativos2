// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI


/*
Ejercicio 6

Realizar un programa que deberá admitir un argumento, que será el nombre de un archivo,
el cual abrirá para lectura y escritura, mostrará en pantalla que va a bloquear el archivo, 
y tras esto activará un bloqueo sobre los 20 primeros bytes del mismo, esperará unos 25 segundos 
más un número aleatorio entre 5 y 15 segundos (usar por esta vez la función “sleep”), y cuando 
continúe mostrará en pantalla que va a desbloquear el archivo, liberará el bloqueo, y terminará cerrando el fichero.

*** Ejecuta este programa simultáneamente en varios terminales sobre el mismo fichero dado como argumento, 
y comprueba que el primer programa hace el bloqueo y el resto se quedan esperando hasta que se libera.


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



// Main
int main(int argc, char *argv[]){
    int fd;     // File descriptor, vamos a guardar el descriptor del archivos que abrimos
    int tiempo; // Tiempo que se va a pausar
    int bloqueado = 1;
    srand(time(NULL));  // Para poder generar los numeros aleatorios diferentes, la semilla es TIME

    if(argc != 2){
        // Si el numero de parametros es diferente al que se necesitan, se sale del programa
        fprintf(stderr, "ERROR. Numero de parametros erroneo.\n");
        exit -1;
    }
    
    // Abrimos el archivo pasado como parametro
    fd = open(argv[1], O_WRONLY | O_RDONLY);
    if(fd == -1){
        // Si al intentar abrir el archivo, nos devuelve un -1, es que ha habido un error
        fprintf(stderr, "Ha habido un error al intentar abrir el archivo. Saliendo.\n");
        exit -1;
    }
    else{
        // Mostramos el mensaje
        fprintf(stdout, "Se va a bloquear el archivo.\n");
        // Bloqueamos el archivo (20 bytes)
        do{
            if((lockf(fd, F_TEST, 20)) == -1){
                bloqueado == 1; // No es necesario pero recordamos que 1 manitiene el do-while
                // Si retorna -1, es que esta bloqueado
                fprintf(stderr, "Archivo bloqueado. Reintentando.\n");
                // Vamos a dormir el proceso 1 segundo
                sleep(1);

            }
            else{
                bloqueado = 0;
                // Si retorna algo diferente, es que no esta bloqueado
                lockf(fd, F_LOCK, 20);
                tiempo = rand () % (15-5+1) + 5;   // Este está entre 5 y 15
                fprintf(stdout, "El archivo estara bloqueado por %d segundos\n", tiempo+25);
                sleep(25+tiempo);

            }
        }while(bloqueado == 1); // Tenemos un flag para saber si esta bloqueado o no. Si esta bloqueado no cambia el flag.
        
        fprintf(stdout, "Se procede a desbloquear el archivo\n");
        lockf(fd, F_ULOCK, 20);     // Desbloqueamos el archivo
        fprintf(stdout, "El archivo se ha desbloqueado. Cerrando archivo y saliendo.\n\n");
        close(fd);          // Cerramos el archivo


    }
    

    
           
}// END MAIN