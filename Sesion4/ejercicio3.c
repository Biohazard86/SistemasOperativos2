// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

/*
Sesion 4
Ejercicio 3: Configuración, Captura, Envío y Espera de señales, Terminación de forma ordenada, ...

Realizar un programa cuyo proceso principal se preparará para recibir dos señales (configurará dos acciones 
con alguna o algunas funciones manejadoras), la de pulsación de CTRL+C (SIGINT) y la de alarma (SIGALRM). 
Entrará luego en un bucle del que SOLO saldrá cuando se pulse CTRL+C. En cada ciclo del bucle creará un 
proceso hijo, y "dormirá" (sin usar sleep) un número de segundos entre 1 y 5 (generado aleatoriamente), 
luego enviará la señal de terminación (SIGTERM) al proceso hijo creado previamente y esperará a que éste 
acabe, momento en el que se cerrará el ciclo del proceso principal (o padre). Aunque el proceso padre detecte 
CTRL+C, el ciclo debe terminarse por completo, es decir, no puede acabar inmediatamente. El proceso hijo cuando 
es creado, se preparará para recibir la señal de terminación, mostrará un mensaje en pantalla informando de su 
PID, y se quedará bloqueado (sin consumir CPU) hasta que llegue dicha señal, momento en el continuará su 
ejecución y terminará ordenadamente devolviendo 0 al sistema. 

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

void manejadora()
{
}

int misleep(int espera);

// Funcion main
int main(int argc, char * argv[])
{
	int *hijos, n_hijos, i, ale, status, flag=0;
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


    // Reservamos memoria para los hijos
	//hijos=(int*)malloc(n_hijos);

    i == 0;
    do{
        // Reservamos memoria para los hijos
        hijos=(int*)malloc(1);
        hijos[i]=fork();        // Creamos el hijo
        printf("\nA \n");
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

        printf("Espero a la señal SIGINT... (Ctl+C) \n");
        sigsuspend(&mascaranueva);
        kill(hijos[i],SIGTERM);
        sigaction(SIGINT, &accionVieja,NULL);
	    sigprocmask(SIG_SETMASK,&mascaravieja,NULL);

        waitpid(hijos[i],&status,0);
		printf("\tHijo %d ha acabado y ha devuelto %d\n",i,WEXITSTATUS(status));


        i++;
    }while(flag == 0);


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