#include <myself.h>

int main() {
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_init(&mutexattr);
    //pthread_mutexattr_settype(&mutexattr,PTHREAD_MUTEX_NORMAL);
    //pthread_mutexattr_settype(&mutexattr,PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutexattr_settype(&mutexattr,PTHREAD_MUTEX_NORMAL);
    pthread_mutex_init(&mutex,&mutexattr);
    int ret = pthread_mutex_lock(&mutex);
    THREAD_ERROR_CHECK(ret,"pthread_mutex_lock 1");
    ret = pthread_mutex_lock(&mutex);
    THREAD_ERROR_CHECK(ret,"pthread_mutex_lock 2");
    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&mutexattr);
}