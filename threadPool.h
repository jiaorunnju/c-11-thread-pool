#ifndef THREAD_POOL_
#define THREAD_POOL_

#include "syncQueue.h"
#include <thread>
#include <functional>
#include <memory>
#include <atomic>
#include <future>

using std::list;
using std::atomic;
using std::once_flag;
using std::thread;
using std::unique_ptr;
using std::future;
using std::packaged_task;
using std::function;

const int MaxTaskCount = 10;

template <typename T>
class ThreadPool {
	using Task = packaged_task<T>;
	list<unique_ptr<thread>> m_threadGroup;
	SyncQueue<Task> m_queue;
	atomic<bool> m_running;
	std::once_flag m_flag;

public:
	explicit ThreadPool(int numThreads = thread::hardware_concurrency()) :m_queue(MaxTaskCount) {
		start(numThreads);
	}

	~ThreadPool() {
		stop();
	}

	void addTask(Task&& task) {
		m_queue.add(std::forward<Task>(task));
	}

	void addTask(const Task& task) {
		m_queue.add(task);
	}

	void stop() {
		std::call_once(m_flag, [this] {stopThreadGroup(); });
	}
	
	auto async(function<T> f)-> decltype(declval<Task>().get_future()) {
		Task task(f);
		auto fut = task.get_future();
		this->addTask(std::move(task));
		return std::move(fut);
	}

private:
	void runInThread() {
		while (m_running)
		{
			Task t;
			bool b = m_queue.getOne(t);
			if (b)
				t();
		}
	}

	void start(int numThreads) {
		m_running = true;
		for (int i = 0; i < numThreads; ++i) {
			m_threadGroup.push_back(std::make_unique<thread>(&ThreadPool::runInThread, this));
		}
	}

	void stopThreadGroup() {
		m_queue.stop()
		m_running = false;
		for (auto& t : m_threadGroup) {
			if(t->joinable())
				t->join();
		}
		m_threadGroup.clear();
	}
	
};

#endif // !THREAD_POOL_