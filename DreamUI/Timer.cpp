#include "stdafx.h"
#include "Timer.h"
#include "Game.h"
#include "GameTime.h"
#include <deque>
#include "Tools.h"

static const unsigned int MAX_TICK_CALLBACK = 100;
const int TASK_TYPE_ADD = 1;

struct UpdateMissionTaskData {
	int type;
	Timer* timer;
};

typedef std::deque<UpdateMissionTaskData> UpdateMissionTaskDataQueueType;
typedef std::deque<Timer*> TimerQueueType;

static unsigned int TickCallbackArraySize = 0;
static TickCallback TickCallbackArray[MAX_TICK_CALLBACK];
static UpdateMissionTaskDataQueueType TaskQueue;
static TimerQueueType TimerQueue;
static TimerQueueType SystemTimerQueue;
static Timer* CurrentTimer = NULL;

Timer* GetTimer(double timeout, TimerCallback callback, bool periodic /*= false*/, TimeType timetype /*= TimeGame*/, bool auto_dispose /*= true*/)
{
	return new Timer(static_cast<unsigned int>(timeout * 1000.0), callback, periodic, timetype, auto_dispose);
}



Timer* GetCurrentTimer() {
	return CurrentTimer;
}

void Timer_Init() {
	TickCallbackArraySize = 0;
}

void Add(Timer* tm) {
	UpdateMissionTaskData data = { TASK_TYPE_ADD, tm };
	TaskQueue.push_back(data);
}

void DoAddTimer(Timer* timer) {
	TimerQueue.push_back(timer);
}

void DoUpdateTasks() {
	while (TaskQueue.size()) {
		UpdateMissionTaskData* data = &(TaskQueue.front());
		switch (data->type) {
		case TASK_TYPE_ADD:
			DoAddTimer(data->timer);
			break;
		}
		TaskQueue.pop_front();
	}
}

void Timer_Cleanup() {
	TimerQueueType::iterator iter;
	for (iter = TimerQueue.begin(); iter != TimerQueue.end(); iter++)
		delete (*iter);
	TimerQueue.clear();
	TaskQueue.clear();
}

//TODO ����list������deque��iterator��Ч��������
void Timer_Update(unsigned int time) {
	if (!IsInGame())
		return;

	for (unsigned int i = 0; i < TickCallbackArraySize; ++i)
		TickCallbackArray[i](time);

	DoUpdateTasks();

	if (TimerQueue.size()) {
		Timer* timer;
		TimerQueueType::iterator i;
		for (i = TimerQueue.begin(); i != TimerQueue.end(); ) {
			timer = *i;
			if (timer->disposed()) {
				delete timer;
				i = TimerQueue.erase(i);
			}
			else {
				if (timer->update(time) == 0) {
					if (timer->disposed()) {
						delete timer;
						i = TimerQueue.erase(i);
						return;
					}
				}
				i++;
			}
		}
	}
}

void Timer_AddTickCallback(TickCallback cb) {
	TickCallbackArray[TickCallbackArraySize++] = cb;
}

Timer::Timer(unsigned int timeout, TimerCallback callback, bool periodic, TimeType timetype, bool auto_dispose) {
	this->timeout_ = timeout;
	this->periodic_ = periodic;
	this->disposed_ = false;
	this->last_update_time_ = 0;
	this->time_remaining_ = timeout;
	this->paused_ = true;
	this->callback_ = callback;
	this->auto_dispose_ = auto_dispose;
	this->added_ = false;
	this->timetype = timetype;
	this->exec_count = 0;
}

Timer::~Timer() {

}

unsigned int Timer::elapsed() const {
	return this->timeout_ - this->time_remaining_;
}

unsigned int Timer::remaining() const {
	return this->time_remaining_;
}

void Timer::reset() {
	this->start();
}

unsigned int Timer::update(unsigned int time) {
	if (this->paused_) {
		this->last_update_time_ = time;
	}
	else {
		unsigned int diff = (TimeRaw() - this->last_update_time_);
		this->time_remaining_ = (diff > this->time_remaining_) ? 0 : this->time_remaining_ - diff;
		this->last_update_time_ = time;
		if (this->time_remaining_ == 0) {
			this->paused_ = true;
			this->time_remaining_ = 0;

			//�ص��п����޸������Timer!
			this->callback_(this);

			if (!this->disposed_) {
				if (!this->periodic_) {
					if (this->auto_dispose_)
						this->disposed_ = true;
				}
				else
				{
					this->exec_count += 1;
					this->start();
				}
			}
		}
	}
	return this->time_remaining_;
}

void Timer::start() {
	if (this->disposed_)
		return;

	this->time_remaining_ = this->timeout_;
	this->last_update_time_ = TimeRaw();
	this->paused_ = false;

	if (!this->added_) {
		Add(this);
		this->added_ = true;
	}
}

void Timer::startImmediate() {
	if (callback_) callback_(this);
	start();
}

void Timer::pause() {
	this->paused_ = true;
}

void Timer::resume() {
	this->paused_ = false;
}

void Timer::destroy() {
	this->disposed_ = true;
}

unsigned int Timer::execCount() const
{
	return exec_count;
}
