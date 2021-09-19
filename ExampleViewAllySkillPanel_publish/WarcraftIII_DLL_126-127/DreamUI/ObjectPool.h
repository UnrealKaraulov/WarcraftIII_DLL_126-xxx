#include "stdafx.h"
#ifndef OBJECT_POOL_H_INCLUDED
#define OBJECT_POOL_H_INCLUDED

#include "Storm.h"

template <class T>
class ObjectPool {
public:
	typedef void (*ObjectPoolConstructFunctionType)(T* obj);
	typedef void (*ObjectPoolDestructFunctionType)(T* obj);
	ObjectPool(uint32_t defaultSize = 32, ObjectPoolConstructFunctionType construct = NULL, ObjectPoolDestructFunctionType destruct = NULL);
	~ObjectPool();
	T* get(bool reset = false);
	void ret(T* obj);
	void check() {
		if (this->list_.size() != this->pool_.size()) {
			////WriteGameLog("OP: size_l != size_p.");
		}
	}
private:
	ObjectPoolConstructFunctionType objConstructFunc_;
	ObjectPoolDestructFunctionType objDestructFunc_;
	std::list<T*> list_;
	std::list<T*> pool_;

	DISALLOW_COPY_AND_ASSIGN(ObjectPool<T>);
};

template <class T>
ObjectPool<T>::ObjectPool(uint32_t defaultSize, ObjectPoolConstructFunctionType construct, ObjectPoolDestructFunctionType destruct) {
	this->objConstructFunc_ = construct;
	this->objDestructFunc_ = destruct;
	
	for (uint32_t i = 0; i < defaultSize; ++i) {
		T* obj = new T;
		if (this->objConstructFunc_) 
			this->objConstructFunc_(obj);
		this->list_.push_back(obj);
		this->pool_.push_back(obj);
	}
}

template <class T>
T* ObjectPool<T>::get(bool reset) {
	T* obj;
	if (!this->pool_.size()) {
		obj = new T;
		if (reset)
			memset(obj, 0, sizeof(T));
		if (this->objConstructFunc_) 
			this->objConstructFunc_(obj);		
		this->list_.push_back(obj);
		if (!obj) {
			//WriteGameLog("OP: Generate new object failed! this = 0x%08X, size_p = %u, size_l = %u.", this, this->pool_.size(), this->list_.size());
		}
	} else {
		obj = this->pool_.front();
		if (reset)
			memset(obj, 0, sizeof(T));
		this->pool_.pop_front();
		if (!obj) {
			//WriteGameLog("OP: Get reusable object failed! this = 0x%08X, size_p = %u, size_l = %u.", this, this->pool_.size(), this->list_.size());
		}
	}
	return obj;
}

template <class T>
void ObjectPool<T>::ret(T* obj) {
	if (obj) {
		this->pool_.push_back(obj);
	} else {
		//WriteGameLog("OP: NULL returned!");
	}
}

template <class T>
ObjectPool<T>::~ObjectPool() {
	for (T * obj : this->list_)
	{
		if (this->objDestructFunc_)
			this->objDestructFunc_(obj);
		if (obj) {
			delete obj;
		}
		else {
			//WriteGameLog("OP: NULL in free list!");
		}
	}

}

#endif
