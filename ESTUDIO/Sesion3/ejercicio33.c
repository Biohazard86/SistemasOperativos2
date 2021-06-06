#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>




int main(int argc, char *argv[]){
    int i, valor_devuelto;
    int estado, numero_veces;
    pid_t pid;

    numero_veces = atoi(argv[1]);

    for(i=0;i<numero_veces;i++){
        valor_devuelto = fork();
        
        
        if (valor_devuelto == -1) {
            //Codigo del padre (y sin hijo)
            perror( "crear hijo");
            exit(1);//o lo que sea...
        
        } else if (valor_devuelto == 0) {
            //Por aqui sigue el codigo SOLO del hijo...
            fprintf(stderr, "Soy el hijo número %d y mi PID es %d.\n", i, getpid());
            sleep(1);
            return -1;
        
        } else {
        //Por aqui sigue el codigo SOLO del padre...
        //pid_t wait(int &estado);

        
        
        pid= wait( &estado);
        if (pid == -1) {
        
        } else if (WIFEXITED( estado)) {
            printf( "El hijo %d con PID %d ha devuelto el valor %d.\n", i, 
            pid, WEXITSTATUS( estado));
        } else {
        printf( "El hijo %d con PID %d no ha terminado normalmente.\n", i, pid);
        }//if
        
        }//if

    }
}
//pid_t wait(int *status);