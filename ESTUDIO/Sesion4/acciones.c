#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define SIG_MAX (SIGRTMAX+2)

void ver_conjunto_bits( const sigset_t * set)
{
  int i, is;
  char bits[SIG_MAX+1];

  for( i= 0; i < SIG_MAX; i++) {
    is= sigismember( set, i);
    //--bits[i]= ((is == -1) ? '-' : (is ? '1' : '0'));
    bits[SIG_MAX-i-1]= ((is == -1) ? '-' : (is ? '1' : '0'));
  }//for
  bits[SIG_MAX]= 0; //Lo mismo que '\0'
  printf( "%s\n", bits);
}

void ver_conjunto_nombres( const sigset_t * set)
{
  int i, is, total= 0;

  for( i= 0; i < SIG_MAX; i++) {
    is= sigismember( set, i);
    if (is && (is != -1)) {
      if (total > 0) printf(", ");
      printf( "%d:%s", i, strsignal(i));
      total++;
    }//if
  }//for
  if (total > 0) printf( " - total %d.\n", total);
}


//-----------------------------------------
//GLOBALES PARA QUE SEAN VISIBLES
int no_pasa_nada= 1;

//-----------------------------------------
void algo(int valor)
{
  printf( "Interrumpido por... %s\n", strsignal(valor));

  if (valor == SIGALRM) {
    no_pasa_nada= 0;
  }

}



//-----------------------------------------
int main()
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

  int segundos= 20, contador;
  printf("La alarma va a llegar en %d segundos.\n", segundos);
  alarm( segundos);

  //-----------------------------------------
  contador = 0;
  while (no_pasa_nada) {
    sigpending( &pendientes);

    ver_conjunto_bits( &pendientes);
    ver_conjunto_nombres( &pendientes);

    if (sigismember( &pendientes, SIGALRM)) {
      printf( "La alarma ha llegado\n");
    }

    //hacer lo que sea mientras se espera...
    //...
    contador++;
    printf("%d\n", contador);
    sleep(1);
  }

  //-----------------------------------------
  //Esperar a que llegue alguna señal habilitada y configurada.
  pause();
  printf( "Continuar tras pausa...\n");

  //-----------------------------------------
  //Esperar a que llegue la señal configurada SIGTERM.
//sigaddset( &todas, SIGALRM);
//sigaddset( &todas, SIGINT);
//sigsuspend( &todas);
  printf( "Continuar tras suspension...\n");

  //-----------------------------------------
  //--printf( "Dejo mascara como estaba.\n");
  //--sigprocmask( SIG_SETMASK, &anteriores, NULL);
  printf( "Salgo de forma normal\n");

}