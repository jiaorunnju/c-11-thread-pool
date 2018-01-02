#ifndef SYNC_QUEUE_
#define SYNC_QUEUE_

#include <list>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <atomic>

using std::mutex;
using std::list;
using std::condition_variable;
using std::cout;
using std::endl;
using std::unique_lock;
using std::atomic;

template <typename T>
class SyncQueue {
	list<T> m_queue;
	mutex m_mutex;
	condition_variable m_notEmpty;
	condition_variable m_notFull;
	int m_maxSize;
	atomic<bool> m_stop;

public:
	explicit SyncQueue(int maxsize) :m_maxSize(maxsize),m_stop(false) {}

	~SyncQueue() {
		stop();
	}

	bool getOne(T& t) {
		unique_lock<mutex> locker(m_mutex);
		//????? why when I use wait here, it does not work, just wait.
		while (!isStop() && empty()) {
			m_notEmpty.wait_for(locker,std::chrono::milliseconds(500));
		}
		//m_notEmpty.wait(locker, [this] {return isStop() || !empty(); });
		if (isStop()) {
			return false;
		}
		t = std::move(m_queue.front());
		//cout << "removing " << t << endl;
		m_queue.pop_front();
		m_notFull.notify_one();
		return true;
	}

	void stop() {
		m_stop = true;
	}

	size_t size() {
		//std::lock_guard<mutex> locker(m_mutex);
		return m_queue.size();
	}

	template <typename F>
	void add(F&& f) {
		unique_lock<mutex> locker(m_mutex);
		while (!isStop() && full()) {
			m_notFull.wait(locker);
		}
		//m_notFull.wait(locker, [this] {return isStop() || !full(); });
		if (isStop()) {
			return;
		}
		m_queue.push_back(std::forward<F>(f));
		m_notEmpty.notify_one();
	}

	bool isStop() {
		return m_stop;
	}

private:
	bool empty() {
		return m_queue.empty();
	}

	bool full() {
		return m_queue.size() == m_maxSize;
	}

};

#endif // !SYNC_QUEUE_
