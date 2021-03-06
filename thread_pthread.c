#include "quakedef.h"
#include "thread.h"
#ifdef THREADRECURSIVE
#define __USE_UNIX98
#include <pthread.h>
#endif
#include <stdint.h>


int Thread_Init(void)
{
	return 0;
}

void Thread_Shutdown(void)
{
}

qboolean Thread_HasThreads(void)
{
	return true;
}

void *_Thread_CreateMutex(const char *filename, int fileline)
{
#ifdef THREADRECURSIVE
	pthread_mutexattr_t    attr;
#endif
	pthread_mutex_t *mutexp = (pthread_mutex_t *) Z_Malloc(sizeof(pthread_mutex_t));
#ifdef THREADDEBUG
	Sys_PrintfToTerminal("%p mutex create %s:%i\n" , mutexp, filename, fileline);
#endif
#ifdef THREADRECURSIVE
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(mutexp, &attr);
#else
	pthread_mutex_init(mutexp, NULL);
#endif
	return mutexp;
}

void _Thread_DestroyMutex(void *mutex, const char *filename, int fileline)
{
	pthread_mutex_t *mutexp = (pthread_mutex_t *) mutex;
#ifdef THREADDEBUG
	Sys_PrintfToTerminal("%p mutex destroy %s:%i\n", mutex, filename, fileline);
#endif
	pthread_mutex_destroy(mutexp);
	Z_Free(mutexp);
}

int _Thread_LockMutex(void *mutex, const char *filename, int fileline)
{
	pthread_mutex_t *mutexp = (pthread_mutex_t *) mutex;
#ifdef THREADDEBUG
	Sys_PrintfToTerminal("%p mutex lock %s:%i\n"   , mutex, filename, fileline);
#endif
	return pthread_mutex_lock(mutexp);
}

int _Thread_UnlockMutex(void *mutex, const char *filename, int fileline)
{
	pthread_mutex_t *mutexp = (pthread_mutex_t *) mutex;
#ifdef THREADDEBUG
	Sys_PrintfToTerminal("%p mutex unlock %s:%i\n" , mutex, filename, fileline);
#endif
	return pthread_mutex_unlock(mutexp);
}

void *_Thread_CreateCond(const char *filename, int fileline)
{
	pthread_cond_t *condp = (pthread_cond_t *) Z_Malloc(sizeof(pthread_cond_t));
	pthread_cond_init(condp, NULL);
#ifdef THREADDEBUG
	Sys_PrintfToTerminal("%p cond create %s:%i\n"   , condp, filename, fileline);
#endif
	return condp;
}

void _Thread_DestroyCond(void *cond, const char *filename, int fileline)
{
	pthread_cond_t *condp = (pthread_cond_t *) cond;
#ifdef THREADDEBUG
	Sys_PrintfToTerminal("%p cond destroy %s:%i\n"   , cond, filename, fileline);
#endif
	pthread_cond_destroy(condp);
	Z_Free(condp);
}

int _Thread_CondSignal(void *cond, const char *filename, int fileline)
{
	pthread_cond_t *condp = (pthread_cond_t *) cond;
#ifdef THREADDEBUG
	Sys_PrintfToTerminal("%p cond signal %s:%i\n"   , cond, filename, fileline);
#endif
	return pthread_cond_signal(condp);
}

int _Thread_CondBroadcast(void *cond, const char *filename, int fileline)
{
	pthread_cond_t *condp = (pthread_cond_t *) cond;
#ifdef THREADDEBUG
	Sys_PrintfToTerminal("%p cond broadcast %s:%i\n"   , cond, filename, fileline);
#endif
	return pthread_cond_broadcast(condp);
}

int _Thread_CondWait(void *cond, void *mutex, const char *filename, int fileline)
{
	pthread_cond_t *condp = (pthread_cond_t *) cond;
	pthread_mutex_t *mutexp = (pthread_mutex_t *) mutex;
#ifdef THREADDEBUG
	Sys_PrintfToTerminal("%p cond wait %s:%i\n"   , cond, filename, fileline);
#endif
	return pthread_cond_wait(condp, mutexp);
}

void *_Thread_CreateThread(int (*fn)(void *), void *data, const char *filename, int fileline)
{
	pthread_t *threadp = (pthread_t *) Z_Malloc(sizeof(pthread_t));
#ifdef THREADDEBUG
	Sys_PrintfToTerminal("%p thread create %s:%i\n"   , threadp, filename, fileline);
#endif
	int r = pthread_create(threadp, NULL, (void * (*) (void *)) fn, data);
	if(r)
	{
		Z_Free(threadp);
		return NULL;
	}
	return threadp;
}

int _Thread_WaitThread(void *thread, int retval, const char *filename, int fileline)
{
	pthread_t *threadp = (pthread_t *) thread;
	void *status = (void *) (intptr_t) retval;
#ifdef THREADDEBUG
	Sys_PrintfToTerminal("%p thread wait %s:%i\n"   , thread, filename, fileline);
#endif
	pthread_join(*threadp, &status);
	Z_Free(threadp);
	return (int) (intptr_t) status;
}


