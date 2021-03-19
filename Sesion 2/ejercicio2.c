//Sesion 2 
//Ejercicio 2

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
	int fd,i,cont=0;
	off_t len;
	ssize_t leer;
	char caracter;

	if(argc <=1 || argc > 2){
		fprintf(stderr,"Se esperaba un fichero como parametro\n");
	}else{
		fd = open(argv[1],O_RDONLY);
		if(fd == -1){
			fprintf(stderr, "Error en la apertura\n");
			perror("Error de ");
		}else{
			 
				while(leer=read(fd,&caracter,sizeof(caracter))>0){
					if(caracter == 'a'){
					caracter = 'B';
					cont++;
					}
					printf("%c",caracter);
				}
				if(leer == -1){
					fprintf(stderr, "Error en la lectura del fichero\n");
					perror(" ");
				} printf("\nhay %d caracteres 'a' en el texto\n",cont);
			
			close(fd);
		}
	}
}
