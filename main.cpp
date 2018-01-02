//#include "mylib.h"
#include "syncQueue.h"
#include "threadPool.h"
#include <iostream>
using namespace std;

int main() {
	auto task1 = packaged_task<int()>([] {std::this_thread::sleep_for(std::chrono::seconds(1)); return 1; });
	auto task2 = packaged_task<int()>([] {std::this_thread::sleep_for(std::chrono::seconds(1)); return 2; });
	auto task3 = packaged_task<int()>([] {std::this_thread::sleep_for(std::chrono::seconds(1)); return 3; });
	auto task4 = packaged_task<int()>([] {std::this_thread::sleep_for(std::chrono::seconds(1)); return 4; });
	auto task5 = packaged_task<int()>([] {std::this_thread::sleep_for(std::chrono::seconds(1)); return 5; });
	auto task6 = packaged_task<int()>([] {std::this_thread::sleep_for(std::chrono::seconds(1)); return 6; });
	auto f1 = task1.get_future();
	auto f2 = task2.get_future();
	auto f3 = task3.get_future();
	auto f4 = task4.get_future();
	auto f5 = task5.get_future();
	auto f6 = task6.get_future();
	ThreadPool<int()> pool(3);
	pool.addTask(std::move(task1));
	pool.addTask(std::move(task2));
	pool.addTask(std::move(task3));
	pool.addTask(std::move(task4));
	pool.addTask(std::move(task5));
	pool.addTask(std::move(task6));
	cout << f1.get() << endl;
	cout << f2.get() << endl;
	cout << f3.get() << endl;
	cout << f4.get() << endl;
	cout << f5.get() << endl;
	cout << f6.get() << endl;
	system("pause");

	/*
	int i = 3;
	SyncQueue<int> q(1);
	q.add(3);
	cout << q.getOne(i) << endl;
	q.stop();
	cout << q.getOne(i) << endl;
	system("pause");
	*/
}