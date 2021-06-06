#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
    int i, valor_devuelto;
    int estado;
    pid_t pid;

    for(i=0;i<7;i++){
        valor_devuelto = fork();
        
        
        if (valor_devuelto == -1) {
            //Codigo del padre (y sin hijo)
            perror( "crear hijo");
            exit(1);//o lo que sea...
        
        } else if (valor_devuelto == 0) {
            //Por aqui sigue el codigo SOLO del hijo...
            fprintf(stderr, "Soy el hijo %d y voy a esperar 2 segundos\n", getpid());
            sleep(2);
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