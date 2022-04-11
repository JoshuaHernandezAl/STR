/*
==================================================================
Referencia:
https://linuxprograms.wordpress.com/2008/01/23/piping-in-threads/

Notes: compile with: gcc -o pipe pipe1.cpp -lpthread
==================================================================
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

/*
Referencia:
https://linuxprograms.wordpress.com/2008/01/23/piping-in-threads/
*/

int fd[2];//File descriptor for creating a pipe

//This function continously reads fd[0] for any input data byte
//If available, prints

void *reader(void *)
{

 int     count = 0;
 sleep (10);
  //Delay in starting the reading from the pipe

  while(1){
      char    ch;
      int     result;

      result = read (fd[0],&ch,1);
      if (result != 1) {
          perror("read");
          exit(3);
      } printf ("Reader: \t\t%d %c\n",++count,ch);
      sleep (1);
  }
}

//This function continously writes Alphabet into fd[1]

void *writer(void *)
{
    int     result;
    char    ch='A'; int     count = 0;

    while(1){

       result = write (fd[1], &ch,1);
       if (result != 1){
           perror ("write");
           exit (2);
       }

       printf ("Writer: %d %c\n", ++count, ch);

       if(ch == 'Z')
          ch = 'A'-1;

        ch++;
       sleep (2);
   }

}

int main()
{
   pthread_t       tid1,tid2;
   int             result;

   result = pipe (fd);

   if (result < 0){
        perror("pipe ");
        exit(1);
   }

   pthread_create(&tid1,NULL,reader,NULL);
   pthread_create(&tid2,NULL,writer,NULL);

   pthread_join(tid1,NULL);
   pthread_join(tid2,NULL);
}



//https://docs.google.com/forms/d/e/1FAIpQLSeY_zCu0oKOB1wpDKl7WiIbMEX9aBnTZbE_aTALHwGwzJtpWg/viewform 

//https://meet.google.com/xjw-zmxw-yzo 
 
