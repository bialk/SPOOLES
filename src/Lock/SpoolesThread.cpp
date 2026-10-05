#include "SpoolesThread.h"

#include <mutex>
#include <new>
#include <system_error>
#include <thread>

extern "C" int
SpoolesThread_create (
   SpoolesThread           *thread,
   SpoolesThreadFunction   function,
   void                    *argument
) {
   if ( thread == nullptr || function == nullptr ) {
      return(-1) ;
   }
   thread->handle = nullptr ;
   try {
      thread->handle = new std::thread([function, argument]() {
         function(argument) ;
      }) ;
   } catch ( const std::system_error& error ) {
      return(error.code().value() != 0 ? error.code().value() : -1) ;
   } catch ( ... ) {
      return(-1) ;
   }
   return(0) ;
}

extern "C" int
SpoolesThread_join (
   SpoolesThread   *thread
) {
   if ( thread == nullptr || thread->handle == nullptr ) {
      return(-1) ;
   }
   std::thread *worker = static_cast<std::thread *>(thread->handle) ;
   try {
      worker->join() ;
   } catch ( const std::system_error& error ) {
      if ( worker->joinable() ) {
         worker->detach() ;
      }
      delete worker ;
      thread->handle = nullptr ;
      return(error.code().value() != 0 ? error.code().value() : -1) ;
   }
   delete worker ;
   thread->handle = nullptr ;
   return(0) ;
}

extern "C" void *
SpoolesMutex_new (
   void
) {
   return(new (std::nothrow) std::mutex) ;
}

extern "C" void
SpoolesMutex_free (
   void   *mutex
) {
   delete static_cast<std::mutex *>(mutex) ;
}

extern "C" void
SpoolesMutex_lock (
   void   *mutex
) {
   static_cast<std::mutex *>(mutex)->lock() ;
}

extern "C" void
SpoolesMutex_unlock (
   void   *mutex
) {
   static_cast<std::mutex *>(mutex)->unlock() ;
}
