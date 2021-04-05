// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

// FUNCIONA, LISTO PARA ENTREGAR


/*
Ejercicio 5: Creación en Cadena de Procesos, Espera ordenada y Valores de Retorno



Realiza un programa que recibirá dos argumentos numéricos, el primero será el número de niveles de creación (N), 
y el segundo será el tiempo (T) de espera de cada hijo antes de terminar. Los límites de cada argumento serán: N 
de 1 a 5, y T de 1 a 10. El proceso inicial deberá detectar y validar los argumentos antes de continuar, y luego 
creará 2 procesos hijos, los cuales serán hijos de nivel 1. Los hijos creados, comprobarán si son procesos del 
último nivel (N), y si no lo son, crearán a su vez 2 procesos hijo y éste será el nivel 2, y así sucesivamente 
hasta que se llegue al nivel N, momento en el que los hijos creados para ese nivel detectarán que son procesos 
de "último nivel", y no crearán ningún hijo, sólo mostrarán el mensaje que se indica más adelante, esperarán el 
tiempo (T) y acabarán ordenadamente devolviendo el valor 1 al sistema. El proceso padre inicial y los procesos de 
niveles intermedios esperarán por la terminación de todos sus hijos creados, acumulando los valores devueltos por 
ellos, mostrando el mensaje descrito más adelante y acabando ordenadamente devolviendo la suma acumulada al sistema. 
Todos los procesos antes de su terminación, deben mostrar en pantalla el mensaje "Proceso P, Nivel N, Hijo H, Suma S." 
siendo P el PID del proceso que acaba, N el nivel al que pertenece, H el número de orden del hijo (que será 1 ó 2), y S 
la suma acumulada de los hijos que ha creado y han terminado. El mensaje del proceso padre inicial será "Proceso P, 
Nivel 0, Padre, Suma S." siendo P el PID del proceso y S la suma acumulada.

*** Aquí al ver con el comando ps los procesos arrancados, se deberá ver un árbol de procesos en cadena, 
como si fueran padres, hijos, nietos, biznietos, ...

*/

// Librerias usadas. No todas
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <time.h>


// Funcion para crear cada proceso
int crear_proceso(){
    int proceso;
    
    proceso = fork();

    return proceso;     //Retprnamos el valor 
}



// Main
int main(int argc, char *argv[]){

    int niveles, tiempo, p_hijo, estado, i, z;
    char n[3],t[3];

    if(argc == 3){
        strcpy(n, argv[1]);
        niveles = atoi(n);

        strcpy(t, argv[2]);
        tiempo = atoi(t);

        if((niveles >= 1) && (niveles <= 5) && (tiempo >= 1) && (tiempo <= 10)){
            fprintf(stdout, "Los datos introducidos son:\nNiveles: %d\nTiempo: %d\n", niveles, tiempo);
        }
        else{
            fprintf(stdout, "Error en el rango de los parametros\n");
            exit -1;
        }
        // Si hemos llegado hasta aqui es que todo esta correcto

        for(i=0;i<niveles;i++){
        p_hijo = crear_proceso();
        srand(time(NULL)); //Para generar numeros aleatorios

        if(p_hijo == -1){
            fprintf(stderr, "Error al crear el proceso numero %d\n", i);
        }else{
            // Si el proceso hijo tiene un valor 0
            fprintf(stdout, "-------------------------\n");
            if(p_hijo == 0){
                // Se ejecuta:
                for(z=0; z<2;z++){
                    p_hijo = crear_proceso();
                }

                exit(1);
                
            }
            else{
                // Codigo del padre
                estado = wait(NULL); /* reaping parent */
                fprintf(stdout,"P: El hijo número %d con PID = %d ha terminado. \n", i, getpid());
        
            }
            fprintf(stdout, "-------------------------\n");
        }

    }


    }


}// END MAIN
