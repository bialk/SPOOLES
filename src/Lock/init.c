/*  init.c  */

#include "Lock.h"
#include "SpoolesThread.h"

/*--------------------------------------------------------------------*/
/*
   ------------------------------------------------------------------
   purpose -- basic initializer
 
   lockflag -- flag to specify lock status
      SOLARIS:
      lockflag = 0 --> mutex lock is not allocated or initialized
      lockflag = 1 --> mutex lock is allocated and it can synchronize
                       only threads in this process.
      lockflag = 2 --> mutex lock is allocated and it can synchronize
                       only threads in this and other processes.
      POSIX:
      lockflag = 0 --> mutex lock is not allocated or initialized
      lockflag = 1 --> mutex lock is allocated and it can synchronize
                       only threads in this process.
 
   created -- 97aug22, cca
   ------------------------------------------------------------------
*/
void
Lock_init (
   Lock   *lock,
   int    lockflag
) {
if ( lockflag > 0 ) {
/*
   -----------------
   allocate the lock
   -----------------
*/
   lock->mutex = SpoolesMutex_new() ;
   if ( lock->mutex == NULL ) {
      fprintf(stderr, "\n fatal error in Lock_init(), unable to allocate mutex\n") ;
      exit(-1) ;
   }
}
return ; }

/*--------------------------------------------------------------------*/
