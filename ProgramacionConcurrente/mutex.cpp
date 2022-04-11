/*****************************************************************************
* FILE: thread10b.cpp
* DESCRIPTION:
* This example program illustrates the use of variables with mutex. Second
* initialization form for mutex
*
* AUTHOR: Luis A. García Espinosa
* Modified by: Luis A García Espinosa
* Created on: 15/10/2012
* LAST REVISED: 15/10/12
******************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMTHRDS 5
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  counter = 0;

void *functionC(void *arg);

int main()
{
   int rc1, rc2;
   pthread_t thread[NUMTHRDS];
   for (int i=0;i<NUMTHRDS;i++){
	   /* Create independent threads each of which will execute functionC */
	   if((rc1=pthread_create(&thread[i],NULL,functionC, NULL)))
	   {
		   printf("Thread %d creation failed: %d\n", i, rc1);
	   }
   }

   /* Wait till threads are complete before main continues. Unless we  */
   /* wait we run the risk of executing an exit which will terminate   */
   /* the process and all threads before the threads have completed.   */

   for(int i=0;i<NUMTHRDS;i++) {
	   pthread_join(thread[i],NULL);
	   pthread_join(thread[i],NULL);
   }
   exit(0);
}

void *functionC(void *arg)
{
   pthread_mutex_lock(&mutex1);
   counter++;
   printf("Contador = %d\n",counter);
   pthread_mutex_unlock(&mutex1);
   pthread_exit(NULL);
}
