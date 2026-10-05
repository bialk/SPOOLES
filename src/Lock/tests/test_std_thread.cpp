#include "SpoolesThread.h"

#include <cstdlib>

namespace {

struct Counter {
   void   *mutex ;
   int    value ;
} ;

void *
incrementCounter (
   void   *argument
) {
   Counter *counter = static_cast<Counter *>(argument) ;
   for ( int iteration = 0 ; iteration < 10000 ; ++iteration ) {
      SpoolesMutex_lock(counter->mutex) ;
      ++counter->value ;
      SpoolesMutex_unlock(counter->mutex) ;
   }
   return(nullptr) ;
}

}

int
main (
   void
) {
   const int      threadCount = 8 ;
   Counter        counter = { SpoolesMutex_new(), 0 } ;
   SpoolesThread  threads[threadCount] = {} ;

   if ( counter.mutex == nullptr ) {
      return(EXIT_FAILURE) ;
   }
   for ( int thread = 0 ; thread < threadCount ; ++thread ) {
      if ( SpoolesThread_create(&threads[thread], incrementCounter,
                                &counter) != 0 ) {
         return(EXIT_FAILURE) ;
      }
   }
   for ( int thread = 0 ; thread < threadCount ; ++thread ) {
      if ( SpoolesThread_join(&threads[thread]) != 0 ) {
         return(EXIT_FAILURE) ;
      }
   }
   SpoolesMutex_free(counter.mutex) ;

   return(counter.value == threadCount * 10000
          ? EXIT_SUCCESS : EXIT_FAILURE) ;
}
