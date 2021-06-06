//Archivos de Cabecera de las Bibliotecas de Funciones usadas...
#include <stdio.h>     //fprintf, sprintf, ... 
#include <stdlib.h>    //exit, system, ...
#include <unistd.h>    //fork, ...
#include <sys/wait.h>  //waitpid, ...
#include <errno.h>     //errno, ...
#include <signal.h>    //sigaction, sigprocmask, ... 

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




int main (int argc, char *argv[]){

    int hijos, retardo, i, j, k, *pid_hijo, idsemaforo;

    //creamos y guardamos el id del semaforo
	idsemaforo=semget(IPC_PRIVATE,1,IPC_CREAT|0600); 
	semctl(idsemaforo,0,SETVAL,1);//Damos el valor 1 al sem


    if(argc <2){
        fprintf(stderr, "ERROR no se han introducido parametros\n");
        return -1;
    }

    hijos = atoi(argv[1]);
    retardo = atoi(argv[2]);

    if((hijos > 25) || (hijos < 5)){
        fprintf(stderr, "ERROR en el parametro de hijos. %d\n", hijos);
        return -1;
    }

    if((retardo > -1) || (retardo < -101)){
        fprintf(stderr, "ERROR en el parametro de retardo: %d\n", retardo);
        return -1;
    }
    retardo = retardo*-1+1;

    fprintf(stdout, "HIJOS %d\tRETARDO %d\n", hijos, retardo);

    // Reservamos memoria para los pids de los hijos
    pid_hijo = (int*)calloc(hijos, sizeof(int));
    for(i=0;i<hijos;i++){

    }


    free(pid_hijo);
}