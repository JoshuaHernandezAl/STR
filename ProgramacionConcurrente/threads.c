/******************************************************************************
* FILE: threads01.cpp
*
* DESCRIPTION:
* A Pthreads program.  Demonstrates creation and termination of threads
* Pthread meaning: POSIX-Thread
* AUTHOR: Luis Garcia
* CREATED:      13/09/12
* LAST REVISED: 26/08/13
* Note: Use -pthread linker option compiling
******************************************************************************/
#include <pthread.h> //Creacion de hilos en estandar POSIX (Portable Operating System Interface for UNIX)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_THREADS	5

void *holaMundo(void *threadid);
/*
  Para cumplir con POSIX
  siempre se tiene que regresar un dato de tipo apuntador vacio, asi mismo eso aplica con su argumento
*/

int main(int argc, char *argv[])
{
  pthread_t threads[NUM_THREADS];//Creacion de una estructura tipo hilo
  int rc;//Response code, recibe las respuesta de las funciones de creacion de hilos
  long t;//Identificador de hilos
  void *status;//Localidad de memoria axiliar
  for(t=0;t<NUM_THREADS;t++){
    printf("Hilo principal: creando hilo %ld\n", t);
     rc = pthread_create(&threads[t], NULL, holaMundo, (void *)t);
    //Todos los hilos tienen atributos, si no se defiene se utilizan los por defecto
    if (rc){
      printf("ERROR; El código regresado por funcion pthread_create() es: %d\n", rc);
      exit(-1);
    }

     // Retardo que permite ver la ejecucuión ordenada de hilos
     //sleep(4);
     //sleep(random()%2);
     //printf("\n\n");
     }	//fin bloque for

  rc = pthread_join(threads[0],&status);
  rc = pthread_join(threads[1],&status);
  rc = pthread_join(threads[2],&status);
  rc = pthread_join(threads[3],&status);
  rc = pthread_join(threads[4],&status);

   /* Last thing that main() should do */
  printf("Hilo principal: terminado\n\n");
  pthread_exit(NULL);
}

// Funcion cuya tarea es mostrar en consola un mensaje
void *holaMundo(void *threadid)
{
  long tid;
  tid = (long)threadid;
  printf("Hilo #%ld: ¡Hola mundo! Soy el hilo #%ld\n", tid,tid);
  pthread_exit(NULL);
}
