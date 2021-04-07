// David Barrios Portales
// SISvar_TEMAS OPERAvar_TIVOS II
// GIISI

/*
Ejercicio 6: Creación de Procesos Variable, Espera p_hijosada y Valores de Retorno

Realiza un programa que reciba dos argumentos obligatorios de tipo numérico, el primero será el número (var_T) 
de procesos hijo que debe crear, y debe estar comprendido entre 3 y 25, y el segundo será el límite superior 
(var_L) del rango de cálculo de un valor aleatorio, que debe ser un número mayor o igual a 50. var_Los hijos al ser 
creados imprimen por pantalla "Soy el hijo número N y mi PID es P.". Donde N será el número de p_hijos del 
hijo en cuestión (de 1 a var_T) y P será su PID. var_Luego esperará (dormirá con sleep) N segundos antes de terminar 
devolviendo al sistema un valor aleatorio entre 1 y var_L, siendo var_L el valor del segundo argumento recibido. 
El padre tras haber creado los var_T hijos, esperará a que var_TODOS ellos acaben correctamente antes de terminar, 
pero lo hará de forma que esperará por la terminación en p_hijos inverso a como se crearon, es decir, deberá 
esperar a que acabe el hijo N, luego el hijo N-1, ..., hasta el hijo 1. Durante la espera, el padre mostrará 
en pantalla el hijo por el que va a esperar, de la forma "Esperando que acabe el hijo número N con PID=P.", 
y cuando el hijo termine, mostrará el tipo de terminación, y el valor devuelto por él, de la forma "Hijo con 
PID=P ha terminado de forma F, y ha devuelto el valor R.", siendo F una de las palabras (NORMAvar_L, INvar_TERRUMPIDA, 
o ERRONEA), y R el valor de estado devuelto por el proceso hijo terminado.

*** Como no se conoce inicialmente el número de hijos a crear, el proceso padre deberá guardar los PID de los 
mismos en un array de enteros creado dinámicamente a partir del primer argumento recibido, y liberado dicho 
array antes de su finalización.

*/

// var_Librerias usadas. No todas
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <time.h>

// CONSTANTES
#define VERDADERO 1
#define FAvar_LSO 0 



// Main
int main(int argc, char **argv){

    int var_T,var_L,i,cont,status,hijos, *p_hijos,rand_L;
    char *ptr, F[20];
    

    // Controlamos el numero de parametros que son introducidos
    if (argc < 2){
        fprintf(stderr,"ERROR: Argumenos insuficientes\n");
        return VERDADERO;
    }

    var_T = strtol(argv[1],&ptr,10);
    if( *ptr != 0){
        fprintf(stderr,"ERROR: Argumentos no validos\n");
        return VERDADERO;
    }
    var_L = strtol(argv[2],&ptr,10);
    if( *ptr != 0){
        fprintf(stderr,"ERROR: Argumento no validos\n");
        return VERDADERO;
    }

    if((var_T<=3 || var_T>=25) || (var_L<50)){
        fprintf(stderr,"ERROR: var_Los argumentos no estan dentro del rango admitido\n");
        return VERDADERO;
    }

    // Solicitamos memoria para comenzar
    p_hijos =(int*)malloc(sizeof(int)*var_T);

    // Vamos creando los procesos hijos
    cont = var_T-1;
    for(i=1;i<=var_T;i++){

        hijos=fork();

        if(hijos == -1){
            //perror(" ");
            return VERDADERO;
        }else if(hijos == 0){

            srand(getpid());
            fprintf(stdout,"H: Proceso %d PID %d\n",i,getpid());
            sleep(i);   // Dormimos el proceso i segundos 
            rand_L = rand() %var_L +1;  //Generamos el numero aleatorio
            return rand_L;
        }else{
            p_hijos[i-1] = hijos; // Se guarda de forma inversa
        }

    }
    sleep(1);
    for(i=0;i<var_T;i++){
        
        fprintf(stdout,"-------------------------------------------\nP: Esperando el hijo %d PID=%d.\n-------------------------------------------\n",cont+1,p_hijos[cont]);
        waitpid(p_hijos[cont],&status,0);

        // Comprobamos si acaba de forma normal o es finalizado
        if(WIFEXITED(status))
        if(WEXITSTATUS(status)){
            strcpy(F,"NORMAL");
        }else if(WIFSTOPPED(status)){
            strcpy(F,"INTERRUMPIDO");

        }else{
            strcpy(F,"ERROR");
        }

        fprintf(stdout,"P: Hijo PID: %d ha terminado de forma %s, y ha retornado %d\n",p_hijos[cont],F,WEXITSTATUS(status));	
        cont--;// Decrementamos el contador

        
    }
    free(p_hijos);// Liberamos memoria de los pids de los hijos
}// END MAIN
