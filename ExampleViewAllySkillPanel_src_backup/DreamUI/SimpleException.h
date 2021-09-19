#include "stdafx.h"
#ifndef SIMPLE_EXCEPTION_H_INCLUDED
#define SIMPLE_EXCEPTION_H_INCLUDED

#include <exception>

class SimpleException : public std::exception {
public:
	SimpleException(const char* msg) {
		this->msg_ = msg;
	}

	virtual const char* what() const throw() {
		return this->msg_;;
	}
private:
	const char* msg_;
};

#endif