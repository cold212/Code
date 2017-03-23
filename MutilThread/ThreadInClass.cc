#include <pthread.h>
#include <iostream>

using namespace std;

enum{
    NUM_THREADS = 5
};

//In the class, in order to be called by thread. It need be a static 
//funtion which stored in the static global area.
class Hello{
    public:
    static void* say_hello(void* args){
    cout << "Hello..." << endl;
    }
};

int main(){
    pthread_t tids[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i) {
        // parameters: TheCreatThreadID, ThreadParameter, TheStartAddr, RunningFunctionParameter.
       int ret = pthread_create(&tids[i], NULL, Hello::say_hello, NULL); //Class Function call. 
       if(ret != 0) // Failed
           cout << "pthread_create error:error_code= " << ret << endl;
    }
       pthread_exit(NULL);
}
