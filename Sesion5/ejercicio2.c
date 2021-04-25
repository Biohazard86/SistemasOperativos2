// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

/*
Sesion 5
Ejercicio 2: Esperar por el Turno con Semáforos, ....

Realizar el mismo programa del ejercicio anterior pero modificándolo adecuadamente para que la salida por pantalla del proceso padre y del hijo sea 
estrictamente alternada, es decir, que salga primero la letra 'O', luego la letra 'X', luego 'O', ..., con lo que deberá verse en pantalla algo como... 

OXOXOXOXOXOXOXOXOXOXOX ... OXOXOXOX





EJERCICIO 1:

Realizar una programa cuyo proceso padre cree un semáforo para sincronizar el acceso a una sección crítica, y posteriormente cree un proceso hijo. 
Tras esto, el padre y el hijo realizarán un bucle de 1000 veces en el que se dedicarán a entrar en la sección crítica, escribir en pantalla la 
letra 'O' o la 'X' según sea el padre o el hijo respectivamente, saldrán de la sección crítica y vuelta a empezar. Cuando se acabe el bucle ambos 
terminarán de forma ordenada, pero el padre antes de acabar esperará a que termine el proceso hijo creado, y eliminará el semáforo del sistema.

*** Revisad lo que se dice en el Ejercicio Práctico de la Sesión a la hora de que varios procesos escriban "simultáneamente" en pantalla.

*** Se recomienda programar una serie de funciones que encapsulen las operaciones WAIT y SIGNAL básicas, para no "ensuciar" el código principal 
con demasiadas lineas que no aportan nada de significado. Estas funciones seguro que las usaréis en más de un momento y ejercicio, y por darles 
un nombre y prototipo, os propongo que os hagáis las siguientes:
int sem_wait( int semid, int indice); //Realiza la operación WAIT(-1);
int sem_signal( int semid, int indice); //Realiza la operación SIGNAL(+1);
...y si os véis con la necesidad de ampliar con operaciones de más de UNA unidad, podéis implementar en un futuro las funciones...
int sem_operacion( int semid, int indice, short N);//Realiza la operación SIGNAL(+N), WAIT(-N) o WAIT(0);
Las funciones deberían devolver el valor 0 si todo ha ido bien, o -1 si hubo algún error interno ¡¡Siempre debe devolver un valor!! -Con lo que 
NO PUEDE ACABAR el proceso brúscamente, es decir, no se puede usar exit dentro de ella. Tampoco se debería imprimir en pantalla nada que no se pida, 
a menos que sea mientras se desarrolla para depurar.

*** Inicialmente estas funciones os parecerán redundantes, pero cuando se empiecen a mezclar los semáforos con las señales en los programas, os daréis 
cuenta que hay que "hacer más cosas" en las operaciones con semáforos, con lo que es mejor implementar algunas funciones que hagan el trabajo "sucio".
 

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
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define FIN_FALLO 1
#define FIN_EXITO 0
#define N_VECES 1000

// ---------------------------------------------------------
int waitsemaforo(int idsemaforo,int indice)
{
int control=1;
struct sembuf oper;
        
oper.sem_num= indice;
oper.sem_op= -1; // WAIT
oper.sem_flg= 0;

while ((control==1) || ((control==-1)&& (errno == EINTR)))
	{
	 control = semop(idsemaforo, &oper, 1);
	}

if (control== -1)
	{
		fprintf(stderr,"ERROR: Se ha producido un error al hacer la funcion wait en el semaforo\n");
		return -1;
	}//if
    
return 0;
}//waitsemaforo


// ---------------------------------------------------------
int signalsemaforo(int idsemaforo,int indice)
{
int control=1;
struct sembuf oper;
        
oper.sem_num= indice;
oper.sem_op= 1;// SIGNAL 
oper.sem_flg= 0;

while ((control==1)|| ((control==-1)&& (errno == EINTR)))
	{
	 control = semop(idsemaforo, &oper, 1);
	}
	
if (control== -1)
	{
		fprintf(stderr,"ERROR: Se ha producido un error al hacer la funcion signal en el semaforo\n");
		return -1;
	}//if
    
return 0;
}//signalsemaforo


// ---------------------------------------------------------

// La diferencia con el ej1, es que vamos a usar dos semaforos para gestionarlo
int main(int argc, char *argv[])
{
	int i,hijo,idsemaforo,returnhijo;    

	//creamos y guardamos el id del semaforo
	idsemaforo=semget(IPC_PRIVATE,2,IPC_CREAT|0600); 
	semctl(idsemaforo,0,SETVAL,1);//Damos el valor 1 al sem1
    semctl(idsemaforo,1,SETVAL,0);//Damos el valor 0 al sem2
	hijo = fork();	// Creamos el hijo
		
	switch (hijo){

	case -1:
		fprintf(stderr, "ERROR: No se ha podido crear el hijo\n");
			return 1;	// Si no se ha podido crear el hijo, salimos
				
	case 0: //Codigo del hijo
		// Realizamos el bucle N_VECES
		for(i=0;i<N_VECES;i++){
			// Esperamos al semaforo
			waitsemaforo(idsemaforo,0);
			// Imprimimos una X (Por ser el hijo)
			fprintf(stderr,"X");
			// Enviamos senial al semaforo
			signalsemaforo(idsemaforo,1);
		}//for
		return 0;
				
	default: //Codigo del padre
		// Realizamos el bucle N_VECES
		for(i=0;i<N_VECES;i++){
			// Esperamos al semaforo
			waitsemaforo(idsemaforo,1);
			// Imprimios una O (Por ser el padre)
			fprintf(stderr,"O");
			// Enviamos senial al semaforo
			signalsemaforo(idsemaforo,0);
			//para imprimir en pantalla hemos utilizado stderr para que se imprima instantáneamente.
		}//for
	}//switch
		

	if(waitpid(hijo,&returnhijo,0)==-1){
		fprintf(stderr,"ERROR waitpid");
	} 
			
	semctl(idsemaforo, 0, IPC_RMID);//Eliminamos el sem
    semctl(idsemaforo, 1, IPC_RMID);//Eliminamos el sem
		
	return 0;
}//Main
