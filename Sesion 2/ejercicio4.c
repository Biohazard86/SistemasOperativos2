// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI


/*
Ejercicio 4
Realiza un programa que admitirá uno o varios argumentos que se corresponderán con nombres de archivo
 los cuales se deberán eliminar de forma segura. Con cada argumento recibido, el programa supondrá que 
 es el nombre de un fichero, el cual si existe, realizará el proceso de borrado seguro, es decir, 
 que lo abrirá para escritura y lo recorrerá completamente destruyendo su contenido escribiendo 
 en cada posición un caracter aleatorio entre los códigos '0' a '9', 'A' a 'Z' o 'a' a 'z'. 
 Tras acabar el recorrido, cerrará el archivo y lo eliminará del sistema. 
*/

// Librerias usadas
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>   // Para los numeros aleatorios




int borrado_seguro(int fd, char pathname[]){
    int i, fin_fichero, rand_elegido;
    // Cadena que contiene todos los posibles caracteres alternativos que se van a sustituir.
    const char rand_text[] = "abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ0123456789";
    char buf[1];    // Buffer para escribir en el archivo caracter a caracter

    fin_fichero = lseek( fd, 0, SEEK_END);  // Vemos en que posicion se encuentra el final del fichero.
    // For para recorrer letra a letra hasta fin de archivo
    for(i=0;i<fin_fichero;i++){
        // Elegimos un caracter aleatorio de rand_text, es decir, entre 0 y 59
        buf[0] = rand_text[rand_elegido = (rand () % (59-0+1) + 0)];   // Este está entre M y N
        // Ponemos el puntero de acceso en el principio del archivo
        // Si no hacemos esto, el apuntador escribira despues de lo que ya haya en el fichero
        lseek( fd, i, SEEK_SET);
        // Si la escritura falla (Se escribe un numero diferente al del tamanio del buffer)
        if (write( fd, buf, sizeof(buf[0])) != sizeof(buf[0])){ 
            // mostramos un mensaje de error
            fprintf(stderr,"Error al escribir un caracter\n");
        }else{
            //fprintf(stdout,"-> %c\n", buf[0]);
            
        }
        
    }
    
}// END BORRADO_SEGURO


// Main
int main(int argc, char *argv[]){
    int fd;     // File descriptor, vamos a guardar el descriptor del archivos que abrimos
    int i;
    srand(time(NULL));  // Para poder generar los numeros aleatorios diferentes, la semilla es TIME

    if(argc<2){ // Si no se introduce     

        fprintf(stderr, "No se han introducido suficientes argumentos.\nSALIENDO\n\n");
        exit (-1);
    }else{
        // Si el numero de parametros es correcto, vamos a abrir cada archivo
        for (i=1;i<argc;i++){

            fd = open(argv[i], O_WRONLY | O_RDONLY);      // abrimos el archivo en modo lectura / escritura.
            if(fd != -1){
                borrado_seguro(fd, argv[i]); // Realizamos el borrado seguro
                close(fd);          // Cerramos el archivo, ya que hemos acabado con el
                // Si todo va bien, entonces procedemos a ejecutar unlink
                if(unlink(argv[i]) == -1){
                    // Si todo va bien , la funcionre retorna 0, si algo falla retorna -1
                    fprintf(stdout,"No se ha podido eliminar el archivo.\n");
                }else{
                    fprintf(stdout,"El archivo %s Se ha borrado correctamente.\n", argv[i]);
                }
            }else{
                fprintf(stdout,"El archivo %s no se puede abrir. No se borra nada.\n", argv[i]);
            }
        }

    }// END ELSE
           
}// END MAIN