#include <future>
#include <iostream>
#include <chrono>
#include <thread>

#include "TaskList.hpp"

using namespace std;

auto startTime = chrono::system_clock::now();

void f()
{
	this_thread::sleep_for(chrono::milliseconds{ 500 });
	cout << chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - startTime).count();
}

void g()
{
	auto fx = async(f);
}

int main()
{
	TaskList<void> t;

	g();

}