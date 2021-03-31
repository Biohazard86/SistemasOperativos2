// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

// FUNCIONA, LISTO PARA ENTREGAR


/*
Ejercicio 2: Creación de Procesos, Espera y Valores de Retorno

Realizar un programa que al ejecutarse cree 7 procesos hijo, y que cuando los tenga creados espere por su terminación de forma adecuada, obteniendo su valor de retorno, sumándolo, y mostrando en pantalla dicho valor y la suma acumulada hasta el momento. A su vez, cada proceso hijo creado mostrará por pantalla el orden de creación (de 1 a 7), su identificador de proceso, y un valor calculado al que se denominará "espera", el cual será el orden de creación más un valor aleatorio entre 1 y 5 segundos; Luego dormirá (con sleep) tantos segundos como el valor "espera" calculado, y cuando despierte terminará devolviendo al sistema el valor del orden de creación multiplicado por 10 más el valor aleatorio calculado. 
*/

// Librerias usadas. No todas
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#define NUM_PROCESOS_HIJO 7




// Main
int main(int argc, char *argv[]){

    int procesos_hijo_pid[NUM_PROCESOS_HIJO], i, n_random;
    
    
     for(i=0;i<NUM_PROCESOS_HIJO;i++){ 
        
        procesos_hijo_pid[i] = fork();
        if(procesos_hijo_pid[i] == -1){
            fprintf(stderr, "Error al crear el proceso numero %d\n", i);
        }else{
            
            fprintf(stdout, "P: Nuevo proceso creado. PID -> %d\n", procesos_hijo_pid[i]);
            
            
            // Si el proceso hijo tiene un valor 0
            if(procesos_hijo_pid[i] == 0){
                // Se ejecuta:
                fprintf(stdout, "\tH: Soy el hijo con ID= %d\n", getpid() );
                fprintf(stdout, "\tH: Y mi padre es ID= %d\n", getppid() );
                fprintf(stdout, "\tH: Mi posicion es la %d\n", i);
                n_random = rand () % (5-1+1) + 1;   
                fprintf(stdout, "\tH: Espera %d\n", i+n_random);
                sleep(i+n_random);
                return(i+n_random);
                //break;
            }
            else{
                // Codigo del padre
            }
        }
        
    } 

         
}// END MAIN
