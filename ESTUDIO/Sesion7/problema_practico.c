//-----------------------------------------------------------------
// Control de un Buzon de Paso de Mensajes.
//-----------------------------------------------------------------

//-----------------------------------------------------------------
//Bibliotecas de Funciones
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#include <sys/ipc.h>
#include <sys/msg.h>

//-----------------------------------------------------------------
//Definiciones
typedef struct {
  long tipo;
  char remite[12];
  char mensaje[80];
} MENSAJE;


void ayuda()
{
  fprintf( stdout, 
"nada o -h: imprime la ayuda del programa.\n"
"  -e tipo \"mensaje\": el programa envia a un buzon el\n"
"     mensaje \"mensaje\" con el tipo \"tipo\".\n"
"  -r tipo: el programa recibe un mensaje del tipo \"tipo\".\n"
"     Si no lo hay, se quedará bloqueado esperando.\n"
"  -b: el programa elimina la cola de mensajes si existe.\n"
  );
}//ayuda

//-----------------------------------------------------------------
//Programa principal
int main( int argc, char * argv[])
{
  int tmp, opcion= 0;
  char * pErr;
  MENSAJE m;
  int clave, idBuzon;

  //Detectar y Validar argumentos.
  if (argc <= 1) {
    ayuda();
    exit(0);
  } else if (strcmp( argv[1], "-h") == 0) {
    ayuda();
    exit(0);
  } else if (strcmp( argv[1], "-e") == 0) {
    if (argc >= 4) {
      tmp= strtol( argv[2], &pErr, 10);
      if ((*pErr != '\0') || (tmp <= 0)) {
        fprintf( stderr, "Error, Argumento \"tipo\" incorrecto.\n");
        ayuda();
        exit( 1);
      } else {
        m.tipo= tmp;
        opcion= 'e';
      }
      //Preparar el Remitente
      strncpy( m.remite, argv[0], sizeof(m.remite)-1);
      m.remite[ sizeof(m.remite)-1]= '\0';
      //Preparar el Mensaje
      strncpy( m.mensaje, argv[3], sizeof(m.mensaje)-1);
      //m.mensaje[ sizeof(m.mensaje)-1]= 0;
      m.mensaje[ sizeof(m.mensaje)-1]= '\0';
    } else {
      fprintf( stderr, "Error, Faltan argumentos.\n");
      ayuda();
      exit(1);
    }
  } else if (strcmp( argv[1], "-r") == 0) {
    if (argc >= 3) {
      tmp= strtol( argv[2], &pErr, 10);
      if (*pErr != '\0') {
        fprintf( stderr, "Error, Argumento \"tipo\" incorrecto.\n");
        ayuda();
        exit( 1);
      } else {
        m.tipo= tmp;
        opcion= 'r';
      }
    } else {
      fprintf( stderr, "Error, Faltan argumentos.\n");
      ayuda();
      exit(1);
    }
  } else if (strcmp( argv[1], "-b") == 0) {
    opcion= 'b';
  } else {
    fprintf( stderr, "Error, Argumentos incorrectos.\n");
    ayuda();
    exit(1);
  }
  
  //Si continua por aqui es una opcion a ejecutar.

  //Preparar Clave del Buzon.
  clave= ftok( "/bin/ls", 'q');

  //Abrir el Buzon si existe...
  idBuzon= msgget( clave, 0);
  if ((idBuzon == -1) && (errno == ENOENT) && (opcion != 'b')) {
    //Crear el Buzon para Enviar o Recibir.
    idBuzon= msgget( clave, IPC_CREAT | IPC_EXCL | 0660);
    if (idBuzon == -1) {
      perror("Crear Buzon");
      exit(2);
    }
  } else if (idBuzon == -1) {
    perror("Abrir Buzon");
    exit(2);
  }
  //Ejecutar la Opcion que corresponda.
  switch (opcion) {
  case 'e': //Enviar Mensaje.
    tmp= msgsnd( idBuzon, &m, sizeof(MENSAJE)-sizeof(long), 0);
    break;
  case 'r': //Recibir Mensaje.
    tmp= msgrcv( idBuzon, &m, sizeof(MENSAJE), m.tipo, 0);
    if (tmp != -1) {
      fprintf( stdout, "%s: %s\n", m.remite, m.mensaje);
    }
    break;
  case 'b': //Borrar Buzon.
    tmp= msgctl( idBuzon, IPC_RMID, NULL);
    break;
  }
  if (tmp == -1) {
    perror("Buzon");
    exit(3);
  }

  //Terminar
  return 0;
}//main

