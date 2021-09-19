#ifndef TIMER_H_
#define TIMER_H_

#include <cstdint>
#include "CustomDataObject.h"
typedef void (*TickCallback)(uint32_t time);

class Timer;
typedef void (*TimerCallback)(Timer* timer);

enum TimeType
{
	TimeGame,
	TimeLocal
};

class Timer : public CustomDataObject {
public:
	Timer(uint32_t timeout, TimerCallback callback, bool periodic = false, TimeType timetype = TimeGame, bool auto_dispose = true);

	virtual ~Timer();

	uint32_t timeout() const {return this->timeout_;}
	void setTimeout (uint32_t timeout) {this->timeout_ = timeout;}
	void setTimeout (double timeout) {this->timeout_ = static_cast<uint32_t>(timeout * 1000.0);}

	bool periodic() const {return this->periodic_;}
	bool paused() const {return this->paused_;}
	bool disposed() const {return this->disposed_;}

	uint32_t elapsed() const;

	uint32_t remaining() const;

	uint32_t execCount() const;

	void reset();

	uint32_t update(uint32_t time);

	void start();

	void startImmediate(); //会先执行一次回调，其余同start

	void pause();

	void resume();

	void destroy();
private:
	uint32_t timeout_;
	uint32_t time_remaining_;
	uint32_t last_update_time_;
	uint32_t exec_count;
	bool paused_;
	bool periodic_;
	bool disposed_;
	bool auto_dispose_;
	bool added_;
	TimerCallback callback_;
	TimeType timetype;
};

void Timer_Init();
void Timer_Update(uint32_t time);
void Timer_Cleanup();
void Timer_AddTickCallback(TickCallback cb);
Timer* GetTimer(double timeout, TimerCallback callback, bool periodic = false, TimeType timetype = TimeGame, bool auto_dispose = true);
Timer* GetCurrentTimer();

#endif