/*Sesion2*/
#include<stdio.h>
#include <sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
/* 
bucle argc{
-detectar parametros (1)
-fich=abrir(nombre) para lectura
-leido=leer(fic,car,1) ->si lee 0 es porque ha llegado al final
	mientras(leido>0){
		si car='a' {car='o' cambios++}
		-imprimir (car)
	-leido=leer(fic,car,1)
	}
-cerrar
}
*/
#define BUFFER 1
int main(int argc, char *argv[]){
	int fd; 
	int error=0,cambios=0;
	off_t len;
	//char caracter[BUFFER];
	char caracter;
	ssize_t leido;
	char mensaje[1000];
	if(argc<=1){
		error=1;
		fprintf(stderr,"Error, debe incluir el nombre de archivo.\n");
		return error;
	}//if

		fd=open(argv[1],O_RDONLY);
		if(fd==-1){
		error=2;
		sprintf(mensaje,"Error al abrir el fichero [%s] : ",argv[1]); //preparo mensaje en memoria
		perror(mensaje);
		}else{
			leido=read(fd,&caracter,sizeof(caracter));
			while(leido >0){
			if(caracter == 'a'){
				caracter='o';
				cambios++;
			}
	printf("%c",caracter);
	leido=read(fd,&caracter,sizeof(caracter));
			}//while
	if(leido==-1){
		error=3;
		sprintf(mensaje,"Error de lectura [%s]",argv[1]);
		perror(mensaje);
	}
	printf("Se han echo %d cambios.\n",cambios);

			close(fd);
		}//if abrir	

	return error;
}//main
