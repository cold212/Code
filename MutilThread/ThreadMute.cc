#include <iostream>
#include <pthread.h>

using namespace std;

#define NUM_THREADS 5

int sum = 0;//global var for sum of all threads
pthread_mutex_t sum_mutex; //Mutex

void* say_hello(void* args){
    cout << "Hello in  thread " << *((int*)args) << endl;
    pthread_mutex_lock(&sum_mutex);
    cout <<  "Before sum is " << sum << "in thread " << *((int*)args) << endl;
    sum += *((int*)args);
    cout << "After sum is " << sum << "in thread" << *((int*)args) << endl;
    pthread_mutex_unlock(&sum_mutex);
    pthread_exit(0);
}

int main(){
    pthread_t tids[NUM_THREADS];
    int index[NUM_THREADS];

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_mutex_init(&sum_mutex, NULL);

    for (int i = 0; i < NUM_THREADS; ++i) {
        index[i] = i;
        int ret = pthread_create(&tids[i], &attr, say_hello, (void*)&index[i]);
        if (ret != 0) {
            cout << "Pthread_create error : error_code = " << ret << endl;
        }
    }
    pthread_attr_destroy(&attr);
    void *status;
    for (int i = 0; i < NUM_THREADS; ++i) {
        int ret = pthread_join(tids[i], &status);
        if (ret != 0) {
            cout << "Pthread_join error : error_code = " << ret << endl;
        }
    }
    cout <<  "Finally sum is : " << sum << endl;
    pthread_mutex_destroy(&sum_mutex);
}
