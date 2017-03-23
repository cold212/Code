/*  typedef struct
{
    int		detachstate;					线程的分离状态
    int     schedpolicy;   					线程调度策略
    struct	sched_param		schedparam;   	线程的调度参数
    int		inheritsched;					线程的继承性 
    int		scope; 							线程的作用域 
    size_t	guardsize; 						线程栈末尾的警戒缓冲区大小 
    int		stackaddr_set; 
	void* 	stackaddr; 						线程栈的位置 
    size_t	stacksize; 						线程栈的大小
}pthread_attr_t;
*/
#include <iostream>
#include <pthread.h>

using namespace std;

enum{NUM_THREADS = 5};

void* say_hello(void* args){
    cout << "Hello in thread: " <<  *((int*)args) << endl;
    //The additional Message when the thread exit, the status for the main routine;
    int status = 10 + *((int*)args);
    pthread_exit((void*)status); 
}

int main(){
    pthread_t tids[NUM_THREADS];
    int index[NUM_THREADS];

    pthread_attr_t attr; //The struct of pthread attr. Add int the pthread_create;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); // Mean could be joined;
    
    for (int i = 0; i < NUM_THREADS; ++i) {
        index[i] = i;
        int ret = pthread_create(&tids[i], &attr, say_hello, (void*)&(index[i]));
        if (ret != 0) {
            cout << "Pthread_create error : error_code = " << ret << endl;
        }
    }

    pthread_attr_destroy(&attr); // Free the memory
    void *status;
    for (int i = 0; i < NUM_THREADS; ++i) {
       int ret = pthread_join(tids[i], &status);
      if (ret != 0 ) {
          cout << "Pthread_join error : error_code = " << ret << endl;
      } 
      else{
        cout << "Pthread_join get status: " << (long)status << endl;
      }
    }
}
