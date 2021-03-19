// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI


/*
Ejercicio 3
Realiza un programa que al ser ejecutado cree un fichero en el directorio activo con el nombre “ejecutor” 
(si ya existe que lo haga de nuevo), escriba dentro el texto [echo “Archivo ejecutado desde un proceso...”] 
(sin los corchetes) y lo cierre. Luego dará al fichero creado el permiso de ejecución y ordenará su ejecución 
utilizando la llamada del sistema correspondiente (consultar la función “system”, pero sólo por esta vez). 
*/

// Librerias usadas
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

// Main
int main(int argc, char *argv[]){
    int fd;     // File descriptor, vamos a guardar el descriptor del archivos que abrimos
    const char name_file[] = "ejecutor.bin";        // Nombre del archivo que vamosa  crear
    const char text_file[] = "echo \"Archivo ejecutado desde un proceso...\" ";        // Texto que vamos a imprimir en el archivo

    // Mostramos el siguiente mensaje por el canal de salida estandar
    fprintf(stdout,"Se procede a crear un archivo llamado '%s'\n", name_file); 
    fd = open(name_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);      // Creamos (Abrimos) el archivo con permisos de lectura y escritura para todos.
    if(fd == -1){                       // Si hay algun error, mostramos el mensaje por el canal de errores 
        fprintf(stderr, "ERROR al crear el archivo %s\n", name_file);
    }
    else{                               // Si no hay error, seguimos con el proceso normal.
        
        // Imprimimos que se ha creado bien el archivo
        fprintf(stdout,"Se ha creado el archivo %s correctamente\n", name_file);   
        // Y se va a proceder a introducir el texto 
        fprintf(stdout,"Se procede a introducir el texto:\n   'echo Archivo ejecutado desde un proceso...'\n");
        
        //lseek(fd, 1000, SEEK_SET);     // Situamos el puntero en el principio del archivo

        // Si la funcion write no nos devuelve un error:
        if(write(fd, text_file, strlen(text_file)) == -1){     // Escribimos en el fichero el texto. Si hay algun error
            fprintf(stderr, "ERROR al escribir en el archivo\n");   // Mostramos un mensaje de error
            close(fd);  // Cerramos el archivo
        }
        else{   // Si todo va bien 
            // Mostramos por el canal estandar que todo ha ido bien
            fprintf(stderr, "Se ha escrito sin errores\n");
            if(fchmod(fd, 777) == -1){ //cambiamos los permisos del archivo
                // Si ha habido algun error mostramos el mensaje siguiente por el canal de errores
                fprintf(stderr, "ERROR al modificar los permisos\n");
            }
            close(fd);          // Si no, simplemente cerramos el archivo
        }
        
        
    }


}