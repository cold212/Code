// threadSample.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>
#include <stdlib.h>

#include <iostream>		//input，output stream
#include <thread>		// thread，   this_thread::sleep_for
#include <chrono>		// chrono::seconds
#include <functional>
#include <atomic>
#include <mutex>
#include <vector>
#include <future>		// promise,future

#include <cmath>		// math，   sin
#include <atomic>		//atomic, atomic_flag, ATOMIC_FLAG_INIT
#include <sstream>		// stringstream

using namespace std;

volatile int counter(0); // Non-atomic counter
mutex mtx;				 // Mutex lock
timed_mutex tmtx;		 // Time Mutex lock
int count_num = 0;
mutex foo, bar;
promise<int> prom;

void thread_task() {
	cout << "Hello thread " << endl;
}

void Sample_1() {
	// 1.a simple sample for thread
	thread t(thread_task);
	t.join();
}


void thread_function_1(int n) {
	for (int i = 0; i < 5; ++i) {
		cout << "Thread " << n << " executing\n" << endl;
		this_thread::sleep_for(chrono::microseconds(10));
	}
}

void thread_function_2(int& n) {
	for (int i = 0; i < 5; ++i) {
		cout << "Thread 2 executing\n" << endl;
		++n;
		this_thread::sleep_for(chrono::microseconds(10));
	}
}

void Sample_2() {
	// 2. Differ construct function example
	int n = 0;
	thread t1;					// t1 is not a thread
	thread t2(thread_function_1, n + 1);		// pass by value
	thread t3(thread_function_2, ref(n));		// pass by reference
	thread t4(move(t3));		// t4 is now running fucntion2(). t3 is no longer a thread
	t2.join();
	t4.join();
	cout << "Final value of n is " << n << endl;
}


void thread_task2(int n) {
	this_thread::sleep_for(chrono::seconds(n));
	cout << "Hello thread " << this_thread::get_id() << " paused " << n << " seconds" << endl;
}

void Sample_3() {
	//3. The memory function of thread
	/*
	get_id 		获取线程 ID。

	joinable	检查线程是否可被 join。

	join		Join 线程。

	detach		Detach 线程

	swap		Swap 线程 。

	native_handle	返回 native handle。

	hardware_concurrency[static]		检测硬件并发特性。
	*/

	thread threads[5];
	cout << "Spawning 5 threads... " << endl;
	for (int i = 0; i < 5; i++)
	{
		threads[i] = thread(thread_task2, i + 1);
	}
	cout << "Done spawning threads! Now wait for them to join" << endl;
	for (auto& t : threads) {
		t.join();
	}
	cout << "All thread joined." << endl;
}


void attempt_10k_increase() {
	/*
	Mutex lock:
	lock , unlock , try_lock
	try_lock：尝试锁住互斥量。a,没锁则成功 b,有则返回失败并跳过 c,造成死锁当被当前线程锁住。
	*/
	for (int i = 0; i < 10000; i++)
	{
		if (mtx.try_lock()) {
			++counter;
			mtx.unlock();
		}
	}
}

void Sample_4() {
	// 4. Mutex lock 
	thread threads[10];
	for (int i = 0; i < 10; i++)
		threads[i] = thread(attempt_10k_increase);
	for (auto& th : threads)
		th.join();
	cout << counter << " Successfull increase of the counter." << endl;
}


void fireworks() {
	/*
	Timed_mutex lock
	try_lock_for	函数接受一个时间范围，表示在这一段时间范围之内线程如果没有获得锁则被阻塞住
	（与 std::mutex 的 try_lock() 不同，try_lock 如果被调用时没有获得锁则直接返回 false），
	如果在此期间其他线程释放了锁，则该线程可以获得对互斥量的锁，如果超时（即在指定时间内还是没有获得锁），
	则返回 false。
	try_lock_until	函数则接受一个时间点作为参数，在指定时间点未到来之前线程如果没有获得锁则被阻塞住，
	如果在此期间其他线程释放了锁，则该线程可以获得对互斥量的锁，如果超时（即在指定时间内还是没有获得锁），
	则返回 false
	*/
	// Waiting to get a lock: each thread prints "-" every 200ms;
	while (!tmtx.try_lock_for(chrono::milliseconds(200)))
	{
		cout << " - ";
	}
	// Get a lock! - wait for 1s, then this thread prints "*"
	this_thread::sleep_for(chrono::milliseconds(1000));
	cout << " * " << endl;
	tmtx.unlock();
}

void Sample_5() {
	// 5. Timed_Mutex
	thread threads[10];
	for (int i = 0; i < 10; i++)
		threads[i] = thread(fireworks);
	for (auto& th : threads)
		th.join();
}


void print_thread_id_2(int id) {
	mtx.lock();		//先手动锁
	lock_guard<mutex> lck(mtx, adopt_lock);		//再通过模板锁,不加adopt_lock标签则会造成死锁。该标签用于接受已锁互斥量
	cout << "Thread #" << id << endl;
}

// T <class Mutex> class lock_guard;
void print_even(int x) {
	if (x % 2 == 0)
		cout << x << " is even" << endl;
	else
		throw(logic_error("not even"));
}

void print_thread_id(int id) {
	try {
		// Using a local lock_guard to lock mtx guarantees unlocking on destruction exception
		lock_guard<mutex> lck(mtx);		// 自解锁。不需要通过unlock解锁，析构时自动解锁
		print_even(id);
	}
	catch (logic_error) {
		cout << "[exception caught]" << endl;
	}
}

void Sample_6() {
	// 6. Lock_guard
	thread threads[10];
	for (int i = 0; i < 10; i++)
		threads[i] = thread(print_thread_id, i+1); 
	for (auto& th : threads)
		th.join();
}


void print_block(int n, char c) {
	unique_lock<mutex> uck(mtx);
	for (int i = 0; i < n; i++)
	{
		cout << c;
	}
	cout << " " << endl;
}

void Sample_7() {	
	// 7. Uniqe_lock
	thread th1(print_block, 50, '*');
	thread th2(print_block, 50, '$');
	th1.join();
	th2.join();
}


void task_a() {
	// Unique_lock 的两个tag adopt_lock he defer_lock. 都是接管互斥量的所有权，但是不进行加锁操作。
	// 一个只接受已加锁的，一个只接受未加锁的
	lock(foo, bar);								// 
	unique_lock<mutex> lck1(foo, adopt_lock);
	unique_lock<mutex> lck2(bar, adopt_lock);
	cout << "task a" << endl;
}

void task_b() {
	unique_lock<mutex> lck1, lck2;
	lck1 = unique_lock<mutex>(bar, defer_lock);
	lck2 = unique_lock<mutex>(foo, defer_lock);
	lock(lck1, lck2);
	cout << "task b" << endl;
}

void Sample_8() {
	// 8. Uniqe_lock 的 tag
	thread th1(task_a);
	thread th2(task_b);
	th1.join();
	th2.join();
}


// Unique_lock 的 move赋值
void print_fifty(char c) {
	unique_lock<mutex> lck = unique_lock<mutex>(mtx);
	for (int i = 0; i < 50; i++)
	{
		cout << c;
	}
	cout << endl;
}

void Sample_9() {
	// 9. Unique_lock的move赋值（原对象呗unique_lock对象取代）
	thread th1(print_fifty, '*');
	thread th2(print_fifty, '$');
	th1.join();
	th2.join();
}


void print_thread_id_3(int id) {
	unique_lock<mutex>lck(mtx, defer_lock);
	lck.lock();
	cout << "Thread #" << id << endl;
	lck.unlock();
}

void Sample_10() {
	// 10. Unique_lock 的 lock 和 unlock
	thread threads[10];
	for (int i = 0; i < 10; i++)
		threads[i] = thread(print_thread_id_3, i + 1);
	for (auto& t : threads)
		t.join();
}


void print_star() {
	unique_lock<mutex> lck(mtx, defer_lock);
	if (lck.try_lock())
		cout << '0';
	else
		cout << 'X';
	this_thread::sleep_for(chrono::milliseconds(20));
}

void Sample_11() {
	// 11. Unicque_lock 的try_lock()函数
	vector<thread> threads;
	for (int i = 0; i < 500; i++)
		threads.emplace_back(print_star);
	for (auto& t : threads)
		t.join();
}


void fireworks_uniq() {
	// Waiting to get a lock: each thread prints "-" every 200ms;
	unique_lock<timed_mutex>lck(tmtx, defer_lock);
	while (!lck.try_lock_for(chrono::milliseconds(200)))
	{
		cout << " - ";
	}
	// Get a lock! - wait for 1s, then this thread prints "*"
	this_thread::sleep_for(chrono::milliseconds(1000));
	cout << " * " << endl;
}

void Sample_12() {
	// 12. Unique_lock 的 try_lock_for 函数
	thread threads[10];
	for (int i = 0; i < 10; i++)
		threads[i] = thread(fireworks_uniq);
	for (auto& th : threads)
		th.join();
}


void print_count_and_unlock(mutex* p_mtx) {
	cout << "count: " << count_num << endl;
	p_mtx->unlock();
}

void task() {
	unique_lock<mutex>lck(mtx);
	++count_num;
	print_count_and_unlock(lck.release());
}

void Sample_13() {
	// 13. Unique_lock 的 release 函数。 返回所管理的Mutex对象指针，并释放所有权
	vector<thread> threads;
	for (int i = 0; i < 10; i++)
		threads.emplace_back(task);
	for (auto& t : threads)
		t.join();
}


class MyMutex : public mutex {
	int _id;
public:
	MyMutex(int id) : _id(id) {}
	int id() { return _id; }
};

MyMutex _mtx(101);

void print_ids(int id) {
	unique_lock<MyMutex> lck(_mtx);
	cout << "thread #" << id << " locked mutex" << lck.mutex()->id() << endl;
}

void Sample_14() {
	thread threads[10];
	for (int i = 0; i < 10; i++)
		threads[i] = thread(print_ids, i + 1);
	for (auto& th : threads)
		th.join();
}


void print_int(future<int>& fut) {
	int x = fut.get();
	cout << "Value: " << x << endl;
}

void Sample_15() {
	// 15. promise 对象。
	/*
	promise 对象是异步 Provider，它可以在某一时刻设置共享状态的值。
	future 对象可以异步返回共享状态的值，或者在必要的情况下阻塞调用者并等待共享状态标志变为 ready，然后才能获取共享状态的值。
	*/
	promise<int> prom;
	future<int> fut = prom.get_future();
	thread t(print_int, ref(fut));
	prom.set_value(10);				// 设置共享状态值，此处与下城t保持同步
	t.join();
}


void print_global_promise() {
	future<int> fut = prom.get_future();
	int x = fut.get();
	cout << "Value: " << x << endl;
}

void Sample_16() {
	// 16. promise对象的move赋值

	thread th1(print_global_promise);
	prom.set_value(10);
	th1.join();
	prom = promise<int>();	// prom被move赋值为一个新的promise对象
	thread th2(print_global_promise);
	prom.set_value(20);
	th2.join();
}


void get_int(promise<int>& prom) {
	int x;
	cout << "Please enter an integer value: " << endl;
	cin.exceptions(ios::failbit);		// Throw on failbit
	try {
		cin >> x;
		prom.set_value(x);
	}
	catch (exception&) {
		prom.set_exception(current_exception());
	}
}

void print_int_2(future<int>& fut) {
	try {
		int x = fut.get();
		cout << "Value: " << x << endl;
	}
	catch (exception e) {
		cout << "[exception caught: " << e.what() << endl;
	}
}

void Sample_17() {
	// 17. promise 的 set_exception函数
	promise<int> prom;
	future<int> fut = prom.get_future();

	thread th1(get_int, ref(prom));
	thread th2(print_int_2, ref(fut));

	th1.join();
	th2.join();
}


int countdown(int from, int to) {
	for (int i = from; i != to; --i)
	{
		cout << i << endl;
		this_thread::sleep_for(chrono::seconds(1));
	}
	cout << "Finished!" << endl;
	return from - to;
}

void Sample_18() {
	// 18. package_task对象
	/*
	std::packaged_task 对象是异步 Provider，它在某一时刻通过调用被包装的任务来设置共享状态的值。
	*/
	packaged_task<int(int, int)> task(countdown);
	future<int> ret = task.get_future();
	thread th(move(task), 10, 0);
	int value = ret.get();
	cout << "The countdown lasted for " << value << " seconds" << endl;
	th.join();
}


void Sample_19() {
	// 19. packaged_task的构造函数
	packaged_task<int(int)> foo;		// 默认
	packaged_task<int(int)> bar([](int x) {return x * 2; });  // lambda表达式初始化
	foo = move(bar);	// move 赋值

	future<int> ret = foo.get_future();	//设置共享状态关联

	thread(move(foo), 10).detach(); //产生线程，调用被包装的任务

	int value = ret.get();
	cout << "The double of 10 is " << value << endl;
}


future<int> launcher(packaged_task<int(int)>& tsk, int arg) {
	if (tsk.valid())
	{
		future<int> ret = tsk.get_future();
		thread(move(tsk), arg).detach();
		return ret;
	}
	else
		return future<int>();
}
void Sample_20() {
	// pagckaged_task 的 valid函数 验证有效性
	packaged_task<int(int)> tsk([](int x) {return x * 2; });
	future<int> fut = launcher(tsk, 25);
	cout << "The double of 25 is " << fut.get() << endl;
}


int triple(int x) { return x * 3; }

void Sample_21() {
	// packaged_task 的 reset函数。重置打包函数传入的参数
	packaged_task<int(int)> tsk(triple);
	future<int> fut = tsk.get_future();
	tsk(100);
	cout << "The tripe of 100 is " << fut.get() << endl;

	tsk.reset();
	fut = tsk.get_future();
	thread(move(tsk), 200).detach();
	cout << "The tripe of 200 is " << fut.get() << endl;
}


bool is_prime(int x) {
	for (int i = 2; i < x; i++)
	{
		if (x % i == 0)
		{
			return false;
		}
	}
	return true;
}

void Sample_22() {
	// asyn 异步线程  +  future对象的wait_for函数
	future<bool> fut = async(is_prime, 17);

	cout << "Cheacking, Please wait" << endl;
	chrono::milliseconds span(100);
	while (fut.wait_for(span) == future_status::timeout)
	{
		cout << ".";
	}
	bool x = fut.get();
	cout << "The nuber " << (x ? "is" : "is not") << " prime." << endl;
}


int do_get_value() { return 10; }

void Sample_23() {
	// shared_fut对象，接受future.shared()的返回，原future则无效 . shared_future可以被多次get。而future对象则不能
	future<int> fut = async(do_get_value);
	shared_future<int> shared_fut = fut.share();
	cout << (fut.valid() ? "有效" : "无效") << endl;

	cout << "Value: " << shared_fut.get() << endl;
	cout << "its double: " << shared_fut.get() * 2 << endl;
}


void Sample_24() {
	// 24. future 的 valid函数，判断有效。只有async(), get_future()初始化的future对象才是有效的。当被move后也无效
	future<int> foo, bar;	// 此时foo，bar都无效
	foo = async(do_get_value);
	bar = move(foo);
	if (foo.valid())
		cout << "foo's value: " << foo.get() << endl;
	else
		cout << "foo is no vald" << endl;

	if(bar.valid())
		cout << "bar's value: " << bar.get() << endl;
	else
		cout << "bar is no vald" << endl;
}


void Sample_25() {
	// future 对象的wait函数
	future<bool> fut = async(is_prime, 194232491);
	cout << "Checking ...." << endl;
	fut.wait();

	if (fut.get())
		cout << "Is prime" << endl;
	else
		cout << "Not prime" << endl;
}


double ThreadTask(int n) {
	cout << this_thread::get_id() << " start computing..." << endl;
	double ret = 0;
	for (int i = 0; i <= n ; i++)
	{
		ret += sin(i);
	}

	cout << this_thread::get_id() << " finished computing..." << endl;
	return ret;
}

void Sample_26() {
	// async 用法
	future<double> f(async(launch::async, ThreadTask, 100000));

#if 1
	while (f.wait_until(chrono::system_clock::now() + chrono::seconds(1)) != future_status::ready)
	{
		cout << "task is runnint ..." << endl;
	}
#else
	while (f.wait_for(chrono::seconds(1)) != future_status::ready)
		cout << "task is running ..." << endl;
#endif

	cout << f.get() << endl;
}

void do_print_ten(char c, int ms) {
	for (int i = 0; i < 10; i++)
	{
		this_thread::sleep_for(chrono::microseconds(ms));
		cout << c << " ";
	}
}

void Sample_27() {
	// 27
	/*
	launch::async		异步任务会在另外一个线程中调用，并通过共享状态返回异步任务的结果（一般是调用 std::future::get() 获取异步任务的结果）。
	launch::deferred	异步任务将会在共享状态被访问时调用，相当与按需调用（即延迟(deferred)调用）。
	*/
	cout << "with launch:: async:" << endl;
	future<void> foo = async(launch::async, do_print_ten, '*', 100);
	future<void> bar = async(launch::async, do_print_ten, '@', 200);
	foo.get();
	bar.get();
	cout << endl << endl;
	cout << "with launch::deferred" << endl;
	foo = async(launch::deferred, do_print_ten, '*', 100);
	bar = async(launch::deferred, do_print_ten, '@', 200);
	foo.get();
	bar.get();
	cout << endl;

}

atomic<bool> ready(false);	//can be checked without being set
atomic_flag winner = ATOMIC_FLAG_INIT;	//Aleays set when checked

void countlm(int id) {
	while (!ready)
	{
		this_thread::yield();
	}	// 等待主线程中设置ready 为true

	for (int i = 0; i < 10000; i++) {
	}	// 计数

	// 某个线程线完成计数过程，输出自己ID、 其他线程执行test_and_set判断，从而不输出自己ID
	if (!winner.test_and_set())
		cout << "thread ID " << id << " win" << endl;
	else
		cout << id << " lose" << endl;
}

void Sample_28() {
	vector<thread> threads;
	cout << "spawning 10 threads that count to 1 million..." << endl;
	for (int i = 0; i < 10; i++)
	{
		threads.push_back(thread(countlm, i + 1));
	}
	ready = true;
	for (auto& th : threads)
		th.join();
}

atomic_flag lock_stream = ATOMIC_FLAG_INIT;
stringstream stream;

void append_numbber(int x) {
	while (lock_stream.test_and_set());
	stream << "thread # " << x << endl;
	lock_stream.clear();
}
void Sample_29() {
	// atomic_flag 的 test_and_set & clear
	/*
		test_and_set() 函数检查 std::atomic_flag 标志，
		如果atomic_flag 之前没有被设置过，则设置atomic_flag 的标志，并返回先前该对象是否被设置过，
		如果之前 std::atomic_flag 对象已被设置，则返回 true，否则返回 false。
		clear() : 用于清楚atomic_flag 的设置，重置为false
	*/
	vector<thread> threads;
	for (int i = 0; i < 10; i++)
	{
		threads.push_back(thread(append_numbber, i + 1));
	}
	for (auto& th : threads)
		th.join();
	cout << stream.str() << endl;
}

atomic_flag lock_flag = ATOMIC_FLAG_INIT;
void f(int n) {
	for (int i = 0; i < 100; i++)
	{
		while (lock_flag.test_and_set(memory_order_acquire));
		cout << "Output from thread " << n << endl;
		lock_flag.clear(memory_order_acquire);
	}
}

void Sample_30() {
	// 30. atomic_flag 的 test_and_set 函数  有问题，程序卡死。。。
	vector<thread> v;
	for (int i = 0; i < 10; i++)
	{
		v.emplace_back(f, i);
	}
	for (auto& t : v)
		t.join();
}


int main(int argc, char* argv)
{
	cout << "Hi, This a thread Sample for C++ 11 " << endl;
	cout << "———————————————————\n\n\n" << endl;

	Sample_29();


	cin.get(); cin.get();
    return EXIT_SUCCESS;
}

