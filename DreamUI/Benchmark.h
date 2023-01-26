#include "stdafx.h"
#ifndef BENCHMARK_H_
#define BENCHMARK_H_

#include <cstdint>
#include <map>

class Benchmark {
public:
	Benchmark() {
		QueryPerformanceFrequency(&(this->count_frequency_));
		QueryPerformanceCounter(&(this->li_));
		this->base_ = this->li_.QuadPart;
	}

	void mark(const char* name) {
		QueryPerformanceCounter(&(this->li_));
		double v = (static_cast<double>(this->li_.QuadPart - base_) / static_cast<double>(this->count_frequency_.QuadPart));
		this->map_[name] = v;
	}

	double elapsedTime(const char* name1, const char* name2) {
		if (this->map_.count(name1) && this->map_.count(name2)) {
			double v1 = this->map_[name1];
			double v2 = this->map_[name2];
			return (v2 - v1) * 1000.0f;
		}
		return 0;
	}
private:
	std::map<const char*, double> map_;
	LONGLONG base_;
	LARGE_INTEGER li_;
	LARGE_INTEGER count_frequency_;

	DISALLOW_COPY_AND_ASSIGN(Benchmark);
};

#define BENCHMARK(tag, code) \
	Benchmark benchmark_##tag;\
	benchmark_##tag.mark("begin");\
	code\
	benchmark_##tag.mark("end");\
	printf("[%s] %.3fms\n", #tag, benchmark_##tag.elapsedTime("begin", "end"));

#define BENCHMARK_BEGIN(tag) \
	Benchmark benchmark_##tag;\
	benchmark_##tag.mark("begin");

#define BENCHMARK_END(tag) \
	benchmark_##tag.mark("end");\
	printf("[%s] %.3fms\n", #tag, benchmark_##tag.elapsedTime("begin", "end"));
#endif