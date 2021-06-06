//---------------------------------------------------------------------------
//Ejercicio Practico Sesion 5: 
//  Exclusion Mutua con Semaforos, Uso de Señales y Pantalla.
//---------------------------------------------------------------------------

//Archivos de Cabecera de las Bibliotecas de Funciones usadas...
#include <stdio.h>     //fprintf, sprintf, ... 
#include <stdlib.h>    //exit, system, ...
#include <unistd.h>    //fork, ...
#include <sys/wait.h>  //waitpid, ...
#include <errno.h>     //errno, ...
#include <signal.h>    //sigaction, sigprocmask, ... 
#include <time.h>
#include <sys/types.h> //tipos varios, ...
#include <sys/ipc.h>   //macros IPC, ...
#include <sys/sem.h>   //semget, semctl, semop, ...

#define FIN_FALLO 1
#define FIN_EXITO 0
#define N_VECES 1000

// Variable global
int seguir = 1;

void funcion_manejo(int senial){
    seguir = 0;
}

//Funciones de manejo de semaforos
int sem_op( int idSem, int indice, int operacion)
{
  int res= 1;
  struct sembuf op;
  
  op.sem_num= indice; 
  op.sem_op = operacion;
  op.sem_flg= 0; 
  
  //Efectuar la operacion si no se haya ejecutado por primera vez, o
  //si hay error, sea error de interrupcion..
  while ((res == 1) || ((res == -1) && (errno == EINTR))) {
    res= semop( idSem, &op, 1);
  }//while
  if (res == -1) perror( "Error Operación Semáforo.");
  
  return (res);
}//sem_op

int waitsemaforo(int idsemaforo,int indice){
// VARS
int control=1;
struct sembuf oper;
        
oper.sem_num = indice;
oper.sem_op = -1; // WAIT
oper.sem_flg = 0;

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


int nsleep(long miliseconds)
{
   struct timespec req, rem;

   if(miliseconds > 999)
   {   
        req.tv_sec = (int)(miliseconds / 1000);                            /* Must be Non-Negative */
        req.tv_nsec = (miliseconds - ((long)req.tv_sec * 1000)) * 1000000; /* Must be in range of 0 to 999999999 */
   }   
   else
   {   
        req.tv_sec = 0;                         /* Must be Non-Negative */
        req.tv_nsec = miliseconds * 1000000;    /* Must be in range of 0 to 999999999 */
   }   

   return nanosleep(&req , &rem);
}


int main(int argc, char *argv[])
{
	int i,hijo,idsemaforo,returnhijo;    

	//creamos y guardamos el id del semaforo
	idsemaforo=semget(IPC_PRIVATE,2,IPC_CREAT|0600); 
	semctl(idsemaforo,0,SETVAL,0);//Damos el valor 0 al sem
    semctl(idsemaforo,1,SETVAL,1);//Damos el valor 0 al sem
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
			fprintf(stderr,"H%d\n", 1); 
			nsleep(2500);
			// Enviamos senial al semaforo
			signalsemaforo(idsemaforo,1);

		}//for
		return 0;
				
	default: //Codigo del padre
		// Realizamos el bucle N_VECES
        /signalsemaforo(idsemaforo,1);
		for(i=0;i<N_VECES;i++){
			// Esperamos al semaforo
			waitsemaforo(idsemaforo,1);
			// Imprimios una O (Por ser el padre)
			fprintf(stderr,"P%d\n", 0); 
			nsleep(2500);
			// Enviamos senial al semaforo
			signalsemaforo(idsemaforo,0);
			//para imprimir en pantalla hemos utilizado stderr para que se imprima instantáneamente.
		}//for
	}//switch
		

	if(waitpid(hijo,&returnhijo,0)==-1){
		fprintf(stderr,"ERROR waitpid");
	} 
			
	semctl(idsemaforo, 0, IPC_RMID);//eliminamos el semáforo
		
	return 0;
}//Main
