#include "stdafx.h"
#ifndef CUSTOM_DATA_OBJECT_H_INCLUDED_
#define CUSTOM_DATA_OBJECT_H_INCLUDED_

class CustomDataObject {
public:
	CustomDataObject() {
		this->data_ = NULL;
		this->data_size_ = 0;
	}

	virtual ~CustomDataObject() {
		if (NULL != this->data_)
		{
			delete this->data_;
			this->data_ = NULL;
		}
	}

	template<typename DataType>
	void setData(DataType* data) {
		if (this->data_size_ < sizeof(DataType)) {
			delete this->data_;
			this->data_ = new DataType;
		}
		memcpy_s(this->data_, sizeof(DataType), data, sizeof(DataType));
	}

	template <typename DataType>
	DataType* data() const {
		return static_cast<DataType*>(this->data_);
	}
	/*
	void* data() const {
		(void *)this->data_;
	}*/
private:
	void* data_;
	unsigned int data_size_;
};

#endif