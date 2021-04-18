// David Barrios Portales
// SISTEMAS OPERATIVOS II
// GIISI

/*
Sesion 4
Ejercicio 5: ...Terminación de forma ordenada y "Sincronización" entre procesos.no



Realizar un programa que admitirá dos argumentos opcionales, que serán valores numéricos a los que denominaremos HIJOS y RETARDO. 
El argumento HIJOS deberá ser un valor comprendido entre 5 y 25, el cual si no viene será 5 por defecto y si es erróneo se 
indicará el error terminando el programa. El argumento RETARDO deberá ser un valor comprendido entre -1 y -101, el cual si no 
viene será -1 por defecto y si es erróneo se indicará como error terminando el programa. Como véis se puede distinguir quién es 
cada uno por el rango de valores, con lo que no debe importar el orden de aparición en los argumentos. Al valor de RETARDO tras 
haberlo validado, le sumáis 1 y le quitáis el signo para que quede un número entre 0 y 100.

Con los argumentos obtenidos y validados, el proceso principal (futuro padre) configurará la señal que se recibe al pulsar CTRL+C 
en el terminal (SIGINT) y la de terminación (SIGTERM) para que, al recibir cualquiera de ellas, sepa que debe terminarse. También 
debe configurar, la señal de "Usuario 1" (SIGUSR1) para recibir avisos de otros procesos (hijos que va a tener) como si fuera un 
"testigo" o indicador de "turno". Tras esto, creará tantos procesos hijo como el valor de HIJOS, que deberá recordar para luego 
enviarles la señal de aviso (SIGUSR1) de que tienen el "turno", o la de terminación al final. Cada proceso hijo creado, como heredan 
la configuración de las señales del padre, deberán reconfigurarse para ignorar la señal de CTRL+C, pero seguirán manteniendo la de 
terminación y la de "Usuario 1", pues servirán para lo mismo (más o menos) que en el padre.

Tras la creación y preparación, todos los procesos entran en un ciclo de trabajo del que sólo saldrán cuando sepan que deben 
terminar (habrán recibido alguna señal que se lo indique). En el caso del proceso padre, antes de entrar en el ciclo, enviará 
el testigo del "turno", usando la señal de "Usuario 1" (SIGUSR1) al último proceso hijo creado.

Dentro del ciclo todos los procesos deberán esperar (sin consumir CPU prácticamente) a tener el turno que les permita seguir 
adelante. Cuando eso ocurra, imprimirán por la salida estándar un mensaje con el siguiente formato de printf: "%c%d(%05ld) %s.\n", 
siendo el primer argumento del formato (%c) un carácter que será la letra 'P' si es el proceso padre, o 'H' si es un proceso hijo, 
el segundo argumento (%d) será el número de orden del proceso, con el 0 para el padre, y de 1 a HIJOS para los hijos, el tercer 
argumento (%05ld) debe ser el PID del proceso formateado a 5 dígitos con espacios por la izquierda, y el cuarto argumento (%s) un 
texto con el contenido "Recibido Testigo". Después de imprimir el mensaje, harán una pausa de tantas décimas de segundo como 
indique el RETARDO enviado, el cual si es 0 no deberá hacer nada (aquí no va a servir el usar sleep o la señal de alarma por la 
resolución de segundos que permiten, pero la función usleep -obsoleta-, o mejor la función nanosleep puede que sí). Al final de 
la pausa, enviarán el testigo del "turno" (con la señal de "Usuario 1") al proceso que toque, que será siempre al último PID de 
proceso que conozcan, es decir, tanto para el proceso padre como para los procesos hijo será el PID del último hijo creado, a 
excepción del primer hijo que, por ser el primero, le enviará el testigo a su padre.

Si todo va bien, durante la ejecución cíclica de los procesos "obteniendo los turnos", se deberá ver en pantalla los mensajes que 
generan de forma "ordenada".

Cuando un proceso deba terminar, saldrá del ciclo ordenadamente, es decir, que lo acabará completamente y al intentar repetirlo 
no entrará de nuevo. En cuanto el proceso padre reciba la señal de CTRL+C o la de terminación (SIGTERM) sabe que debe terminar, 
los procesos hijo saben que deben acabar al recibir SÓLO la señal de terminación.

En el caso de un proceso hijo, tras acabar el bucle, mostrará en la salida estándar un mensaje con el formato indicado anteriormente, 
pero el argumento de mensaje (%s) será el texto "Finalizado".

En el caso del proceso padre, después del bucle, mostrará un mensaje con el formato indicado pero con el texto "Avisando Hijos", 
enviará a todos sus hijos la señal de terminación (SIGTERM) para que sepan que deben ir acabando, mostrará otro mensaje (mismo formato) 
con el texto "Esperando Hijos", esperará a que todos sus hijos hayan terminado y por último mostrará un mensaje con el texto "Programa 
Terminado".

*/

// Librerias usadas. No todas

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
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define FIN_FALLO 1
#define FIN_EXITO 0


// ------------------------------------------------------------------------------------------

int seguir=1;//cualquier número != de 0.Para que no salga del turno

//SI SE RECIBE LA SEÑAL SIGINT(para el padre) o SIGTERM(para los hijos), SE SALDRÁ DE LA ESPERA DE TURNO
void manejadora(int signum){
    if(signum==SIGINT || signum==SIGTERM)//Si no se añade SIGTERM, cuando los HIJOS la reciban, no saldrán de la espera de turno.
        seguir=0;
    
}

//Función turno, recibe PID de proceso anterior al que hace la llamada
void turno(int pid,int retardo,char letra,int orden){
    
    sigset_t mascara;//Se declara la máscara
    struct timespec espera;//Estructura para tiempo de espera de nanosleep
    
    //Se convierte convierte el retardo introducido a nanosegundos
    espera.tv_nsec=(retardo)*10000000;
    
    sigfillset(&mascara); //Se enmascara toda la máscara
    sigdelset(&mascara,SIGUSR1); //Se desenmascara SIGUSR1...SIGINT sólo sigue enmascarada para los hijos.
    
    while(seguir){
        sigsuspend(&mascara); //Se espera la señal SIGUSR1 ó SIGTERM()
        fprintf(stdout,"%c%d(%05d) %s.\n",letra,orden,getpid(),"Recibido Testigo");
        nanosleep(&espera,NULL);
        kill(pid,SIGUSR1);//Se envía SIGUSR1 al proceso indicado.
    }
}



int main(int argc, char *argv[])
{
    int hijos=5,retardo=0,argumento,i,j, *pid, status;
    struct sigaction accionnueva;
    sigset_t mascara;//Se declara la máscara para cambiarla
    
    //Se comprueba el número de argumentos.
    if (argc>3) {
        fprintf(stderr,"Sólo se permiten dos argumentos como máximo");
    }
    
    //Se comprueba si los argumentos son correctos
    for(i=1;i<argc;i++)
    {
        
        if (argv[i][0]=='-') {
            for (j=1; argv[i][j]!=NULL; j++) {
                if (argv[i][j]<'0'||argv[i][j]>'9'){
                    fprintf(stderr,"ERROR al introducir el argumento de retardo.\n");
                    return 1;
                }
            }
                retardo=-(atoi(argv[i])+1);//Se guarda retardo
                //printf("retardo =%d\n",retardo);
        } else {
            
            for (j=0; argv[i][j]!=NULL; j++) {
                if (argv[i][j]<'0'||argv[i][j]>'9'){
                    fprintf(stderr,"ERROR al introducir el número de hijos.\n");
                    return 1;
                }
                

            }
            hijos=atoi(argv[i]);//Se guarda el número de hijos a crear
            //printf("hijos =%d\n",hijos);

        }
        
    }//for
   
    //getchar();
    
    
    //Se determina cual será función manejadora y la máscara para la nueva acción
    accionnueva.sa_handler=manejadora;
    accionnueva.sa_mask=mascara;
    
    //Se cambia el comportamiento de las señales SIGUSR1, SIGINT y SIGTERM por el que se defina.
    if(sigaction(SIGUSR1,&accionnueva,NULL)==-1){
        perror("ERROR SIGUSR1");
        return 2;
    }
    if(sigaction(SIGINT,&accionnueva,NULL)==-1){
        perror("ERROR SIGINT");
        return 2;
    }
    if(sigaction(SIGTERM,&accionnueva,NULL)==-1){
        perror("ERROR SIGTERM");
        return 2;
    }
    
    sigemptyset(&mascara); //Se vacia la máscara.
    sigaddset(&mascara,SIGUSR1); //SE BLOQUEA SIGUSR1
    sigaddset(&mascara,SIGINT);	//SE BLOQUEA SIGINT

    if(sigprocmask(SIG_SETMASK,&mascara,NULL)==-1){ //Se aplica la máscara,de momento, común a Padre e hijos.
        perror("ERROR estableciendo la máscara..\n");
        return 3;
    }
    
    //Se reserva memoria para los PID de hijos y padre
    pid=malloc(sizeof(int)*(hijos+1));
    pid[0]=getpid();//Se guarda PID del padre
    
    
    
    //CREACIÓN DE HIJOS
    for(i=1; i<=hijos; i++){
        
        pid[i]=fork();//Se guarda el PID del hijo creado
       
        switch(pid[i]){
            
            case -1 :
                
                perror("ERRROR CREANDO HIJOS");
                
                return 2;
            
            case 0 : //HIJOS
            
                turno(pid[i-1],retardo,'H',i); //LLamada a función turno con el PID del proceso anterior.
                
                //Una vez recibida la señal SIGTERM.(enviada por el padre)
                fprintf(stdout,"%c%d(%05d) %s.\n",'H',i,getpid(),"Finalizado");
                exit(0);
            default:
                
                
                break;
        }//switch
    }//for
    
    
    
    
    //CÓDIGO PADRE
    sigdelset(&mascara,SIGINT);	//Se desenmascara SIGINT(para el padre)...sólo queda SIGUSR1
    if(sigprocmask(SIG_SETMASK,&mascara,NULL)==-1){ //Se aplica la nueva máscara y no se guarda para un uso posterior(el NULL)
        perror("ERROR enmascarando");
        return 3;
    }
    
    
    
    
    if(kill(pid[hijos],SIGUSR1)==-1){ //Se envía SIGUSR1 al último hijo para que comiencen a pasar el testigo
        perror("ERROR mandando señal SIGUSR1");
        return 3;
    }
    //ESPERA DE TURNO PADRE
    turno(pid[hijos],retardo,'P',0); //Se llama a turno para esperar el testigo
    
    
    //AQUÍ el padre YA HA RECIBIDO LA SEÑAL SIGINT
    fprintf(stdout,"%c%d(%05d) %s.\n",'P',0,getpid(),"Avisando Hijos");
    
    for(i=1;i<=hijos;i++){ //MATA A LOS HIJOS
        
        kill(pid[i],SIGTERM);
        //fprintf(stdout,"voy a matar al hijo%d\n",pid[i]);
        
    }
    fprintf(stdout,"%c%d(%05d) %s.\n",'P',0,getpid(),"Esperando Hijos");
    
    for(i=1;i<=hijos;i++){ //bucle para esperar por los procesos hijos, si hay un error se indica
        
        if(waitpid(pid[i],&status,0)==-1){
            perror("waitpid");
        }
        
    }
    
    
    fprintf(stdout,"%c%d(%05d) %s.\n",'P',0,getpid(),"Programa Terminado");	
    
    
    return 0;
}
