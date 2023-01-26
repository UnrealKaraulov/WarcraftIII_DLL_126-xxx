#include "stdafx.h"
#ifndef EVENT_H_INCLUDED_
#define EVENT_H_INCLUDED_

#include "EventData.h"
#include "KeyCode.h"
#include "CustomDataObject.h"
#include "EventDispatcher.h"

#define NULL 0

class Event : public CustomDataObject {
public:
	Event() {
		this->id_ = 0;
	}

	Event(int id) {
		this->id_ = id;
	}

	virtual ~Event() {

	}

	unsigned int id() const { return this->id_; }
	void setId(unsigned int id) { this->id_ = id; }
	void discard() {
		DiscardCurrentEvent();
	}
private:
	unsigned int id_;

	DISALLOW_COPY_AND_ASSIGN(Event);
};

#endif