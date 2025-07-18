/*
 * sthread.c - Implements the public API (the functions defined in
 *             include/sthread.h). Since sthreads supports two implementations
 *             (pthreads and student supplied user-level threads), this
 *             just consists of dispatching the calls to the implementation
 *             that the application choose.
 *
 */

#include <config.h>

#include <assert.h>

#include <sthread.h>
#include <sthread_pthread.h> 
#include <sthread_user.h>

#ifdef USE_PTHREADS
#define IMPL_CHOOSE(pthread, user) pthread
#else
#define IMPL_CHOOSE(pthread, user) user
#endif

void sthread_init(void) {
  IMPL_CHOOSE(sthread_pthread_init(), sthread_user_init());
}

sthread_t sthread_create(sthread_start_func_t start_routine, void *arg, int priority) {
  sthread_t newth;
IMPL_CHOOSE(
  newth = sthread_pthread_create(start_routine, arg),
  newth = sthread_user_create(start_routine, arg, priority)
);
return newth;
}

void sthread_exit(void *ret) {
  IMPL_CHOOSE(sthread_pthread_exit(ret), sthread_user_exit(ret));
}

void sthread_yield(void) {
  IMPL_CHOOSE(sthread_pthread_yield(), sthread_user_yield());
}

int sthread_sleep(int time) {
   return IMPL_CHOOSE(sthread_pthread_sleep(time),sthread_user_sleep(time));
}

int sthread_join(sthread_t thread, void **value_ptr) {
  return IMPL_CHOOSE(sthread_pthread_join(thread,value_ptr),sthread_user_join(thread,value_ptr));
}

int sthread_self(){
  return set_user_Id();
}

/* implementada o sthread_user_dump */
/* void sthread_user_dump() {
    printf("\n--- DUMP de Threads ---\n");
    printf("Clock atual: %ld\n", Clock);
    printf("Thread ativa: TID=%d | prioridade=%d\n", active_thr->tid, active_thr->prioridade);

    printf("\n[Executáveis]\n");
    queue_print(exe_thr_list);

    printf("\n[Adormecidas]\n");
    queue_print(sleep_thr_list);

    printf("\n[Zumbis]\n");
    queue_print(zombie_thr_list);
}

void queue_print(queue_t *q) {
    queue_element_t *e = q->first;
    while (e != NULL) {
        printf("TID=%d | prioridade=%d\n", e->thread->tid, e->thread->prioridade);
        e = e->next;
    }
}
*/
void sthread_dump() {
  sthread_user_dump();
}

int sthread_nice(int nice) {
    return sthread_user_nice(nice);
}

/**********************************************************************/
/* Synchronization Primitives: Mutexs and Condition Variables         */
/**********************************************************************/


sthread_mutex_t sthread_mutex_init() {
  sthread_mutex_t lock;
  IMPL_CHOOSE(lock = sthread_pthread_mutex_init(),
	      lock = sthread_user_mutex_init());
  return lock;
}

void sthread_mutex_free(sthread_mutex_t lock) {
  IMPL_CHOOSE(sthread_pthread_mutex_free(lock),
	      sthread_user_mutex_free(lock));
}

void sthread_mutex_lock(sthread_mutex_t lock) {
  IMPL_CHOOSE(sthread_pthread_mutex_lock(lock),
	      sthread_user_mutex_lock(lock));
}

void sthread_mutex_unlock(sthread_mutex_t lock) {
  IMPL_CHOOSE(sthread_pthread_mutex_unlock(lock),
	      sthread_user_mutex_unlock(lock));
}

sthread_mon_t sthread_monitor_init() {
  sthread_mon_t mon;
  IMPL_CHOOSE(mon = sthread_dummy_monitor_init(),
	      mon = sthread_user_monitor_init());
  return mon;
}

void sthread_monitor_free(sthread_mon_t mon) {
  IMPL_CHOOSE(sthread_dummy_monitor_free(mon),
	      sthread_user_monitor_free(mon));
}

void sthread_monitor_enter(sthread_mon_t mon) {
  IMPL_CHOOSE(sthread_dummy_monitor_enter(mon),
	      sthread_user_monitor_enter(mon));
}

void sthread_monitor_exit(sthread_mon_t mon) {
  IMPL_CHOOSE(sthread_dummy_monitor_exit(mon),
	      sthread_user_monitor_exit(mon));
}


void sthread_monitor_wait(sthread_mon_t mon) {
  IMPL_CHOOSE(sthread_dummy_monitor_wait(mon),
	      sthread_user_monitor_wait(mon));
}

void sthread_monitor_signal(sthread_mon_t mon) {
  IMPL_CHOOSE(sthread_dummy_monitor_signal(mon),
	      sthread_user_monitor_signal(mon));
}


