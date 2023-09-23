#pragma once
#include <queue>
#include <mutex>
#include <thread>

template<typename T>
struct TSQueue {
public:
	size_t Size() const {
		return data.size();
	}

	T PopFront() {
		std::lock_guard<std::mutex> lock(mutex);
		T result = data.front();
		data.pop();
		return result;
	}
	
	void PushBack(T newValue) {
		std::lock_guard<std::mutex> lock(mutex);
		data.push(newValue);
	}

	bool IsEmpty() {
		return data.empty();
	}

	void Clear() {
		std::lock_guard<std::mutex> lock(mutex);
		data.clear();
	}
private:
	std::mutex mutex;
	std::queue<T> data;
};