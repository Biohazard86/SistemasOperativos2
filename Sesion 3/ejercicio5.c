// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

// FUNCIONA (MAS O MENOS)


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

#define FIN_FALLO 1
#define FIN_EXITO 0

// Función que comprueba la validez de los parametros
int parametros(int *niveles, int *tiempo, int argc, char * argv[])
{
    if(argc<3)
    {
        fprintf(stderr, "ERROR: Numero de parametros incorrecto\n");
        return -1;
    }

    *niveles = atoi(argv[1]);
    *tiempo = atoi(argv[2]);

    if((*niveles < 1) || (*niveles > 5))
    {
        fprintf(stderr, "ERROR: el parametro numero de procesos : %d \n", *niveles );
        return -1;
    }
    if((*tiempo < 1) || (*tiempo > 10))
    {
        fprintf(stderr, "ERROR: el parametro tiempo : %d \n", *tiempo );
        return -1;
    }
    return 0;
}

// función que crea dos procesos
int crea_procesos(int procesos_restantes, int *hijos, int nivel_actual){
    srand(time(NULL)); //Para generar numeros aleatorios
    int i=0, tiempo=0, ale=0;
    
    // Creamos los procesos hijo
    hijos[nivel_actual]=fork();
   
    if(hijos[nivel_actual]==0)
    {
        // Codigo del hijo

        srand(getpid());
        ale=rand () % (150-1+1) + 1;;
        printf("Soy el hijo número %d con PID: %d. Hijo de %d.\n",nivel_actual,getpid(), getppid());
        // dormimos el proceso en función de la posición que tenga
        sleep(nivel_actual);
        
        
        if(procesos_restantes == nivel_actual){
            
            fprintf(stdout, "--------------------------------------\nSe ha llegado al final de la cascada\n--------------------------------------\n");
            //return 1;
        }else{
            crea_procesos(procesos_restantes, hijos, nivel_actual+1);
        }
        // Retornamos el valor aleatorio
        return ale;
        
    }
    else if(hijos[nivel_actual] ==-1)
    {

        fprintf(stderr, "ERROR: No se han creado los hijos\n");
    }
    else
    {
        //el padre
    }
    
}

// MAIN
int main(int argc, char * argv[])
{
    int niveles, tiempo, *hijos,i, status, ale, nivel_actual;

    // Comprobamos que los parametros sean correctos
    if(parametros(&niveles,&tiempo, argc, argv)==-1)
    {
        return FIN_FALLO;
    }

    // Reservamos memoria para los PIDs de los hijos
    hijos = (int*)malloc(niveles);

    //crear los hijos
    crea_procesos(niveles, hijos,0);

    sleep(niveles+2);
    
    
    for(i=0; i<=niveles;i++)
    {
        printf(" Esperando que acabe el hijo número %d con PID=%d.\n", i, hijos[i] );

        waitpid(hijos[i+1],&status,0);

        if(hijos[i] != 0){
            if(WIFEXITED(status)==0)
                printf("\tHijo con PID=%d ha terminado de forma normal. Valor devuelto: %d.\n",hijos[i-1], WEXITSTATUS(status));
            if(WIFSTOPPED(status)==0)
                printf("\tHijo con PID=%d se ha detenido. Valor devuelto %d.\n",hijos[i-1], WEXITSTATUS(status));
        }

    }
    
    return FIN_EXITO;

}
//FIN MAIN
