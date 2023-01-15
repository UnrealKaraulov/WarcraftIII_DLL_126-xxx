#ifndef TIMER_H_
#define TIMER_H_

#include <cstdint>
#include "CustomDataObject.h"
typedef void (*TickCallback)(unsigned int time);

class Timer;
typedef void (*TimerCallback)(Timer* timer);

enum TimeType
{
	TimeGame,
	TimeLocal
};

class Timer : public CustomDataObject {
public:
	Timer(unsigned int timeout, TimerCallback callback, bool periodic = false, TimeType timetype = TimeGame, bool auto_dispose = true);

	virtual ~Timer();

	unsigned int timeout() const { return this->timeout_; }
	void setTimeout(unsigned int timeout) { this->timeout_ = timeout; }
	void setTimeout(double timeout) { this->timeout_ = static_cast<unsigned int>(timeout * 1000.0); }

	bool periodic() const { return this->periodic_; }
	bool paused() const { return this->paused_; }
	bool disposed() const { return this->disposed_; }

	unsigned int elapsed() const;

	unsigned int remaining() const;

	unsigned int execCount() const;

	void reset();

	unsigned int update(unsigned int time);

	void start();

	void startImmediate(); //会先执行一次回调，其余同start

	void pause();

	void resume();

	void destroy();
private:
	unsigned int timeout_;
	unsigned int time_remaining_;
	unsigned int last_update_time_;
	unsigned int exec_count;
	bool paused_;
	bool periodic_;
	bool disposed_;
	bool auto_dispose_;
	bool added_;
	TimerCallback callback_;
	TimeType timetype;
};

void Timer_Init();
void Timer_Update(unsigned int time);
void Timer_Cleanup();
void Timer_AddTickCallback(TickCallback cb);
Timer* GetTimer(double timeout, TimerCallback callback, bool periodic = false, TimeType timetype = TimeGame, bool auto_dispose = true);
Timer* GetCurrentTimer();

#endif