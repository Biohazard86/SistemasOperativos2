#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int i, num_parametros = argc;
    char parametros;

    for(i=0;i<num_parametros;i++){
        fprintf(stdout, "Parametro %d -> %s\n", i, argv[i]);
    }

}