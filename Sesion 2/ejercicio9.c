// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

// NO FUNCIONA


/*
Ejercicio 9

Realiza un programa que admita dos argumentos, el primero será el nombre de un fichero, y el segundo el nuevo número 
identificador de grupo del mismo. Se deben comprobar que se reciben al menos dos argumentos, y que el segundo es un 
valor numérico. El programa debe cambiar el identificador del grupo al que pertenece el fichero por el identificador 
enviado. Se deberán detectar y mostrar las condiciones de error y devolver 0 al sistema operativo si no se produjeron 
errores y 1 en caso contrario. Además, en caso de que el fichero sea un enlace simbólico, debe cambiar el grupo del 
propio enlace, no de aquello a lo que apunta. Y por último, el propietario del fichero debe permanecer inalterado.

*/

// Librerias usadas. No todas
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>   // Para los numeros aleatorios
#include <sys/mman.h>
#define MAX_PARAMETROS 5

int is_symlink(const char *filename){
    struct stat p_statbuf;

    if (lstat(filename, &p_statbuf) < 0) {  /* if error occured */
        perror("calling stat()");
        exit(1);  /* end progam here */
    }

    if (S_ISLNK(p_statbuf.st_mode) == 1) {
        //printf("%s is a symbolic link\n", filename);
        return 1;
    } else {
        //printf("%s is NOT a symbolic link\n", filename);
        return 0;
    }
}


// Main
int main(int argc, char *argv[]){

    long val;
    int uno, dos, tres, cuatro, fd, permisos = 0;

    

    // Comprobamos que se reciben dos argumentos 
    if(argc != 3){
        fprintf(stderr, "ERROR al introducir el numero de argumentos. Saliendo.\n");
        exit -1;
    }else{
        // Si se han introducido bien los argumentos, continuamos.
        // Vamos a comprobar que el segundo parametro sea un numero
        

        // Vamos a cambiar los permisos de acceso al fichero pasado por parametro
        open(argv[1], O_WRONLY);
        //Vamos a concatenar los permisos
        uno = argv[2][0];
        dos = argv[2][1];
        tres = argv[2][2];
        cuatro = argv[2][3];
        uno = uno-48;
        dos = dos-48;
        tres = tres-48;
        cuatro = cuatro-48;
        permisos = uno*1000+dos*100+tres*10+cuatro;

        fprintf(stdout, "Los permisos son %d\n", permisos);

        if((is_symlink(argv[1])) == 1){
            // Es enlace simbolico
        }
        else{
            // No es enlace simbolico
        }

        fchmod(fd, permisos);
        close(fd);
    }
    
    
           
}// END MAIN