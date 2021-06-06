// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

/*
Sesion 4
Ejercicio 1: Creación, Espera y Retorno de Procesos con Ejecución externarno



Realizar una función en "C" que se llame "misleep", a la que se le pase el número de segundos que tiene el programa que esperar sin consumir prácticamente CPU, 
e imprima una cuenta atrás de esos segundos por la salida estándar.

La función realizará la configuración y activación de las señales necesarias, también hará el bloqueo de señales que se necesiten mientras tanto, etc. 
No se puede usar la función de biblioteca "sleep", como es lógico.

La función no puede afectar al comportamiento de las señales que tenga establecidas el proceso que la llama, es decir, que si se modifica el comportamiento 
al entrar en la función, debe ser restaurado antes de retornar de ella.

La función devolverá el valor 0 si todo ha ido bien, o -1 si hubo algún error interno ¡¡Siempre debe devolver un valor!! -Con lo que NO PUEDE ACABAR el proceso 
brúscamente, es decir, no se puede usar exit dentro de ella. Tampoco debe imprimir nada que no se pida, para no estorbar en la salida de pantalla o similar del proceso que la usa.

Para probar la función, realizar un programa básico que admita como argumento el número de segundos que se deben esperar o "dormir", y use la función desarrollada para ello...

int misleep( int segundos)
{
  [...lo que hace la funcion...]
}//misleep

int main( [...] )
{
  dormir= [...obtener y validar argumento recibido...]
  
  misleep( dormir);
}//main

...así, por ejemplo, al usar el programa desde la línea de comandos:
		programa 3

...debería salir por pantalla
		3, 2, 1, 0.
...haciendo una pausa de un segundo entre cada número que sale en pantalla.

. 
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
#define SIG_MAX (SIGRTMAX+2)
#define FIN_FALLO 1     // Lo que retornamos si hay fallo
#define FIN_EXITO 0     // Si no hay fallo

void manejadora()
{
}

int misleep(int espera);
int no_pasa_nada= 1;

// Funcion MAIN
int main(int argc, char * argv[])
{
	int dormir, retorno;    // Los segundos a dormir y el valor retornado por la funcion.
	
	



    //  Si el numero de argumentos es diferente a 2
	if(argc != 2)
	{
		fprintf(stderr, "ERROR: Faltan argumentos.\n");
		return FIN_FALLO;
	}

    // Guardamos los segundos que se va a dormir
	dormir= atoi(argv[1]);

    // Si el valor de los segundos es 0, se informa y se sale
	if(dormir == 0)
	{
		fprintf(stderr, "ERROR: Valor no valido\n");
		return FIN_FALLO;
	}

	

 

    // Si no hay ningun error, entonces llamamos a la funcion misleep y guardamos su retorno en una variable
	retorno= misleep(dormir);

    // Comprobamos que el retorno de la funcion es correcto.
	if(retorno==1)
	{	
		fprintf(stderr, "ERROR:La funcion ha develto -1\n");
	}
    else{
        fprintf(stdout, "La funcion ha develto 0\n");
    }

	return retorno;
}

//-----------------------------------------
void algo(int valor)
{
  printf( "Interrumpido por... %s\n", strsignal(valor));

  if (valor == SIGALRM) {
    no_pasa_nada= 0;
  }

}

// Funcion misleep
int misleep(int espera)
{

	sigset_t todas, anteriores, pendientes;
  struct sigaction acNueva, acAnterior;

  //-----------------------------------------
  //configurar alguna señal.
  acNueva.sa_handler= algo;
  //acNueva.sa_mask= todas;
  sigemptyset( &acNueva.sa_mask);
  acNueva.sa_flags= 0;

  sigaction( SIGALRM, &acNueva, &acAnterior);
  sigaction( SIGINT, &acNueva, NULL);
  sigaction( SIGTERM, &acNueva, NULL);


  //-----------------------------------------
  sigfillset( &todas);
  sigdelset( &todas, SIGALRM);
  sigdelset( &todas, SIGINT);
  sigdelset( &todas, SIGTERM);

  //sigprocmask( SIG_BLOCK, &todas, &anteriores);
  sigprocmask( SIG_SETMASK, &todas, &anteriores);

  //-----------------------------------------
  printf("Soy el proceso %d.\n", getpid());
  printf("Se comprueban seniales en el rango [0..%d].\n", SIG_MAX-1);

  int segundos= espera, contador;
  printf("La alarma va a llegar en %d segundos.\n", segundos);
  alarm( segundos);

  //-----------------------------------------
  contador = espera;
  while (no_pasa_nada) {
    sigpending( &pendientes);

    //ver_conjunto_bits( &pendientes);
    //ver_conjunto_nombres( &pendientes);

    if (sigismember( &pendientes, SIGALRM)) {
      printf( "La alarma ha llegado\n");
    }

    //hacer lo que sea mientras se espera...
    //...
    contador--;
    printf("%d\n", contador);
    sleep(1);
	return 0;
  }

  //-----------------------------------------
  //Esperar a que llegue alguna señal habilitada y configurada.
  pause();
  //printf( "Continuar tras pausa...\n");

  //-----------------------------------------
  //Esperar a que llegue la señal configurada SIGTERM.
//sigaddset( &todas, SIGALRM);
//sigaddset( &todas, SIGINT);
//sigsuspend( &todas);
  //printf( "Continuar tras suspension...\n");

  //-----------------------------------------
  //--printf( "Dejo mascara como estaba.\n");
  //--sigprocmask( SIG_SETMASK, &anteriores, NULL);
  //printf( "Salgo de forma normal\n");
}