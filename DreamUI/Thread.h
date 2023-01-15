#include "stdafx.h"
#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

#include <process.h>
typedef unsigned int(__stdcall* ThreadProcType)(void* arg);

class Thread {
public:
	static void* Create(ThreadProcType proc, void* arg = NULL, bool start = false) {
		void* rv = (void*)_beginthreadex(NULL, NULL, proc, arg, CREATE_SUSPENDED, NULL);
		if (start)
			Thread::Resume(rv);
		return rv;
	}

	static bool Resume(void* handle) {
		return ResumeThread(handle) != -1;
	}

	Thread(ThreadProcType proc, void* arg = NULL) {
		this->proc_ = proc;
		this->handle_ = (void*)_beginthreadex(NULL, NULL, proc, arg, CREATE_SUSPENDED, NULL);
	}

	void start() {
		this->resume();
	}

	void resume() {
		ResumeThread(this->handle_);
	}

	void suspend() {
		SuspendThread(this->handle_);
	}

	void terminate() {
		TerminateThread(this->handle_, 0xFFFFFFFF);
	}

	~Thread() {
		CloseHandle(this->handle_);
	}
private:
	ThreadProcType proc_;
	void* handle_;
};

#endif