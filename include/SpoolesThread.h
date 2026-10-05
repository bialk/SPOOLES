#ifndef _SPOOLESTHREAD_
#define _SPOOLESTHREAD_

#ifdef __cplusplus
extern "C" {
#endif

typedef void *(*SpoolesThreadFunction)(void *) ;

typedef struct _SpoolesThread {
   void   *handle ;
} SpoolesThread ;

int
SpoolesThread_create (
   SpoolesThread           *thread,
   SpoolesThreadFunction   function,
   void                    *argument
) ;

int
SpoolesThread_join (
   SpoolesThread   *thread
) ;

void *
SpoolesMutex_new (
   void
) ;

void
SpoolesMutex_free (
   void   *mutex
) ;

void
SpoolesMutex_lock (
   void   *mutex
) ;

void
SpoolesMutex_unlock (
   void   *mutex
) ;

#ifdef __cplusplus
}
#endif

#endif
