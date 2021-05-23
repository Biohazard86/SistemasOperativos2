// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

// FUNCIONA, LISTO PARA ENTREGAR


/*
Ejercicio 4: Creación, Espera y Retorno de Procesos con Ejecución externarno
Realizar dos programas por separado los cuales harán tareas de monitorización y de ejecución respectivamente.
Monitor: El primer programa (monitor.c) deberá admitir dos argumentos, ambos opcionales. El primero será el número (P) de procesos hijo que deberá crear. 
Será un número que deberá estar en el rango 1 a 50, el cual si no viene se asumirá 1 por defecto, pero si viene y no es válido se indicará el error por 
pantalla y terminará con error. El segundo argumento será el número de segundos (S) que el otro programa deberá ejecutarse antes de terminar. Será un 
número en el rango 1 a 150 y si no viene se asumirá 10 por defecto, pero si viene y no es válido se indicará el error por pantalla y terminará con error.
- El monitor tras validar los argumentos creará todos los procesos hijos que deba según el argumento (P) obtenido, y cuando los tenga creados, se quedará 
esperando a que todos ellos acaben, independientemente del orden de creación. Cuando detecte la terminación de un proceso, informará por pantalla del PID 
del mismo y del resultado que ha devuelto. A su vez, cada proceso hijo creado calculará un número aleatorio (A) entre 1 y 15, mostrará un mensaje en 
pantalla indicando su PID y el valor aleatorio calculado, y lanzará la ejecución del programa externo (ejecutor), enviándole dos argumentos, siendo el 
primero el número de segundos (S), y el segundo el número aleatorio (A) calculado. Si la ejecución del programa externo no es correcta, el proceso hijo 
terminará con el código de error igual a 255, y si es correcta se habrá convertido en el programa externo y terminará cuando éste acabe con su propio retorno. 
El array de argumentos que se pasará al programa externo serán dos cadenas y no llevará en el índice 0 el nombre del programa, sino que en este caso se 
usará el índice 0 para los segundos (S) y el 1 para el aleatorio (A).
Ejecutor: El segundo programa (ejecutor.c) detectará los dos posibles argumentos que le pueden llegar del programa monitor, teniendo en cuenta que en el 
índice 0 no deberá llegar el nombre del programa, sino el número de segundos (S) y en el argumento de índice 1 el número aleatorio. Por tanto si los dos 
argumentos no son dos números, el programa mostrará un mensaje de error y terminará con el código de error 254. El mensaje puede ser algo como... "Intento 
de ejecución sin usar el monitor. No se puede continuar."
- El ejecutor tras validar los argumentos calculará el tiempo (T) que va a dormir, sumando ambos valores (T=S+A), mostrará un mensaje en pantalla indicando 
su PID y el tiempo T que va a "trabajar" y hará una pausa sin consumir CPU durante ese tiempo (T) medido en segundos, y cuando termine, deberá acabar 
devolviendo al sistema ese valor (T) como código de retorno. Para simular el trabajo del proceso, se debe dormir el proceso utilizando la función "sleep()" del sistema.
*** El programa ejecutor como puedes observar no hace nada útil, pero la combinación de ambos programas sirve de base para implementar cualquier trabajo de 
cálculo o de larga ejecución, y/o que se quiera ejecutar de forma controlada/protegida y no de forma manual, y/o que se necesite tener vigilado por otro programa 
para detectar su terminación y luego hacer otras cosas.
. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int delay, n_random, tiempo_dormir;
    
    //fprintf(stdout, "SOY EJECUTOR!\n");

    if(!atoi(argv[0])){
        fprintf(stdout, "Intento de ejecución sin usar el monitor. No se puede continuar.\n");
        return 254;
    }
    
    delay=atoi(argv[0]);
    n_random=atoi(argv[1]);
    //fprintf(stdout, "Se ha llamado desde monitor. DELAY %d / RANDOM %d\n", delay, n_random);
    tiempo_dormir = delay + n_random;
    fprintf(stdout,"\tEJECUTOR: Voy a trabajar %d segundos\n", tiempo_dormir);
    sleep(tiempo_dormir);
    return tiempo_dormir;

    //sleep(10);
}