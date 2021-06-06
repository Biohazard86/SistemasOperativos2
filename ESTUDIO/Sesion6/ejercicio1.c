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
#include <sys/shm.h>
#include <time.h>

struct memoria_compartida
{
    int dato;
};



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

    int n_hijos, retardo, idsemaforo, idmemoria, i, j, k, pid_padre=getpid(), *pid_hijo, inicio_memoria, random, espera;
    struct memoria_compartida * memoria;

    n_hijos = atoi(argv[1]);
    retardo = atoi(argv[2]);

    srand(time(NULL));

    if((n_hijos < 1) || (n_hijos > 100)){
        return -1;
    }
    if((retardo < 0) || (retardo > 10)){
        return -1;
    }

    // Reservamos memoria para todos los hijos
    pid_hijo = (int*)calloc(n_hijos, sizeof(int));

    // Creamos un semaforo iniciado a cero
    idsemaforo=semget(IPC_PRIVATE,1,IPC_CREAT|0600); 
	semctl(idsemaforo,0,SETVAL,0);//Damos el valor 0 al sem

    // Creamos la memoria compartida
    idmemoria = shmget( IPC_PRIVATE, sizeof( * memoria) , IPC_CREAT | 0600);
    //Enganchamos con la memoria compartida
    memoria = (struct memoria_compartida*)shmat( idmemoria, NULL, 0);


    for(i=0; i<1+n_hijos;i++){
        if(getpid() == pid_padre){
            pid_hijo[i] = fork();
        }
        switch (pid_hijo[i])
        {
        case -1:
            fprintf(stderr, "ERROR al crear el hijo numero %d\n", i);
            break;

        case 0:
            // Codigo del hijo
            
             
            random = rand() % (5 + 1 - 1) + 1, espera;
            
            waitsemaforo(idsemaforo,0);

            espera = random * retardo;

            fprintf(stdout, "Soy el hijo %d y voy a hacer una espera de %d\n", getpid(), espera);
            sleep(espera);

            // Escribimos en la memoria compartida el tiempo de espera

            memoria->dato=espera+memoria->dato;



            signalsemaforo(idsemaforo,1);
            fprintf(stdout, "Dejo libre el semaforo\n");
            return 0;

        default:
            sleep(1);

            semctl(idsemaforo,0,SETVAL,n_hijos);//Damos el valor n_hijos al sem

            sleep(10);
            fprintf(stdout, "El total almacenado el la memoria compartida es %d\n", memoria->dato);
            break;
        }
            
        
    }



    //Desenganchamos la memoria
    //shmdt(inicio_memoria);
    free(pid_hijo);
}