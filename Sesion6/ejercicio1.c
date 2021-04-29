// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

/*
Sesion 6

Ejercicio 1: Procesos y Memoria Compartida.



Programar para el S.O. Linux, un proceso padre que admitirá dos argumentos en la línea de ordenes, el primer argumento será el número de procesos hijo que debe crear
 (mínimo 1 y máximo 100), y el segundo argumento será el factor de retardo para las esperas (mínimo 0 y máximo 10). Tras validar los argumentos creará un semáforo 
 iniciado a cero, y una zona de memoria compartida de tantos bytes como el número de procesos hijo a crear y la iniciará a cero. Creará los procesos hijo uno a uno, 
 recordando su PID para usarlos en la espera final, y una vez creados todos, indicará su "arranque" señalizando el semáforo tantas unidades como el número de hijos. 
 Tras esto, el proceso padre esperará a que TODOS los hijos hayan terminado su ejecución pero en el orden inverso al de su creación (deberá esperar a que acabe el 
 hijo "N-1", luego el hijo "N-2", ..., hasta el hijo "0"), sumará los valores de los bytes de la memoria compartida y mostrará el resultado en pantalla, eliminará el 
 semáforo y la memoria compartida adecuadamente, y terminará.

Los procesos hijo, por su parte, cuando inician su ejecución esperarán a la señal de "arranque" del padre esperando por el semáforo, y tras esto realizarán una espera 
con "sleep" de tantos segundos como resulte de multiplicar el factor de retardo (segundo argumento) por un valor aleatorio entre 1 y 5 que debe generarse. El valor de 
espera resultante se guardará en la memoria compartida, en el byte correspondiente al número de orden de su creación (índice de "0" a "N-1"), y terminará su ejecución.

*/


// Librerias usadas 

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>
#include <time.h>
#include <errno.h>


//-----------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------
//(Funci'on reutilizada)
int signalsemaforo(int idsemaforo,int indice)
{
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


// FUNCION MAIN
int main (int argc ,char *argv[] ){
//Variables usadas	
int hijos,retardo,idmemoria,idsemaforo,i,hijo,returnhijo,espera,aleatorio,sumavalores=0;
int *dirmem,*pids;
time_t tiempo;
srand((unsigned) time(&tiempo));
struct timespec esperar;

//Primero realizamos el control de errores
if(argc!=3){
	// Si el numero de parametros no es correcto
	 fprintf(stderr,"ERROR al introducir los argumentos.\n");
	 return 1;
	}//if
else{
	// Si es correcto, entonces guardamos los parametros
	 hijos=atoi(argv[1]);
	 retardo=atoi(argv[2]);
	}//else

if(hijos<1||hijos>100){
	// Si los hijos estan fuera del rango
	 fprintf(stderr,"ERROR. El primer argumento tiene que estar comprendido entre 1 y 100.\n");
	 return 1;
	}

if(retardo<0||retardo>10)
	{
	// Si el retardo esta fuera del rango
	 fprintf(stderr,"ERROR. El segundo argumento tiene que estar comprendido entre 1 y 10.\n");
	 return 1;
	}

// Aqui ya se ha varificado que los datos introducidos son correctos

pids=malloc(sizeof (int)*hijos+1);//Se reserva memoria dinamica para guardar los PIDs
pids[0]=getpid();

idmemoria= shmget( IPC_PRIVATE,sizeof(int)*hijos, IPC_CREAT | 0600);//reservamos el bloque de memoria compartida

dirmem = shmat(idmemoria,NULL,0);// id memoria compartida , null , 0 

memset (dirmem,0,hijos);// pasar puntero dirección de memoria. no concuerda 

// Recorremos todos los hijos
for(i=0;i<hijos;i++)
{
fprintf(stdout,"\nPosicion %d del array compartido - valor %d \n\n",i,dirmem[i]);//comprobamos que todos los espacios del array de la memoria compartida han tomado valor 0.
}

idsemaforo = semget(IPC_PRIVATE,hijos+1,IPC_CREAT|0600);    //creamos los semadoros y guardamos su ID.
for (i=0;i<=hijos;i++)  semctl(idsemaforo,i,SETVAL,0);      //damos valor 0 a los semaforos 

// Recorremos con un FOR
for(i=1;i<=hijos;i++){
	
	hijo= fork();//creamos un hijo. 
	aleatorio=(rand()%5+1); // Damos un tiempo aleatorio 

	switch (hijo){
	    case -1:
		    fprintf(stderr, "\nERROR: no se ha creado correctamente el hijo\n");
			free (pids);//Se libera la memoria
			return 1;
            
		case 0: //Hijo

		    fprintf(stdout,"Hijo n: %d con pid (%05d) ejecutandose.\n",i,getpid());
            espera= retardo * aleatorio;// factor de retardo por numero aleatorio entre 1 y 5

			dirmem[i-1]=espera;
            waitsemaforo(idsemaforo,i);//dejamos esperando el hijo hasta que le demos el signal
 
            fprintf(stdout,"  ->Hijo n: %d con pid (%05d) terminado.\n",i,getpid());
			return 0;

	    break;
         
		default: //Padre
		pids[i]=hijo;//Se guarda el valor devuelto por el hijo en el array de pid. 
	}//switch
}//for    

sleep(1);//para que le de tiempo a crear los hijos antes de empezar a esperar.

for(i=hijos;i>0;i--){
    printf("\nEsperando a que finalice el proceso %d con PID = %d\n",i,pids[i]);
	fprintf(stdout,"  ->Va a tardar %d segundos\n",dirmem[i-1]);
	if(i != 0) 
		{	
			sumavalores=sumavalores+dirmem[i-1];		
			signalsemaforo(idsemaforo,i);
	 		sleep(dirmem[i-1]);//para que espere el tiempo que debe antes de terminar. 
		}
   else signalsemaforo(idsemaforo,0);
}

fprintf(stdout,"\nSe procede a liberar memoria y cerrar semaforos.\n");

shmctl( idmemoria, IPC_RMID, NULL);//eliminamos la memoria compartida

semctl(idsemaforo, 0, IPC_RMID);//eliminamos el semáforo

fprintf(stdout,"\nTiempo de total %d. \n",sumavalores);
return 0;
}// FIN main

