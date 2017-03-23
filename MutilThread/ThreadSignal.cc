#include <iostream>
#include <pthread.h>
#include <stdio.h>

using namespace std;

#define BOUNDARY 5

int tasks = 10;
pthread_mutex_t tasks_mutex;
pthread_cond_t tasks_cond;  //Conditional Signal variable;

void* say_hello2(void* args){
    pthread_t pid = pthread_self();     // Get the current thread id
    cout << "[" << pid << "] hello in thread " << *((int*)args) << endl;

    bool is_signaled = false;
    while(1){
        pthread_mutex_lock(&tasks_mutex); //Lock
        if (tasks > BOUNDARY) {
            cout << "[" << pid << "] take task: " << tasks << " in thread " << *((int*)args) << endl;
           --tasks; // Modify 
        }
        else if (!is_signaled) {
            cout << "[" << pid << "] pthread_cond_signal in thread " << *((int*)args) << endl;
            pthread_cond_signal(&tasks_cond);
            is_signaled = true; //Signal has sent, exit this thread;
        }
        pthread_mutex_unlock(&tasks_mutex); //Unlock
        if (tasks == 0) {
            break;
        }
    }
}

void* say_hello1(void* args){
    pthread_t pid = pthread_self(); //Get the current thread
    cout << "[" << pid << "] hello in thread " << *((int*)args) << endl;

   while(1){
    pthread_mutex_lock(&tasks_mutex); //Lock
    if (tasks > BOUNDARY) {
        cout << "[" << pid << "] pthread_cond_signal in thread " << *((int*)args) << endl;
        pthread_cond_wait(&tasks_cond, &tasks_mutex); //Waiting for signal
    }
    else{
        cout << "[" << pid << "] take task: " << tasks << " in thread " << *((int*)args) << endl;
        --tasks;
    }
    pthread_mutex_unlock(&tasks_mutex);    //Unlock
    if (tasks == 0) {
        break;
    }
   } 
}

int main(){
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_cond_init(&tasks_cond, NULL);
    pthread_mutex_init(&tasks_mutex, NULL);
    pthread_t tid1, tid2; // Two thread
    int index1 = 1;
    int ret = pthread_create(&tid1, &attr, say_hello1, (void*)&index1);
    if (ret != 0) {
        cout << "pthread_create error:error_code = "<< ret << endl;
    }
    int index2 = 2;
    ret = pthread_create(&tid2, &attr, say_hello2, (void*)&index2);
    if (ret != 0) {
        cout << "pthread_create error:error_code = "<< ret << endl;
    }
    pthread_join(tid1, NULL); //Connect two thread
    pthread_join(tid2 , NULL);

    pthread_attr_destroy(&attr);            //Free the memory
    pthread_mutex_destroy(&tasks_mutex);    //logoff the mutex
    pthread_cond_destroy(&tasks_cond);      //Exit Nornally 

}
