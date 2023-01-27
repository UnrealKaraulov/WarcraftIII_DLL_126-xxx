#include "stdafx.h"
#ifndef OBSERVER_H_INCLUDED
#define OBSERVER_H_INCLUDED
#include "War3ClassWrapper.h"
#include "CustomDataObject.h"
#include "BaseStructs.h"
#include "GameStructs.h"

#define OBSERVER_EVENT_HANDLER(name) \
	unsigned int __fastcall name(war3::CObserver* OBSERVER, unsigned int __DUMMY, war3::CEvent* EVENT)

namespace CObserver {
	typedef unsigned int(__fastcall* EventProcessFunctionType)(war3::CObserver* ob, unsigned int dummy, war3::CEvent* evt);
	war3::CObserver* create(EventProcessFunctionType func = NULL, void* userData = NULL);

	template <typename SenderType>
	unsigned int addEventObserver(war3::CObserver* t, unsigned int evtId, SenderType* ob) {
		return addEventObserver(t, evtId, reinterpret_cast<void*>(ob));
	}

	void destroy(war3::CObserver* t);

	void setEventProcessFunction(war3::CObserver* t, EventProcessFunctionType func);
	unsigned int addEventObserver(war3::CObserver* t, unsigned int evtId, war3::CObserver* ob);
	unsigned int dispatchEvent(war3::CObserver* t, war3::CEvent* evt);

	void** getOriginalVtable();
}

class Observer : public War3ClassWrapper, public CustomDataObject {
public:
	typedef void (*HandlerType)(Observer* t, unsigned int evtId);
	static Observer* Create(HandlerType handler = NULL);
	static void Destroy(Observer* t);

	Observer(war3::CObserver* base = NULL);
	void addEventObserver(Observer* child, unsigned int evtId);
	void setHandler(HandlerType handler) { this->handler_ = handler; }
	HandlerType handler() { return this->handler_; }
private:
	HandlerType handler_;
};

#endif