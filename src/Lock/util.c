/*  util.c  */

#include "Lock.h"
#include "SpoolesThread.h"

#define MYDEBUG 0

/*--------------------------------------------------------------------*/
/*
   -----------------------
   lock the lock

   created -- 97aug22, cca
   -----------------------
*/
void
Lock_lock ( 
   Lock   *lock 
) {
/*
   ---------------
   check the input
   ---------------
*/
if ( lock == NULL ) {
   fprintf(stderr, "\n fatal error in Lock_lock(%p)"
           "\n bad input\n", lock) ;
   exit(-1) ;
}
/*
fprintf(stdout, "\n inside Lock_lock()") ;
fflush(stdout) ;
*/
#if MYDEBUG > 0
fprintf(stdout, "\n Lock_lock : std::mutex locking") ;
fflush(stdout) ;
#endif
SpoolesMutex_lock(lock->mutex) ;
#if MYDEBUG > 0
fprintf(stdout, "\n Lock_lock : std::mutex lock done") ;
fflush(stdout) ;
#endif
lock->nlocks++ ;

return ; }

/*--------------------------------------------------------------------*/
/*
   -----------------------
   unlock the lock

   created -- 97aug22, cca
   -----------------------
*/
void
Lock_unlock ( 
   Lock   *lock 
) {
/*
   ---------------
   check the input
   ---------------
*/
if ( lock == NULL ) {
   fprintf(stderr, "\n fatal error in Lock_unlock(%p)"
           "\n bad input\n", lock) ;
   exit(-1) ;
}
lock->nunlocks++ ;
/*
fprintf(stdout, "\n inside Lock_unlock()") ;
fflush(stdout) ;
*/
#if MYDEBUG > 0
fprintf(stdout, "\n Lock_unlock : std::mutex unlocking") ;
fflush(stdout) ;
#endif
SpoolesMutex_unlock(lock->mutex) ;
#if MYDEBUG > 0
fprintf(stdout, "\n Lock_unlock : std::mutex unlocking done") ;
fflush(stdout) ;
#endif

return ; }

/*--------------------------------------------------------------------*/
