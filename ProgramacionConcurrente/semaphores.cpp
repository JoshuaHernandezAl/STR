
// C program to demonstrate working of Semaphores
// 
// References:
// https://www.geeksforgeeks.org/use-posix-semaphores-c/
// http://man7.org/linux/man-pages/man3/sem_init.3.html
// Compilation should be done with gcc a.c -lpthread -lrt
// Date: 11-oct-2019

#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
  
sem_t mutex;

void* thread(void*);
  
int main() 
{ 
    // arg 2. 0: to be shared between threads of a process
    // arg 3. nonzero: to be shared shared between
    //        processes, and should be located in a region
    //        of shared memory (see  shm_open(3), mmap(2),
    //        and shmget(2)).
    sem_init(&mutex, 0, 1); 
    pthread_t t1,t2; 
    pthread_create(&t1,NULL,thread,NULL);
    sleep(2); 
    pthread_create(&t2,NULL,thread,NULL); 
    pthread_join(t1,NULL); 
    pthread_join(t2,NULL); 
    sem_destroy(&mutex); 
    return 0; 
} 

void* thread(void* arg) { 
    //wait 
    sem_wait(&mutex); 
    printf("\nEntered..\n"); 
  
    //critical section 
    sleep(4); 
      
    //signal 
    printf("\nJust Exiting...\n"); 
    sem_post(&mutex); 
}
