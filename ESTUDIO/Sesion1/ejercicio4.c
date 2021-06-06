#include <stdlib.h>
#include <stdio.h>


int main (int argc, char *argv[], char *environ[]){
    int i;
    char **envpp;

    // Vamos a ver todas las variables de entorno
    //Ver variables de entorno
  for (envpp= environ; (*envpp != NULL); envpp++) {
    fprintf( stderr, "Variable de entorno: %s\n", *envpp);
  }

  fprintf(stdout,"\n\n");

  //Ver variables de entorno con otra técnica...
  //...que se puede aplicar también a los argumentos.
  for (i=0; (environ[i] != NULL); i++) {
    fprintf( stderr, "Variable de entorno: %s\n", environ[i]);
  }//for

}