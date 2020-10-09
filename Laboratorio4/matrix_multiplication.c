#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

/* Global variables */
int     thread_count;
int     m, n;
double* A;
double* x;
double* y;

/* Serial functions */
void Usage(char* prog_name);
/* Parallel function */
void *Pth_mat_vect(void* rank);

/*------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   long       thread;
   pthread_t* thread_handles;
   double start, finish;

   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   thread_handles = malloc(thread_count*sizeof(pthread_t));

   printf("Enter m and n\n");
   scanf("%d%d", &m, &n);

   A = malloc(m*n*sizeof(double));
   
   for(int i=0;i<m;i++){
       for(int j=0;j<n;j++){
           A[i*n+j] = rand() % 100;
       }
   }

   x = malloc(n*sizeof(double));

   for(int i=0;i<n;i++){
       x[i] = rand() % 100;
   }

   y = malloc(m*sizeof(double));
   
   GET_TIME(start);

   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
         Pth_mat_vect, (void*) thread);

   for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);
   
   GET_TIME(finish);
   
   printf("Elapsed time = %.5f seconds\n", finish - start);
   
   free(A);
   free(x);
   free(y);
   free(thread_handles);

   return 0;
}  /* main */

void Usage (char* prog_name) {
   fprintf(stderr, "usage: %s <thread_count>\n", prog_name);
   exit(0);
}  /* Usage */

void *Pth_mat_vect(void* rank) {
   long my_rank = (long) rank;
   int i, j;
   int local_m = m/thread_count; 
   int my_first_row = my_rank*local_m;
   int my_last_row = my_first_row + local_m - 1;

   for (i = my_first_row; i <= my_last_row; i++) {
      y[i] = 0.0;
      for (j = 0; j < n; j++)
          y[i] += A[i*n+j]*x[j];
   }

   return NULL;
}  /* Pth_mat_vect */