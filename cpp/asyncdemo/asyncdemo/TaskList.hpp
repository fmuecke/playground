#pragma once
#include <queue>
#include <future>
#include <thread>
#include <chrono>
#include <windows.h>
#include <algorithm>

template <typename T>
class TaskList
{
	static unsigned GetNumCores()
	{
		SYSTEM_INFO sysinfo;
		::GetSystemInfo(&sysinfo);
		return sysinfo.dwNumberOfProcessors;
	}

public:
	explicit TaskList(unsigned maxTasks) : _maxTasks{ maxTasks }
	{}

	TaskList() : TaskList(GetNumCores())
	{}

	~TaskList() {}

	bool IsEmpty() const { return _tasks.empty(); }

	bool HasResults() const
	{
		return std::any_of(cbegin(_tasks), cend(_tasks), [](std::future<T> const& f) { return f.valid(); });
	}

	T PopTask()
	{
		if (IsEmpty())
		{
			return T();
		}

		auto pos = GetFirstValidTask();
		while (pos == cend(_tasks))
		{
			this_thread::sleep_for(chrono::milliseconds{ 50 });
			pos = GetFirstValidTask();
		}

		auto result = pos->get();
		_tasks.pop();
		return result;
	}

private:
	inline auto GetFirstValidTask() //-> decltype(begin(_tasks))
	{
		return std::find_if(cbegin(_tasks), cend(_tasks), [](std::future<T> const& f) { return f.valid(); });
	}

	std::queue<std::future<T>> _tasks;
	unsigned _maxTasks = 0;
};

