#include "stdafx.h"
#ifndef EVENT_DISPATCHER_H_INCLUDED_
#define EVENT_DISPATCHER_H_INCLUDED_

#include <map>
#include <deque>

class Event;
struct ListenerUpdateTask;

typedef void (*EventListenerType)(const Event*);
typedef std::deque<EventListenerType> EventListenerQueueType;
typedef std::map<unsigned int, EventListenerQueueType> EventDispatchMapType;
typedef std::deque<ListenerUpdateTask> UpdateTaskQueueType;

struct ListenerUpdateTask {
	int type;
	unsigned int event_id;
	EventListenerType callback;
};

static const unsigned int EVENTDISPATCHER_MAX_SUBSCRIBER = 1000;

class EventDispatcher {
public:
	EventDispatcher();
	~EventDispatcher();

	void listen(unsigned int event_id, EventListenerType callback);
	void subscribe(EventListenerType callback);
	bool hasListener(unsigned int event_id);
	const Event* currentEvent();
	bool dispatch(const Event* evt);
private:
	static const unsigned int MAX_SUBSCRIBE;
	unsigned int subscriber_count_;
	EventListenerType subscribers_[EVENTDISPATCHER_MAX_SUBSCRIBER];

	void performTasks();
	const Event* current_event_;
	EventDispatchMapType map_;
	UpdateTaskQueueType tasks_;

	DISALLOW_COPY_AND_ASSIGN(EventDispatcher);
};

void DiscardCurrentEvent();

#endif