#include "stdafx.h"
#ifndef RCSTRING_H_INCLUDED
#define RCSTRING_H_INCLUDED

#include "StringStructs.h"

namespace RCString {
	war3::RCString* create(const char* str = NULL);
	void destroy(war3::RCString* t);

	void setString(war3::RCString* t, const char* str);
	const char* getString(war3::RCString* t);
}


#endif