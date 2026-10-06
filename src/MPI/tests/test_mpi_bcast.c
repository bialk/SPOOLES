#include "spoolesMPI.h"

int
main(int argc, char *argv[])
{
   IV   *values ;
   int   errors, globalErrors, myid ;

   MPI_Init(&argc, &argv) ;
   MPI_Comm_rank(MPI_COMM_WORLD, &myid) ;

   values = IV_new() ;
   if ( myid == 0 ) {
      IV_init(values, 4, NULL) ;
      IV_setEntry(values, 0, 3) ;
      IV_setEntry(values, 1, 1) ;
      IV_setEntry(values, 2, 4) ;
      IV_setEntry(values, 3, 1) ;
   }

   values = IV_MPI_Bcast(values, 0, 0, NULL, MPI_COMM_WORLD) ;
   errors = IV_size(values) != 4
         || IV_entry(values, 0) != 3
         || IV_entry(values, 1) != 1
         || IV_entry(values, 2) != 4
         || IV_entry(values, 3) != 1
         || maxTagMPI(MPI_COMM_WORLD) < 0 ;

   MPI_Allreduce(&errors, &globalErrors, 1, MPI_INT, MPI_SUM,
                 MPI_COMM_WORLD) ;
   IV_free(values) ;
   MPI_Finalize() ;

   return globalErrors == 0 ? 0 : 1 ;
}
