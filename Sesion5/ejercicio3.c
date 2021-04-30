// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

/*
Sesion 5
Ejercicio 3-4: Sincronización de Turno entre N procesos con Semáforos,...Captura de Señales, y Terminación de forma ordenada.



Este ejercicio es la versión con Semáforos del último ejercicio de la Sesión de Señales.

Realizar un programa que admitirá dos argumentos opcionales, que serán valores numéricos a los que denominaremos HIJOS y RETARDO. 
El argumento HIJOS deberá ser un valor comprendido entre 5 y 25, el cual si no viene será 5 por defecto y si es erróneo se indicará 
el error terminando el programa. El argumento RETARDO deberá ser un valor comprendido entre -1 y -101, el cual si no viene será -1 
por defecto y si es erróneo se indicará como error terminando el programa. Como véis se puede distinguir quién es cada uno por el 
rango de valores, con lo que no debe importar el orden de aparición en los argumentos. Al valor de RETARDO tras haberlo validado, 
le sumáis 1 y le quitáis el signo para que quede un número entre 0 y 100.

Con los argumentos obtenidos y validados, el proceso principal (futuro padre) configurará la señal que se recibe al pulsar CTRL+C 
en el terminal (SIGINT) y la de terminación (SIGTERM) para que, al recibir cualquiera de ellas, sepa que debe terminarse. También 
debe crear un conjunto de semáforos para sincronizar adecuadamente la obtención del "testigo" o "Turno" de ejecución entre todos 
los procesos que participarán. Tras esto, creará tantos procesos hijo como el valor de HIJOS, que deberá recordar para luego 
enviarles la señal de terminación (SIGTERM) al final de la ejecución, y también para esperar por su terminación al final. Cada 
proceso hijo creado, como heredan la configuración de las señales del padre, deberán reconfigurarse para ignorar la señal de CTRL+C, 
pero seguirán manteniendo la de terminación, pues servirá para lo mismo (más o menos) que en el padre.

Tras la creación y preparación, todos los procesos entran en un ciclo de trabajo del que sólo saldrán cuando sepan que deben terminar 
(habrán recibido alguna señal que se lo indique).

Dentro del ciclo todos los procesos deberán esperar (sin consumir CPU prácticamente) a tener el turno que les permita seguir adelante, 
es decir, entrarán en la sección crítica.

Cuando eso ocurra, imprimirán por la salida estándar un mensaje con el siguiente formato de printf: "%c%d(%05ld) %s.\n", siendo el 
primer argumento del formato (%c) un carácter que será la letra 'P' si es el proceso padre, o 'H' si es un proceso hijo, el segundo 
argumento (%d) será el número de orden del proceso, con el 0 para el padre, y de 1 a HIJOS para los hijos, el tercer argumento (%05ld) 
debe ser el PID del proceso formateado a 5 dígitos con espacios por la izquierda, y el cuarto argumento (%s) un texto con el contenido 
"Recibido Testigo".

Después de imprimir el mensaje, harán una pausa de tantas décimas de segundo como indique el RETARDO enviado, el cual si es 0 no 
deberá hacer nada (aquí no va a servir el usar sleep o la señal de alarma por la resolución de segundos que permiten, pero la 
función usleep, o mejor la función nanosleep puede que sí).

Al final de la pausa, enviarán el testigo del "turno", avisando al proceso que toque que salen de la sección crítica, que será 
siempre el siguiente proceso en orden de creación, es decir, tanto para el proceso padre como para los procesos hijo será el siguiente 
hijo creado, a excepción del último hijo que, por ser el último, le enviará el testigo a su padre.

Si todo va bien, durante la ejecución cíclica de los procesos "obteniendo los turnos", se deberá ver en pantalla los mensajes que 
generan de forma "ordenada".

Cuando un proceso deba terminar, saldrá del ciclo ordenadamente, es decir, que lo acabará completamente y al intentar repetirlo no 
entrará de nuevo. En cuanto el proceso padre reciba la señal de CTRL+C o la de terminación (SIGTERM) sabe que debe terminar, los 
procesos hijo saben que deben acabar al recibir SÓLO la señal de terminación.

En el caso de un proceso hijo, tras acabar el bucle, mostrará en la salida estándar un mensaje con el formato indicado anteriormente, 
pero el argumento de mensaje (%s) será el texto "Finalizado".

En el caso del proceso padre, después del bucle, mostrará un mensaje con el formato indicado pero con el texto "Avisando Hijos", 
enviará a todos sus hijos la señal de terminación (SIGTERM) para que sepan que deben ir acabando, mostrará otro mensaje (mismo formato) 
con el texto "Esperando Hijos", esperará a que todos sus hijos hayan terminado, eliminará el conjunto de semáforos y por último mostrará 
un mensaje con el texto "Programa Terminado".
*/

// -------------------------------------------------------------------------------------------------------------

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
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
// -------------------------------------------------------------------------------------------------------------

// Variable global para ver si se sigue o no
int seguir=1;

// -------------------------------------------------------------------------------------------------------------
// Funcion para esperar el semaforo (Funciones reutilizadas)
int waitsemaforo(int idsemaforo,int indice){
int control=1;	// Variable de control
struct sembuf oper;    
oper.sem_num= indice;
oper.sem_op= -1; // WAIT
oper.sem_flg= 0;

while ((control==1) || ((control==-1)&& (errno == EINTR))){
	control = semop(idsemaforo, &oper, 1);
}
// Si control es igual a 1, eso quiere decir que se ha producido un arror, mostramos el mensaje por el canal de errores
if (control== -1){
	fprintf(stderr,"ERROR: Se ha producido un error al hacer la funcion wait en el semaforo\n");
	return -1;
}//if
    
return 0;
}//FIN waitsemaforo

// -------------------------------------------------------------------------------------------------------------

//(Funci'on reutilizada)
int signalsemaforo(int idsemaforo,int indice){
int control=1;		// variable de control
struct sembuf oper;
        
oper.sem_num= indice;
oper.sem_op= 1;// SIGNAL 
oper.sem_flg= 0;

while ((control==1)|| ((control==-1)&& (errno == EINTR))){
	control = semop(idsemaforo, &oper, 1);
}
	
if (control== -1){
	fprintf(stderr,"ERROR: Se ha producido un error al hacer la funcion signal en el semaforo\n");
	return -1;
}//if
    
return 0;
}//FIN signalsemaforo

void handler(int signum)
{
	if(signum==SIGINT || signum==SIGTERM) seguir=0;    
}//handler

// -------------------------------------------------------------------------------------------------------------

// Funcion que alterna los turnos
void turno(int retardo,char letra,int indice,int idsemaforo,int hijos)
{
    struct timespec esperar;

    esperar.tv_sec=0;
    esperar.tv_nsec=(retardo)*10000000;

	 while(seguir)
		{
		 waitsemaforo(idsemaforo,indice);
  	 	 fprintf(stdout,"%c%d(%05d) %s.\n",letra,indice,getpid(),"Recibido el testigo");
  	 	 nanosleep(&esperar,NULL);
  	 	 if(indice != hijos) signalsemaforo(idsemaforo,indice+1);
  	 	 else signalsemaforo(idsemaforo,0);
  	}//while

}//turno

// -------------------------------------------------------------------------------------------------------------

// Funcion MAIN
int main(int argc, char *argv[]){
// VARIABLES
int i,*hijo,idsemaforo,returnhijo,hijos,retardo;    
struct sigaction accion;
accion.sa_handler=handler;
sigaction(SIGINT,&accion,NULL);
sigaction(SIGTERM,&accion,NULL);

if(argc>3){
	fprintf(stderr,"El programa solo admite dos argumentos.\n SALIENDO\n");
	return -1;
}//if
else{
    if(argc==3){
            // En el caso de que vaya bien se guardan
            hijos=atoi(argv[1]);
            retardo=atoi(argv[2]);
        }//if
    else{
        if(argc==2){
            hijos=atoi(argv[1]);
            retardo=-1;
        }//if
        else{
            if(argc==1){
                hijos=5;
                retardo=-1;
            }//if
        }//else
    }//else
}//else
	
if(hijos<5||hijos>25){
    fprintf(stderr,"El primer argumento debe estar entre 5 y 25.\n");
    return -1;
}
if(retardo<-101||retardo>-1){
    fprintf(stderr,"El segundo debe estar entre -1 y -101.\n");
    return -1;
}
else{
    retardo= (retardo + 1)*-1;
}	

idsemaforo=semget(IPC_PRIVATE,hijos+1,IPC_CREAT|0600); //creamos el conjunto de sem y guardamos su id.
for (i=0;i<=hijos;i++)  semctl(idsemaforo,i,SETVAL,0);//damos valor 0 a los semaforos 

// Reservamos memoria para todos los hijos
hijo=malloc(sizeof(int)*(hijos+1));
// Obtenemos el pid del hijo 0
hijo[0]=getpid();

for(i=1;i<=hijos;i++){
	hijo[i]= fork();//creamos un hijo. 
    // Elegimos que es lo que pasa con el hijo i
	switch (hijo[i]){
	   case -1:
            // En caso de que el resultado sea -1
			fprintf(stderr, "\nERROR:No se ha podido crear el hijo\n");
			return 1;
            
		 case 0: //Codigo del hijo
 				signal(SIGINT, SIG_IGN);// ignorar la señal del sigint
				turno(retardo,'H',i,idsemaforo,hijos);//llamo a la func turno que controla los sem
                fprintf(stdout,"%c%d(%05d) %s.\n",'H',i,getpid(),"Finalizado");
				exit(0);
			return 0;
	            
		 default: //Padre
		 break;
	}//switch case
}//for    

signalsemaforo(idsemaforo,1);//pongo a 1 el sem del primer hijo para empezar.
turno(retardo,'P',0,idsemaforo,hijos);// llamo a la funcion turno que controla los sem

fprintf(stdout,"%c%d(%05d) %s.\n",'P',0,getpid()," -> Avisando a los hijos");

fprintf(stdout,"%c%d(%05d) %s.\n",'P',0,getpid()," -> Esperando a los hijos");
    
for(i=1;i<=hijos;i++){ 
    // Matamos a los hjijos
    kill(hijo[i],SIGTERM);
}
    

    
for(i=1;i<=hijos;i++)
	{ //bucle para esperar por los procesos hijos, si hay un error se indica
   if(waitpid(hijo[i],&returnhijo,0)==-1)
		{
	   fprintf(stderr,"ERROR al intentar cerrar los hijos");
    }   
  }
    
    
fprintf(stdout,"%c%d(%05d) %s.\n",'P',0,getpid(),"--- Programa Terminado ---");	
  
semctl(idsemaforo, 0, IPC_RMID);//eliminamos el sem
      
return 0;
}//Main
