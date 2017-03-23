#include <pthread.h>
#include <iostream>

using namespace std;

enum{
    NUM_THREADS = 5
};

void* say_hello(void* args)
{
    cout << "Hello... in Thread:" << *(int*)args <<  endl;
}

int main(){
    pthread_t tids[NUM_THREADS];
    int index[NUM_THREADS];

    cout << "Hello in main.." << endl;

    for (int i = 0; i < NUM_THREADS; ++i) {
       index[i] = i; 
       int ret = pthread_create(&tids[i], NULL, say_hello, (void*)&(index[i])); 
       cout << "Create Pthread id = " << tids[i] << endl;
       if(ret != 0) // Failed
           cout << "pthread_create error:error_code= " << ret << endl;
    }
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(tids[i], NULL);        
    }
}
