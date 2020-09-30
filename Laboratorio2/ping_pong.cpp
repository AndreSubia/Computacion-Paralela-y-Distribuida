#include <stdio.h>
#include <string.h>/* For strlen */
#include <mpi.h> /*For MPI functions, etc */

int main(void){
    
    int limit = 10;
    int my_rank;
    int comm_sz;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    int count = 0;
    int partner_rank = (my_rank + 1) % 2;
    
    while (count < limit) {
        if (my_rank == count % 2) {
            count++;
            MPI_Send(&count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
            printf("%d -> %d : Envio e incremento el contador a " "%d\n", my_rank, partner_rank, count);
        } else {
            MPI_Recv(&count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%d <- %d : Recibio el contador en %d\n", my_rank, partner_rank, count);
        }
    }
    return 0;
}


