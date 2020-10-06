#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

long thread_count;
long long n;
double sum;
int flag;

const int MAX_THREADS = 64;

void* Thread_sum(void* rank);
void Get_args(int argc, char* argv[]);
void Usage(char* prog_name);


int main(int argc, char* argv[]) {
   long       thread; 
   pthread_t* thread_handles;
   double start, finish, elapsed;

   Get_args(argc, argv);

   thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t)); 
   sum = 0.0;
   flag = 0;

   GET_TIME(start);

   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], NULL,
          Thread_sum, (void*)thread);  

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 
   

   GET_TIME(finish);
   elapsed = finish - start;

   sum = 4.0*sum;
   printf("With n = %lld terms,\n", n);
   printf("Estimate value of pi = %.15f\n", sum);
   printf("The elapsed time is %.4f seconds\n", elapsed);
   free(thread_handles);
   return 0;
}  /* main */


void* Thread_sum(void* rank) {
    long my_rank = (long) rank;
    double factor, my_sum = 0.0;

    long long i;
    long long my_n = n/thread_count;
    long long my_first_i = my_n*my_rank;
    long long my_last_i = my_first_i + my_n;
    
    if (my_first_i % 2 == 0)
        factor = 1.0;
    else
        factor = -1.0;

    //Pthreads global sum with busy-waiting
    /* for (i = my_first_i; i < my_last_i; i++, factor = - factor) {
        while (flag != my_rank);
        sum += factor/(2*i+1);
        flag = (flag+1) % thread_count;
    } */

    // Global sum function with critical section after loop
    for (i = my_first_i; i < my_last_i; i++, factor = - factor)
        my_sum += factor/(2*i+1);
    
    while (flag != my_rank);
    sum += my_sum;
    flag = (flag+1) % thread_count;

    return NULL;
}/* Thread sum */


void Get_args(int argc, char* argv[]) {
   if (argc != 3) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);
   n = strtoll(argv[2], NULL, 10);
   if (n <= 0) Usage(argv[0]);
}

void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads> <n>\n", prog_name);
   fprintf(stderr, "   n is the number of terms and should be >= 1\n");
   fprintf(stderr, "   n should be evenly divisible by the number of threads\n");
   exit(0);
}