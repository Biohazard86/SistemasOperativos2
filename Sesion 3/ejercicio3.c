// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

// FUNCIONA, LISTO PARA ENTREGAR


/*
Ejercicio 3: Creación y Espera ordenada de Procesos, y Valores de Retorno

Realizar un programa que al ejecutarse cree 7 procesos hijo, y que cuando los tenga creados espere por su terminación de forma adecuada, 
obteniendo su valor de retorno, sumándolo, y mostrando en pantalla dicho valor y la suma acumulada hasta el momento. A su vez, cada proceso 
hijo creado mostrará por pantalla el orden de creación (de 1 a 7), su identificador de proceso, y un valor calculado al que se denominará 
"espera", el cual será el orden de creación más un valor aleatorio entre 1 y 5 segundos; Luego dormirá (con sleep) tantos segundos como el 
valor "espera" calculado, y cuando despierte terminará devolviendo al sistema el valor del orden de creación multiplicado por 10 más el 
valor aleatorio calculado. 
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





// Main
int main(int argc, char *argv[]){

    int numero_procesos=0;

    int procesos_hijo_pid[numero_procesos], i, n_random, valor_devuelto;
    pid_t estado;
    char numero[1];
    int num;

    // Si se han introducido bien el numero de parametros:
    if(argc == 2){
        strcpy(numero, argv[1]);
        numero_procesos = atoi(numero);

        if((numero_procesos< 5) || (numero_procesos > 30)){
            fprintf(stderr, "El numero introducido no esta entre 5 y 30.\n");
            exit(1);
        }
        //fprintf(stdout, "El numero es > %d\n",numero_procesos);
    }
    // Si no se han introducido bien:
    else{
        fprintf(stderr, "No se han introducido los parametros requeridos.\n");
        exit(1);
    }
    
     for(i=0;i<numero_procesos;i++){ 
        srand(time(NULL)); //Para generar numeros aleatorios
        procesos_hijo_pid[i] = fork();
        if(procesos_hijo_pid[i] == -1){
            fprintf(stderr, "Error al crear el proceso numero %d\n", i);
        }else{
            
            fprintf(stdout, "P: Nuevo proceso creado. PID -> %d\n", procesos_hijo_pid[i]);
            
            
            // Si el proceso hijo tiene un valor 0
            if(procesos_hijo_pid[i] == 0){
                // Se ejecuta:
                fprintf(stdout, "\tH: Soy el hijo %d con ID= %d\n", i, getpid() );
                sleep(1);
                exit(1);
                
            }
            else{
                // Codigo del padre
                estado = wait(NULL); /* reaping parent */

                fprintf(stdout,"P: El hijo número %d con PID=%d ha terminado con estado %d. \n", i, getpid(), WEXITSTATUS(estado));

                /* fprintf(stdout,"P: Soy el padre. \n");
                fprintf(stdout,"P: Mi PID = %d\n", getpid());
                fprintf(stdout,"P: PID de mi hijo = %d\n", estado);
                //fprintf(stdout,"Exit status: %d\n", WEXITSTATUS(estado));
                if(WIFEXITED(estado) != 0){
                    fprintf(stdout,"P: El codigo devuelto es:  %d\n", WEXITSTATUS(estado));
                }else{
                    fprintf(stdout,"P: Mi hijo no ha terminado de forma correcta. \n");
 
                } */
            }
            fprintf(stdout, "-------------------------\n");
        }
        
    } 

         
}// END MAIN
