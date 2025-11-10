#include <iostream>
#include <chrono>
#include <map>

std::map<int, std::chrono::steady_clock::time_point> timers{};

int __stdcall StartTimer(int id)
{
	timers[id] = std::chrono::high_resolution_clock::now();
	return 0;
}


unsigned int __stdcall StopTimer(int id)
{
	std::chrono::microseconds retval = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - timers[id]);
	return (unsigned int)retval.count();
}