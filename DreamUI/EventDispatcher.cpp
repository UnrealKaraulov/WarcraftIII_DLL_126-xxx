#include "stdafx.h"
#include "Event.h"
#include "EventDispatcher.h"

static const int TYPE_ADD = 1;
static bool CurrentEventDiscarded;

void DiscardCurrentEvent() {
	CurrentEventDiscarded = true;
}

EventDispatcher::EventDispatcher() : subscriber_count_(0) { }

EventDispatcher::~EventDispatcher() { }

unsigned int DispatchedLevel = 0;
const unsigned int MAX_DISPATCH_RECURSIVE_LEVEL = 15;
bool EventDispatcher::dispatch(const Event* evt) {
	if (DispatchedLevel > 0) {
		if (DispatchedLevel >= MAX_DISPATCH_RECURSIVE_LEVEL) {
			DispatchedLevel = 0;
#ifndef _VMP
			;//OutputDebug("[WARNING] recursive dispatch limit reached! Event: 0x%X", evt->id());
#endif
			return true;
		}
#ifndef _VMP
		;//OutputDebug("[WARNING] recursive dispatch! Event: 0x%X", evt->id());
#endif
	}

	CurrentEventDiscarded = false;
	this->current_event_ = evt;
	EventListenerType subscriber;

	for (unsigned int i = 0; i < this->subscriber_count_; ++i) {
		if (subscriber = this->subscribers_[i]) subscriber(evt);
	}

	this->performTasks();

	int evtid = evt->id();
	if (this->map_.count(evtid)) {
		EventListenerQueueType& queue = this->map_[evtid];
		DispatchedLevel++;
		for (EventListenerQueueType::iterator iter = queue.begin(); !CurrentEventDiscarded && iter != queue.end(); ++iter) {
			(*iter)(evt);
		}
		DispatchedLevel--;
	}
	return !CurrentEventDiscarded;
}

void EventDispatcher::listen(unsigned int event_id, EventListenerType callback) {
	ListenerUpdateTask task = { TYPE_ADD, event_id, callback };

	for (auto & s : this->tasks_)
	{
		if (s.callback == task.callback
			&& s.event_id == task.event_id
			&& s.type == task.type
			)
			return;
	}

	this->tasks_.push_back(task);
}

bool EventDispatcher::hasListener(unsigned int event_id) {
	return this->map_.count(event_id) > 0 && this->map_[event_id].size() > 0;
}

const Event* EventDispatcher::currentEvent() {
	return this->current_event_;
}

void EventDispatcher::performTasks() {
	if (this->tasks_.size()) {
		for (UpdateTaskQueueType::iterator iter = this->tasks_.begin(); iter != this->tasks_.end(); ++iter) {
			ListenerUpdateTask* q = &(*iter);
			switch (q->type) {
			case TYPE_ADD:
				this->map_[q->event_id].push_back(q->callback);
				break;
			}
		}
		this->tasks_.clear();
	}
}

void EventDispatcher::subscribe(EventListenerType callback) {
	if (this->subscriber_count_ < EVENTDISPATCHER_MAX_SUBSCRIBER)
		this->subscribers_[this->subscriber_count_++] = callback;
#ifndef _VMP
	//else
	//	;//OutputDebug("[WARNING] unable to add subscriber.(overflow)");
#endif
}