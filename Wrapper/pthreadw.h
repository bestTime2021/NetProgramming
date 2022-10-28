/*
 * wrapper for pthread functions
 */
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void posix_error(int error, const char *msg);

void Pthread_create(pthread_t *tid, const pthread_attr_t *attr, //attr alway NULL
										void *(*func)(void*), void *func_arg);
void Pthread_join(pthread_t tid, void **thread_return);
pthread_t Pthread_self();
void Pthread_detach(pthread_t tid);
void Pthread_exit(void *status);

void Pthread_key_create(pthread_key_t *rl_key, void (*destructor)(void* value));
void Pthread_once(pthread_once_t *onceptr, void (*init)());
void *Pthread_getspecific(pthread_key_t key);
void Pthread_setspecific(pthread_key_t key, const void *value);

void Pthread_mutex_lock(pthread_mutex_t *mptr);
void Pthread_mutex_unlock(pthread_mutex_t *mptr);

void Pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr);
void Pthreada_cond_timedwait(pthread_cond_t *cptr, pthread_mutex_t *mptr, const struct timespec *abstime); 
void Pthread_cond_signal(pthread_cond_t *cptr);
void Pthread_cond_broadcast(pthread_cond_t *cptr);

/********************************************
 *							 Implement
 ********************************************/
void posix_error(int error, const char *msg) {
	fprintf(stderr, "%s: %s\n", msg, strerror(error));
	exit(0);
}

void Pthread_create(pthread_t *tid, const pthread_attr_t *attr, 
									 void *(*func)(void*), void *arg) {
	int error;
	if ( (error = pthread_create(tid, attr, func, arg)) != 0)
		posix_error(error, "pthread_create() error");
}


void Pthread_join(pthread_t tid, void **thread_return) {
	int error;
	if ( (error = pthread_join(tid, thread_return)) != 0)
		posix_error(error, "pthread_create() error");
}

pthread_t Pthread_self() {
	return pthread_self();
}

void Pthread_detach(pthread_t tid) {
	int error;
	if ( (error = pthread_detach(tid)) != 0)
		posix_error(error, "pthread_detach() error");
}

void Pthread_exit(void *status) {
	pthread_exit(status);
}

void Pthread_key_create(pthread_key_t *rl_key, void (*destructor)(void* value)){
	int error;
	if ( (error = pthread_key_create(rl_key, destructor)) != 0)
		posix_error(error, "pthread_key_create() error");
}

void Pthread_once(pthread_once_t *onceptr, void (*init)()) {
	//*onceptr can == PTHREAD_ONCE_INIT
	int error;
	if ( (error = pthread_once(onceptr, init)) != 0)
		posix_error(error, "pthread_once() error");
}

void *Pthread_getspecific(pthread_key_t key) {
	void *pkeyi;
	if ( (pkeyi = pthread_getspecific(key)) == NULL)
		fprintf(stderr, "pthread_getspecific() error\n"), exit(1);

	return pkeyi;
}

void Pthread_setspecific(pthread_key_t key, const void *value) {
	int error;
	if ( (error = pthread_setspecific(key, value)) != 0)
		posix_error(error, "pthread_setspecific() error");
}

void Pthread_mutex_lock(pthread_mutex_t *mptr) {
	int error;
	if ( (error = pthread_mutex_lock(mptr)) != 0)
		posix_error(error, "pthread_mutex_lock() error");
}

void Pthread_mutex_unlock(pthread_mutex_t *mptr) {
	int error;
	if ( (error = pthread_mutex_unlock(mptr)) != 0)
		posix_error(error, "pthread_mutex_unlock() error");
}

void Pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr) {
	int error;
	if ( (error = pthread_cond_wait(cptr, mptr)) != 0)
		posix_error(error, "pthread_cond_wait() error");
}

void Pthreada_cond_timedwait(pthread_cond_t *cptr, pthread_mutex_t *mptr, const struct timespec *abstime) {
	int error = 0;
	if ( (error = pthread_cond_timedwait(cptr, mptr, abstime)) != 0)
		posix_error(error, "pthread_cond_timedwait() error");
}

void Pthread_cond_signal(pthread_cond_t *cptr) {
	int error;
	if ( (error = pthread_cond_signal(cptr)) != 0)
		posix_error(error, "pthread_cond_signal() error");
}

void Pthread_cond_broadcast(pthread_cond_t *cptr) {
	int error;
	if ( (error = pthread_cond_broadcast(cptr)) != 0)
		posix_error(error, "pthread_cond_broadcast() error");
}
