// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

// FUNCIONA, LISTO PARA ENTREGAR


/*
Ejercicio 1: Información del Proceso

Dado que todo proceso en UNIX tiene mucha información disponible, y sobre todo tiene algún 
padre, realiza un programa que al ejecutarse muestre la siguiente información: 

Datos del Proceso:
  ID Proceso..............: ¿?
  ID Padre................: ¿?
  ID Usuario..............: ¿?
  ID Usuario Efectivo.....: ¿?
  ID Grupo Usuarios.......: ¿?
  ID Grupo Usr. Efectivo..: ¿?
  ID Sesion...............: ¿?
  ID Grupo Procesos.......: ¿?
  Prioridad...............: ¿?
  Directorio de Trabajo...: ¿?
  Argumentos:
    Nombre Ejecutable: ¿?
    Argumento 1 .....: ¿?
    Argumento 2 .....: ¿?
    Argumento 3 .....: ¿?
    [...]
    Argumento N .....: ¿?

*/

// Librerias usadas. No todas
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>





// Main
int main(int argc, char *argv[]){

	int i;
	char buf[1024];
	//char *buf;
	char *ptr;
	ptr = getcwd(buf, 1024);

	fprintf(stdout, "ID Proceso..................: %d\n", getpid());
	fprintf(stdout, "ID Proceso..................: %d\n", getppid());
	fprintf(stdout, "ID Usuario..................: %d\n", getuid());
	fprintf(stdout, "ID Usuario Efectivo.........: %d\n", geteuid());
	fprintf(stdout, "ID Grupo Usuarios...........: %d\n", getgid());
	fprintf(stdout, "ID Grupo Usr. Efectivo......: %d\n", getegid());
	fprintf(stdout, "ID Grupo Procesos...........: %d\n", getpgrp());
	fprintf(stdout, "Prioridad...................: %s\n", ptr);
	fprintf(stdout, "Argumentos: \n");
	fprintf(stdout, "    Nombre Ejecutable..: %s\n", argv[1]);
	for(i=1;i<argc-1;i++){
		fprintf(stdout, "    Argumento %d........: %s\n", i, argv[i+1]);	
	}


          
}// END MAIN
