// LIBRERIAS
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

int main (int argc, char *argv[]){

    // Variables
    int i, num_vars = argc;
    char buff[100];

    // Mostramos lo datos:
    fprintf(stdout, "ID Proceso............. %d \n", getpid());
    fprintf(stdout, "ID Padre............... %d \n", getppid());
    fprintf(stdout, "ID Usuario............. %d \n", getuid());
    fprintf(stdout, "ID Usuario efectivo.... %d \n", geteuid());
    fprintf(stdout, "ID Grupo usuarios...... %d \n", getgid());
    fprintf(stdout, "ID Grupo Usr.Efectivo.. %d \n", getegid());
    fprintf(stdout, "ID Sesion.............. %d \n", getsid(0));
    fprintf(stdout, "ID Grupo procesos...... %d \n", getpgrp());
    fprintf(stdout, "Prioridad.............. %d \n", getpriority(PRIO_PROCESS, 0));
    getcwd(buff, 100);
    fprintf(stdout, "Directorio de trabajo.. %s \n", buff);
    fprintf(stdout, "Argumentos: \n");
    fprintf(stdout, "\tNombre ejecutable: %s \n", argv[0]);

    if(num_vars > 1){
        // Recorremos y mostramos todos los argumentos
        for(i=1;i<argc;i++){
            fprintf(stdout, "\tArgumento %s: \n", i, argv[i]);
        }

    }
    else{
        fprintf(stdout,"\tNo se han introducido argumentos\n");
    }


}