#include "stdafx.h"
#ifndef OBSERVER_H_INCLUDED
#define OBSERVER_H_INCLUDED
#include "War3ClassWrapper.h"
#include "CustomDataObject.h"
#include "BaseStructs.h"
#include "GameStructs.h"

#define OBSERVER_EVENT_HANDLER(name) \
	uint32_t __fastcall name(war3::CObserver* OBSERVER, uint32_t __DUMMY, war3::CEvent* EVENT)

namespace CObserver { 
	typedef uint32_t (__fastcall *EventProcessFunctionType)(war3::CObserver* ob, uint32_t dummy, war3::CEvent* evt);
	war3::CObserver* create(EventProcessFunctionType func = NULL, void* userData = NULL);

	template <typename SenderType>
	uint32_t addEventObserver(war3::CObserver* t, uint32_t evtId, SenderType* ob) {
		return addEventObserver(t, evtId, reinterpret_cast<void*>(ob));
	}

	void destory(war3::CObserver* t);

	void setEventProcessFunction(war3::CObserver* t, EventProcessFunctionType func);
	uint32_t addEventObserver(war3::CObserver* t, uint32_t evtId, war3::CObserver* ob);
	uint32_t dispatchEvent(war3::CObserver* t, war3::CEvent* evt);

	void** getOriginalVtable();
}

class Observer : public War3ClassWrapper, public CustomDataObject {
public:
	typedef void (*HandlerType)(Observer* t, uint32_t evtId);
	static Observer* Create(HandlerType handler = NULL);
	static void Destroy(Observer* t);

	Observer(war3::CObserver* base = NULL);
	void addEventObserver(Observer* child, uint32_t evtId);
	void setHandler(HandlerType handler) {this->handler_ = handler;}
	HandlerType handler() {return this->handler_;}
private:
	HandlerType handler_;
};

#endif