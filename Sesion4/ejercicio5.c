// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

/*
Sesion 4
Ejercicio 5: ...Terminación de forma ordenada y "Sincronización" entre procesos.no



Realizar un programa que admitirá dos argumentos opcionales, que serán valores numéricos a los que denominaremos HIJOS y RETARDO. 
El argumento HIJOS deberá ser un valor comprendido entre 5 y 25, el cual si no viene será 5 por defecto y si es erróneo se 
indicará el error terminando el programa. El argumento RETARDO deberá ser un valor comprendido entre -1 y -101, el cual si no 
viene será -1 por defecto y si es erróneo se indicará como error terminando el programa. Como véis se puede distinguir quién es 
cada uno por el rango de valores, con lo que no debe importar el orden de aparición en los argumentos. Al valor de RETARDO tras 
haberlo validado, le sumáis 1 y le quitáis el signo para que quede un número entre 0 y 100.

Con los argumentos obtenidos y validados, el proceso principal (futuro padre) configurará la señal que se recibe al pulsar CTRL+C 
en el terminal (SIGINT) y la de terminación (SIGTERM) para que, al recibir cualquiera de ellas, sepa que debe terminarse. También 
debe configurar, la señal de "Usuario 1" (SIGUSR1) para recibir avisos de otros procesos (hijos que va a tener) como si fuera un 
"testigo" o indicador de "turno". Tras esto, creará tantos procesos hijo como el valor de HIJOS, que deberá recordar para luego 
enviarles la señal de aviso (SIGUSR1) de que tienen el "turno", o la de terminación al final. Cada proceso hijo creado, como heredan 
la configuración de las señales del padre, deberán reconfigurarse para ignorar la señal de CTRL+C, pero seguirán manteniendo la de 
terminación y la de "Usuario 1", pues servirán para lo mismo (más o menos) que en el padre.

Tras la creación y preparación, todos los procesos entran en un ciclo de trabajo del que sólo saldrán cuando sepan que deben 
terminar (habrán recibido alguna señal que se lo indique). En el caso del proceso padre, antes de entrar en el ciclo, enviará 
el testigo del "turno", usando la señal de "Usuario 1" (SIGUSR1) al último proceso hijo creado.

Dentro del ciclo todos los procesos deberán esperar (sin consumir CPU prácticamente) a tener el turno que les permita seguir 
adelante. Cuando eso ocurra, imprimirán por la salida estándar un mensaje con el siguiente formato de printf: "%c%d(%05ld) %s.\n", 
siendo el primer argumento del formato (%c) un carácter que será la letra 'P' si es el proceso padre, o 'H' si es un proceso hijo, 
el segundo argumento (%d) será el número de orden del proceso, con el 0 para el padre, y de 1 a HIJOS para los hijos, el tercer 
argumento (%05ld) debe ser el PID del proceso formateado a 5 dígitos con espacios por la izquierda, y el cuarto argumento (%s) un 
texto con el contenido "Recibido Testigo". Después de imprimir el mensaje, harán una pausa de tantas décimas de segundo como 
indique el RETARDO enviado, el cual si es 0 no deberá hacer nada (aquí no va a servir el usar sleep o la señal de alarma por la 
resolución de segundos que permiten, pero la función usleep -obsoleta-, o mejor la función nanosleep puede que sí). Al final de 
la pausa, enviarán el testigo del "turno" (con la señal de "Usuario 1") al proceso que toque, que será siempre al último PID de 
proceso que conozcan, es decir, tanto para el proceso padre como para los procesos hijo será el PID del último hijo creado, a 
excepción del primer hijo que, por ser el primero, le enviará el testigo a su padre.

Si todo va bien, durante la ejecución cíclica de los procesos "obteniendo los turnos", se deberá ver en pantalla los mensajes que 
generan de forma "ordenada".

Cuando un proceso deba terminar, saldrá del ciclo ordenadamente, es decir, que lo acabará completamente y al intentar repetirlo 
no entrará de nuevo. En cuanto el proceso padre reciba la señal de CTRL+C o la de terminación (SIGTERM) sabe que debe terminar, 
los procesos hijo saben que deben acabar al recibir SÓLO la señal de terminación.

En el caso de un proceso hijo, tras acabar el bucle, mostrará en la salida estándar un mensaje con el formato indicado anteriormente, 
pero el argumento de mensaje (%s) será el texto "Finalizado".

En el caso del proceso padre, después del bucle, mostrará un mensaje con el formato indicado pero con el texto "Avisando Hijos", 
enviará a todos sus hijos la señal de terminación (SIGTERM) para que sepan que deben ir acabando, mostrará otro mensaje (mismo formato) 
con el texto "Esperando Hijos", esperará a que todos sus hijos hayan terminado y por último mostrará un mensaje con el texto "Programa 
Terminado".

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
#include <unistd.h>
#include <signal.h>

#define FIN_FALLO 1
#define FIN_EXITO 0


int main(int argc, char * argv[]){

    int n_hijos, retardo, p1, p2, i;

    int *hijos_pid;



    // Realizamos la comprobacion de que nos llegan los parametros necesarios.
    if (argc != 3){
        // Si el numero de parametros no es 3, entonces salimos del programa ya que necesitamos que sea el nombre del programa y dos argumentos
        fprintf(stderr,"ERROR en la introduccion de parametros\n");
        return FIN_FALLO;
    }
    // Convertimos los parametros de char a int
    p1 = atoi(argv[1]);
    p2 = atoi(argv[2]);

    // Guardamos el valor por defecto por si no entra en ningun condicional
    n_hijos = 5;

    //Comprobamos si el parametro 1 se corresponde con el de los hijos
    if((p1 >5) && (p1 < 25)){
        n_hijos = p1;
    }
    // Comprobamos si el segundo parametro se corresponde con el de los hijos
    if((p2>5) && (p2 < 25)){
        n_hijos = p2;
    }

    // Si no recibimos el retardo el p1
    if((p1 > (-101)) && (p1 < (-1))){
        retardo = p1;
    }
    else{
        // Ni en P2
        if((p2 > (-101)) && (p2 < (-1))){
            retardo = p2;
        }
        else{
            // Finalizamos el programa, ya que es un dato necesario.
            fprintf(stderr,"ERROR en la introduccion de parametros\n");
            return FIN_FALLO;
        }
    }


    
    fprintf(stdout,"HIJOS = %d - RETARDO = %d\n", hijos, retardo);

    // Resetvamos memoria para los hijos
    hijos_pid=(int*)malloc(n_hijos);
    // Vamos a crear todos los hijos.
    for(i=0;i<n_hijos;i++){

    }


}