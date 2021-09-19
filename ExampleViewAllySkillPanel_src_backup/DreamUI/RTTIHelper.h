#include "stdafx.h"
#ifndef RTTIHELPER_INCLUDED
#define RTTIHELPER_INCLUDED
#include "fp_call.h"
namespace RTTIHelper {
	struct TypeDescriptor {
		DWORD pVFTable;
		DWORD spare;
		char name[];
	};

	/*	
	RTTI (Run-Time Type Identification) is special compiler-generated information which is used to support C++ operators like dynamic_cast<> and typeid(), and also for C++ exceptions. Due to its nature, RTTI is only required (and generated) for polymorphic classes, i.e. classes with virtual functions. 
	MSVC compiler puts a pointer to the structure called "Complete Object Locator" just before the vftable. The structure is called so because it allows compiler to find the location of the complete object from a specific vftable pointer (since a class can have several of them). COL looks like following: 
	*/
	struct RTTICompleteObjectLocator
	{
		DWORD signature; //always zero ?
		DWORD offset;    //offset of this vtable in the complete class
		DWORD cdOffset;  //constructor displacement offset
		struct TypeDescriptor* pTypeDescriptor; //TypeDescriptor of the complete class
		struct RTTIClassHierarchyDescriptor* pClassDescriptor; //describes inheritance hierarchy
	};

	/*
	Class Hierarchy Descriptor describes the inheritance hierarchy of the class. It is shared by all COLs for a class. 
	*/
	struct RTTIClassHierarchyDescriptor
	{
		DWORD signature;      //always zero?
		DWORD attributes;     //bit 0 set = multiple inheritance, bit 1 set = virtual inheritance
		DWORD numBaseClasses; //number of classes in pBaseClassArray
		struct RTTIBaseClassArray* pBaseClassArray;
	};

	struct PMD
	{
		int mdisp;  //member displacement
		int pdisp;  //vbtable displacement
		int vdisp;  //displacement inside vbtable
	};

	/*
	Base Class Array describes all base classes together with information which allows compiler to cast the derived class to any of them during execution of the _dynamic_cast_ operator. Each entry (Base Class Descriptor) has the following structure: 
	*/
	struct RTTIBaseClassDescriptor
	{
		struct TypeDescriptor* pTypeDescriptor; //type descriptor of the class
		DWORD numContainedBases; //number of nested classes following in the Base Class Array
		struct PMD where;        //pointer-to-member displacement info
		DWORD attributes;        //flags, usually 0
	};

	inline void** vtable(void* obj) {return *(void***)obj;}

	inline const char* className(void* obj) {
		RTTICompleteObjectLocator* col = *dreamaero::pointer_calc<RTTICompleteObjectLocator**>(vtable(obj), -4);
		return &(col->pTypeDescriptor->name[4]);
	}

	/*
	bool isInstanceOf(void* obj, const char* parentName) {
		bool rv = false;
		RTTICompleteObjectLocator* col = *dreamaero::pointer_calc<RTTICompleteObjectLocator**>(vtable(obj), -4);
		uint32_t parent_count = col->pClassDescriptor->numBaseClasses;
		size_t name_len = strlen(parentName);
		const char* object_class_name = &(col->pTypeDescriptor->name[4]);
		if (parent_count && name_len) {
			for (uint32_t i = 0; i < parent_count; ++i) {
				const char* parent_name = col->pClassDescriptor->pBaseClassArray 
				if (strstr(object_class_name, parentName) != NULL && strlen())
					return true;
			}
		}
		return rv;
	}
	*/
};

#endif