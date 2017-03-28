// threadSample.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include <stdio.h>
#include <stdlib.h>

#include <iostream>		//input��output stream
#include <thread>		// thread��   this_thread::sleep_for
#include <chrono>		// chrono::seconds
#include <functional>
#include <atomic>
#include <mutex>
#include <vector>
#include <future>		// promise,future

#include <cmath>		// math��   sin
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
	get_id 		��ȡ�߳� ID��

	joinable	����߳��Ƿ�ɱ� join��

	join		Join �̡߳�

	detach		Detach �߳�

	swap		Swap �߳� ��

	native_handle	���� native handle��

	hardware_concurrency[static]		���Ӳ���������ԡ�
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
	try_lock��������ס��������a,û����ɹ� b,���򷵻�ʧ�ܲ����� c,�������������ǰ�߳���ס��
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
	try_lock_for	��������һ��ʱ�䷶Χ����ʾ����һ��ʱ�䷶Χ֮���߳����û�л����������ס
	���� std::mutex �� try_lock() ��ͬ��try_lock ���������ʱû�л������ֱ�ӷ��� false����
	����ڴ��ڼ������߳��ͷ�����������߳̿��Ի�öԻ����������������ʱ������ָ��ʱ���ڻ���û�л��������
	�򷵻� false��
	try_lock_until	���������һ��ʱ�����Ϊ��������ָ��ʱ���δ����֮ǰ�߳����û�л����������ס��
	����ڴ��ڼ������߳��ͷ�����������߳̿��Ի�öԻ����������������ʱ������ָ��ʱ���ڻ���û�л��������
	�򷵻� false
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
	mtx.lock();		//���ֶ���
	lock_guard<mutex> lck(mtx, adopt_lock);		//��ͨ��ģ����,����adopt_lock��ǩ�������������ñ�ǩ���ڽ�������������
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
		lock_guard<mutex> lck(mtx);		// �Խ���������Ҫͨ��unlock����������ʱ�Զ�����
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
	// Unique_lock ������tag adopt_lock he defer_lock. ���ǽӹܻ�����������Ȩ�����ǲ����м���������
	// һ��ֻ�����Ѽ����ģ�һ��ֻ����δ������
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
	// 8. Uniqe_lock �� tag
	thread th1(task_a);
	thread th2(task_b);
	th1.join();
	th2.join();
}


// Unique_lock �� move��ֵ
void print_fifty(char c) {
	unique_lock<mutex> lck = unique_lock<mutex>(mtx);
	for (int i = 0; i < 50; i++)
	{
		cout << c;
	}
	cout << endl;
}

void Sample_9() {
	// 9. Unique_lock��move��ֵ��ԭ������unique_lock����ȡ����
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
	// 10. Unique_lock �� lock �� unlock
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
	// 11. Unicque_lock ��try_lock()����
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
	// 12. Unique_lock �� try_lock_for ����
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
	// 13. Unique_lock �� release ������ �����������Mutex����ָ�룬���ͷ�����Ȩ
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
	// 15. promise ����
	/*
	promise �������첽 Provider����������ĳһʱ�����ù���״̬��ֵ��
	future ��������첽���ع���״̬��ֵ�������ڱ�Ҫ����������������߲��ȴ�����״̬��־��Ϊ ready��Ȼ����ܻ�ȡ����״̬��ֵ��
	*/
	promise<int> prom;
	future<int> fut = prom.get_future();
	thread t(print_int, ref(fut));
	prom.set_value(10);				// ���ù���״ֵ̬���˴����³�t����ͬ��
	t.join();
}


void print_global_promise() {
	future<int> fut = prom.get_future();
	int x = fut.get();
	cout << "Value: " << x << endl;
}

void Sample_16() {
	// 16. promise�����move��ֵ

	thread th1(print_global_promise);
	prom.set_value(10);
	th1.join();
	prom = promise<int>();	// prom��move��ֵΪһ���µ�promise����
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
	// 17. promise �� set_exception����
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
	// 18. package_task����
	/*
	std::packaged_task �������첽 Provider������ĳһʱ��ͨ�����ñ���װ�����������ù���״̬��ֵ��
	*/
	packaged_task<int(int, int)> task(countdown);
	future<int> ret = task.get_future();
	thread th(move(task), 10, 0);
	int value = ret.get();
	cout << "The countdown lasted for " << value << " seconds" << endl;
	th.join();
}


void Sample_19() {
	// 19. packaged_task�Ĺ��캯��
	packaged_task<int(int)> foo;		// Ĭ��
	packaged_task<int(int)> bar([](int x) {return x * 2; });  // lambda���ʽ��ʼ��
	foo = move(bar);	// move ��ֵ

	future<int> ret = foo.get_future();	//���ù���״̬����

	thread(move(foo), 10).detach(); //�����̣߳����ñ���װ������

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
	// pagckaged_task �� valid���� ��֤��Ч��
	packaged_task<int(int)> tsk([](int x) {return x * 2; });
	future<int> fut = launcher(tsk, 25);
	cout << "The double of 25 is " << fut.get() << endl;
}


int triple(int x) { return x * 3; }

void Sample_21() {
	// packaged_task �� reset���������ô����������Ĳ���
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
	// asyn �첽�߳�  +  future�����wait_for����
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
	// shared_fut���󣬽���future.shared()�ķ��أ�ԭfuture����Ч . shared_future���Ա����get����future��������
	future<int> fut = async(do_get_value);
	shared_future<int> shared_fut = fut.share();
	cout << (fut.valid() ? "��Ч" : "��Ч") << endl;

	cout << "Value: " << shared_fut.get() << endl;
	cout << "its double: " << shared_fut.get() * 2 << endl;
}


void Sample_24() {
	// 24. future �� valid�������ж���Ч��ֻ��async(), get_future()��ʼ����future���������Ч�ġ�����move��Ҳ��Ч
	future<int> foo, bar;	// ��ʱfoo��bar����Ч
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
	// future �����wait����
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
	// async �÷�
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
	launch::async		�첽�����������һ���߳��е��ã���ͨ������״̬�����첽����Ľ����һ���ǵ��� std::future::get() ��ȡ�첽����Ľ������
	launch::deferred	�첽���񽫻��ڹ���״̬������ʱ���ã��൱�밴����ã����ӳ�(deferred)���ã���
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
	}	// �ȴ����߳�������ready Ϊtrue

	for (int i = 0; i < 10000; i++) {
	}	// ����

	// ĳ���߳�����ɼ������̣�����Լ�ID�� �����߳�ִ��test_and_set�жϣ��Ӷ�������Լ�ID
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
	// atomic_flag �� test_and_set & clear
	/*
		test_and_set() ������� std::atomic_flag ��־��
		���atomic_flag ֮ǰû�б����ù���������atomic_flag �ı�־����������ǰ�ö����Ƿ����ù���
		���֮ǰ std::atomic_flag �����ѱ����ã��򷵻� true�����򷵻� false��
		clear() : �������atomic_flag �����ã�����Ϊfalse
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
	// 30. atomic_flag �� test_and_set ����  �����⣬������������
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
	cout << "��������������������������������������\n\n\n" << endl;

	Sample_29();


	cin.get(); cin.get();
    return EXIT_SUCCESS;
}

