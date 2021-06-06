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


int main()
{

  sigset_t todas, anteriores, pendientes;

  //-----------------------------------------
  sigfillset( &todas);  
  sigprocmask( SIG_BLOCK, &todas, &anteriores);
  //sigprocmask( SIG_SETMASK, &todas, &anteriores);

  //-----------------------------------------
  printf("Soy el proceso %d.\n", getpid());
  printf("Se comprueban seniales en el rango [0..%d].\n", SIG_MAX-1);

  int no_pasa_nada= 1, segundos= 5;
  printf("La alarma va a llegar en %d segundos.\n", segundos);
  alarm( segundos);
  while (no_pasa_nada) {
    sigpending( &pendientes);

    ver_conjunto_bits( &pendientes);
    ver_conjunto_nombres( &pendientes);

    if (sigismember( &pendientes, SIGALRM)) {
      printf( "La alarma ha llegado\n");
      no_pasa_nada= 0;
    }

    //hacer lo que sea mientras se espera...
    //...
    sleep(1);
  }

  printf( "Dejo mascara como estaba.\n");
  sigprocmask( SIG_SETMASK, &anteriores, NULL);
  printf( "Salgo de forma normal\n");

}