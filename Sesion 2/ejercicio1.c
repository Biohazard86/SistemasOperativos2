#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

off_t len;


int main(int argc, char *argv[]){
    int i, fd;
    char mensaje[1000];

    if(argc > 1){
        fprintf(stdout, "Lista de los archivos que se van a acceder\n");
        for(i=1;i<argc;i++){
            fd = open(argv[i], O_RDONLY);
            if(fd == -1){
                sprintf(mensaje,"\tArchivo %s --> ERROR", argv[i]);
                perror(mensaje);
                printf("\n");
            }
            else{
                len = lseek(fd, 0, SEEK_END);
                if(len == -1){
                    sprintf(mensaje, "Error al situar el puntero\n");
                    perror(mensaje);
                }
                else{
                    fprintf(stdout,"\tArchivo %s --> %d\n", argv[i], len);
                }
                close(fd);
            }
            
        }
    }
    else{
        fprintf(stdout, "No se ha introducido ningun nombre de archivo.\n");
        return 1;
    }

    
        
    return 1;
}
