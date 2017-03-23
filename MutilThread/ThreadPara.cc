#include <pthread.h>
#include <iostream>

using namespace std;

enum{
    NUM_THREADS = 5
};

void* say_hello(void* args)
{
    int i = *((int*)args);  //TypeTrans for the input parameter.
    cout << "Hello... in Thread" << i <<  endl;
}

int main(){
    pthread_t tids[NUM_THREADS];
    cout << "Hello in main.." << endl;
    for (int i = 0; i < NUM_THREADS; ++i) {
        // parameters: TheCreatThreadID, ThreadParameter, TheStartAddr, RunningFunctionParameter.
       int ret = pthread_create(&tids[i], NULL, say_hello, (void*)&i); 
       cout << "Create Pthread id = " << tids[i] << endl;
       if(ret != 0) // Failed
           cout << "pthread_create error:error_code= " << ret << endl;
    }
       pthread_exit(NULL);
}
