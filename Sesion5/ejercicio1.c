// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

/*
Sesion 5
Ejercicio 1: Sección Crítica con Semáforos y Funciones para su manejo básico, ...



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
#include <signal.h>
#include <errno.h>

#define FIN_FALLO 1
#define FIN_EXITO 0
/* #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h> */


int waitsemaforo(int idsemaforo,int indice);
int signalsemaforo(int idsemaforo,int indice);

int main(int argc, char *argv[])
{
int i,hijo,idsemaforo,returnhijo;    

idsemaforo=semget(IPC_PRIVATE,1,IPC_CREAT|0600); //creamos un semáforo y guardamos su id.
semctl(idsemaforo,0,SETVAL,1);//damos valor 1 al semáforo.
hijo= fork();//creamos un hijo. 
    
switch (hijo)
	{
   case -1:
		fprintf(stderr, "\nERROR: no se ha creado correctamente el hijo\n");
			return 1;
            
	 case 0: //Hijo
		for(i=0;i<1000;i++)
			{
       waitsemaforo(idsemaforo,0);
       fprintf(stderr,"X");
       signalsemaforo(idsemaforo,0);
      }//for
		return 0;
            
	 default: //Padre
		for(i=0;i<1000;i++)
			{
       waitsemaforo(idsemaforo,0);
       fprintf(stderr,"O");
       signalsemaforo(idsemaforo,0);
      //para imprimir en pantalla hemos utilizado stderr para que se imprima instantáneamente.
      }//for
	}//switch case
    

if(waitpid(hijo,&returnhijo,0)==-1) fprintf(stderr,"ERROR waitpid");
        
semctl(idsemaforo, 0, IPC_RMID);//eliminamos el semáforo
    
return 0;
}//Main

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
		fprintf(stderr,"\nse ha producido un error al hacer el WAIT del semáforo\n");
		return -1;
	}//if
    
return 0;
}//waitsemaforo

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
		fprintf(stderr,"\nse ha producido un error al hacer el SIGNAL del semáforo\n");
		return -1;
	}//if
    
return 0;
}//signalsemaforo
