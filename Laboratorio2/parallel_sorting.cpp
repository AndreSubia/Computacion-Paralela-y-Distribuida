#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <vector>

void Load(int my_keys[], int local_n, int my_rank) {
    int i;
    srandom(my_rank+1);
    for (i = 0; i < local_n; i++)
        my_keys[i] = random() % 1000;
}  

void Print_global_list(int my_keys[], int local_n, int my_rank, int p,
      MPI_Comm comm) {
   int* A = NULL;
   int i, n;

   if (my_rank == 0) {
      n = p*local_n;
      A = (int*) malloc(n*sizeof(int));
      MPI_Gather(my_keys, local_n, MPI_INT, A, local_n, MPI_INT, 0,comm);
      printf("Global list:\n");
      for (i = 0; i < n; i++)
         printf("%d ", A[i]);
      printf("\n\n");
      free(A);
   } else {
      MPI_Gather(my_keys, local_n, MPI_INT, A, local_n, MPI_INT, 0,comm);
   }

}  /* Print_global_list */

int Compare(const void* a_p, const void* b_p) {
   int a = *((int*)a_p);
   int b = *((int*)b_p);

   if (a < b)
      return -1;
   else if (a == b)
      return 0;
   else /* a > b */
      return 1;
}  /* Compare */


void Merge_low(int my_keys[], int temp_B[], int temp_C[],
        int local_n) {
   int m_i, r_i, t_i;

   m_i = 0;
   r_i = 0;
   t_i = 0;
   while (t_i < local_n) {
      if (my_keys[m_i] <= temp_B[r_i]) {
         temp_C[t_i] = my_keys[m_i];
         t_i++; m_i++;
      } else {
         temp_C[t_i] = temp_B[r_i];
         t_i++; r_i++;
      }
   }

   memcpy(my_keys, temp_C, local_n*sizeof(int));
}  /* Merge_low */

void Merge_high(int my_keys[], int temp_B[], int temp_C[],
        int local_n) {
   int m_i, r_i, t_i;

   m_i = local_n-1;
   r_i = local_n-1;
   t_i = local_n-1;
   while (t_i >= 0) {
      if (my_keys[m_i] >= temp_B[r_i]) {
         temp_C[t_i] = my_keys[m_i];
         t_i--; m_i--;
      } else {
         temp_C[t_i] = temp_B[r_i];
         t_i--; r_i--;
      }
   }

   memcpy(my_keys, temp_C, local_n*sizeof(int));
}  /* Merge_low */

void Odd_even_iter(int my_keys[], int temp_B[], int temp_C[],
        int local_n, int phase, int even_partner, int odd_partner,
        int my_rank, int p, MPI_Comm comm) {
   MPI_Status status;

   if (phase % 2 == 0) {  /* Even phase, odd process <-> rank-1 */
      if (even_partner >= 0) {
         MPI_Sendrecv(my_keys, local_n, MPI_INT, even_partner, 0,
            temp_B, local_n, MPI_INT, even_partner, 0, comm,
            &status);
         if (my_rank % 2 != 0)
            Merge_high(my_keys, temp_B, temp_C, local_n);
         else
            Merge_low(my_keys, temp_B, temp_C, local_n);
      }
   } else { /* Odd phase, odd process <-> rank+1 */
      if (odd_partner >= 0) {
         MPI_Sendrecv(my_keys, local_n, MPI_INT, odd_partner, 0,
            temp_B, local_n, MPI_INT, odd_partner, 0, comm,
            &status);
         if (my_rank % 2 != 0)
            Merge_low(my_keys, temp_B, temp_C, local_n);
         else
            Merge_high(my_keys, temp_B, temp_C, local_n);
      }
   }
}  /* Odd_even_iter */

void Print_list(int my_keys[], int local_n, int rank) {
   int i;
   printf("%d: ", rank);
   for (i = 0; i < local_n; i++)
      printf("%d ", my_keys[i]);
   printf("\n");
}  /* Print_list */

void Print_local_lists(int my_keys[], int local_n,
         int my_rank, int p, MPI_Comm comm) {
   int*       A;
   int        q;
   MPI_Status status;

   if (my_rank == 0) {
      A = (int*) malloc(local_n*sizeof(int));
      Print_list(my_keys, local_n, my_rank);
      for (q = 1; q < p; q++) {
         MPI_Recv(A, local_n, MPI_INT, q, 0, comm, &status);
         Print_list(A, local_n, q);
      }
      free(A);
   } else {
      MPI_Send(my_keys, local_n, MPI_INT, 0, 0, comm);
   }
}  /* Print_local_lists */


void Sort(int my_keys[], int local_n, int my_rank, int p, MPI_Comm comm) {
    int phase;
    int *temp_B, *temp_C;
    int even_partner;  /* phase is even or left-looking */
    int odd_partner;   /* phase is odd or right-looking */

    /* Temporary storage used in merge-split */
    temp_B = (int*) malloc(local_n*sizeof(int));
    temp_C = (int*) malloc(local_n*sizeof(int));

    /* Find partners:  negative rank => do nothing during phase */
    if (my_rank % 2 != 0) {
        even_partner = my_rank - 1;
        odd_partner = my_rank + 1;
        if (odd_partner == p) odd_partner = -1;  // Idle during odd phase
    } else {
        even_partner = my_rank + 1;
        if (even_partner == p) even_partner = -1;  // Idle during even phase
        odd_partner = my_rank-1;
    }

    /* Sort local list using built-in quick sort */
    qsort(my_keys, local_n, sizeof(int), Compare);

    for (phase = 0; phase < p; phase++)
        Odd_even_iter(my_keys, temp_B, temp_C, local_n, phase,
                even_partner, odd_partner, my_rank, p, comm);

    free(temp_B);
    free(temp_C);
}  /* Sort */


int main(int argc, char* argv[]) {
    
    std::vector<double> time;
    int local_n = 10000;//10000000;


    int my_rank, p;
    char g_i;
    int *my_keys;
    int global_n;

    MPI_Comm comm;
    double start, finish;

    MPI_Init(&argc, &argv);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &p);
    MPI_Comm_rank(comm, &my_rank);

    my_keys = (int*) malloc(local_n*sizeof(int));
    Load(my_keys, local_n, my_rank);

    start = MPI_Wtime();
    Sort(my_keys, local_n, my_rank, p, comm);
    finish = MPI_Wtime();
    if (my_rank == 0)
        printf("Elapsed time = %e ms\n", (finish-start)*1000);
    //Print_global_list(my_keys, local_n, my_rank, p, comm);
    free(my_keys);
    MPI_Finalize();

   return 0;
}  /* main */
