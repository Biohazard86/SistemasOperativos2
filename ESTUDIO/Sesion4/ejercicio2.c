// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

/*
Sesion 4
Ejercicio 2: Creación, Espera y Retorno de Procesos con Ejecución externarno

Programar un proceso padre que cree N procesos hijo, siendo "N" un valor numérico 
(recibido como argumento opcional) entre 5 y 25, el cual si no viene será 5 y si es 
erróneo se indicará el error terminando el programa. El proceso padre cuando tenga a 
los hijos creados, esperará por la pulsación de CTRL+C (SIGINT), y cuando eso ocurra, 
indicará a los hijos su terminación de forma adecuada (con SIGTERM), esperará a que 
acaben todos, y obtendrá sus PIDs y sus valores de retorno mostrándolos por pantalla, 
antes de finalizar. Por otra parte, cada proceso hijo tras ser creado, mostrará su 
identificador de proceso por pantalla, configurará la señal de alarma (SIGALRM) para 
poder "dormir" (sin usar sleep) tantos segundos como el orden de creación del mismo 
(1 a N) más un valor aleatorio entre 0 y 5 segundos, y cuando despierte mostrará en 
pantalla un mensaje con su PID indicando que va a terminar, y acabará devolviendo al 
sistema el número de segundos que ha dormido.
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


// Funcion manejadora
void manejadora()
{
}

int misleep(int espera);


// Funcion main
int main(int argc, char * argv[])
{
	int *hijos, n_hijos, i, ale, status;
	sigset_t mascaranueva, mascaravieja, mascarahijos;
	struct sigaction accionNueva, accionVieja, accionHijos;
	
	sigfillset(&mascaranueva);
	sigprocmask(SIG_SETMASK,&mascaranueva,&mascaravieja);


	sigfillset(&mascarahijos);

	accionNueva.sa_handler=manejadora;
	accionNueva.sa_mask=mascaranueva;
	accionNueva.sa_flags=0;

	accionHijos.sa_handler=manejadora;
	accionHijos.sa_mask=mascarahijos;
	accionHijos.sa_flags=0;

	sigaction(SIGINT, &accionNueva,NULL);

	sigaction(SIGTERM, &accionHijos,NULL);	
	
	sigdelset(&mascaranueva,SIGINT);
	sigdelset(&mascarahijos,SIGTERM);

	
	//  Si el numero de argumentos es diferente a 2
	if(argc != 2)
	{
		fprintf(stderr, "ERROR: Faltan argumentos.\n");
		return FIN_FALLO;
	}
	
    // Guardamos el numero de hijos que queremos.
	n_hijos = atoi(argv[1]);

    
    // Si se introduce un numero fuera de parametros
	if(n_hijos <=2, n_hijos>=50)
	{
		fprintf(stderr,"ERROR: Numero de hijos incorrecto. 2>= N <= 50.\t");
		return FIN_FALLO;
	}

    // Reservamos memoria para los hijos
	hijos=(int*)malloc(n_hijos);


    // En el bucle creamos los hijos
	for(i=0;i<n_hijos;i++)
	{	
		hijos[i]=fork();        // Creamos el hijo
        // Realizamos la comprobacion
		if(hijos[i]==0)
		{
			srand(getpid());    // Num random
			ale=rand()%5+1+i;
			
			misleep(ale);   
			sigsuspend(&mascarahijos);
			printf("\nFinaliza hijo %d, PID -> %d. (Fue dormido %d s) \n", (i+1),getpid(), ale);
			return ale;
		}
		else if(hijos[i]==-1)
		{
			perror("ERROR creando hijos.");
			return FIN_FALLO;
		}
		else
		{
		}
	}
	
	printf("Espero a la señal SIGINT... (Ctl+C) \n");

	sigsuspend(&mascaranueva);

	for(i=0;i<n_hijos;i++)
	{
		kill(hijos[i],SIGTERM);
	}

	sigaction(SIGINT, &accionVieja,NULL);
	sigprocmask(SIG_SETMASK,&mascaravieja,NULL);

	for(i=0;i<n_hijos;i++)
	{
		waitpid(hijos[i],&status,0);
		printf("\tHijo %d ha acabado y ha devuelto %d\n",i,WEXITSTATUS(status));
	}

	return FIN_EXITO;

}

int misleep(int espera)
{
	int i;
	sigset_t mascaranueva,mascaravieja;
	struct sigaction accionNueva, acccionVieja;

	if(sigfillset(&mascaranueva)==-1)
	{
		perror("Fallo sigfillset");
		return FIN_FALLO;
	}

	accionNueva.sa_handler=manejadora;
	accionNueva.sa_mask=mascaranueva;
	accionNueva.sa_flags =0;

	if(sigaction(SIGALRM, &accionNueva, &acccionVieja)==-1)
	{
		perror("Fallo sigaction, accionNueva");
		return FIN_FALLO;
	}


	if(sigprocmask(SIG_SETMASK,&mascaranueva,&mascaravieja)==-1)
	{
		perror("Fallo sigprocmask, mascaranueva");
		return FIN_FALLO;
	}

	if(sigdelset(&mascaranueva,SIGALRM)==-1)
	{
		perror("Fallo sigdelset");
		return FIN_FALLO;
	}

	for(i=espera;i>0;i--)
	{
		//printf("%d,",i);
		alarm(1);
		sigsuspend(&mascaranueva);
	}
	//printf("0\n");

	if(sigaction(SIGALRM, &acccionVieja, NULL)==-1)
	{
		perror("Fallo sigaction, acccionVieja ");
		return FIN_FALLO;
	}

	if(sigprocmask(SIG_SETMASK,&mascaravieja,NULL)==-1)
	{
		perror("Fallo sigprocmask, mascaravieja");
		return FIN_FALLO;
	}	
	return FIN_EXITO;
}


