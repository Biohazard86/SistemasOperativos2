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

void salida(void)
{ 
  //Lo que queramos que haga la función.
  fprintf(stdout,"SE EJECUTA LA FUNCION\n\n");
}//salida


void main(void)
{
  if (atexit( salida) == -1) {
    perror("atexit"); 
    exit(1);
  }
}//main