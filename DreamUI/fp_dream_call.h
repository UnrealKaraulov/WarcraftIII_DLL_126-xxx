#pragma once

#include <cstddef>
#include <cstdint>

#if defined(_MSC_VER) || defined(__BORLAND__)
#define AERO_FP_CALL_CDECL __cdecl
#define AERO_FP_CALL_STDCALL __stdcall
#define AERO_FP_CALL_FASTCALL __fastcall
#else
#define AERO_FP_CALL_CDECL
#define AERO_FP_CALL_STDCALL
#define AERO_FP_CALL_FASTCALL
#endif	

namespace dreamaero
{
	typedef void* any_t;
	typedef const void* const_any_t;

	struct pointer_traits
	{
		typedef void (*function_pointer_type)();
		typedef void* pointer_type;
		typedef const void* const_pointer_type;
	};

	typedef pointer_traits::function_pointer_type function_pointer_type;
	typedef pointer_traits::pointer_type pointer_type;
	typedef pointer_traits::const_pointer_type const_pointer_type;

	struct call_traits
	{
		typedef pointer_traits::function_pointer_type entry_point_type;
	};

	typedef call_traits::entry_point_type entry_point_type;

	template <typename PointerReturnType, typename TypeA, typename TypeB>
	inline PointerReturnType pointer_sum(TypeA a, TypeB b)
	{
		return (PointerReturnType)(
			(uint8_t*)(a)+b
			);
	}

	template <typename PointerReturnType, typename TypeA, typename TypeB>
	inline PointerReturnType pointer_substract(TypeA a, TypeB b)
	{
		return (PointerReturnType)(
			(uint8_t*)(a)-b
			);
	}

	template <typename PointerType>
	inline PointerType pointer_calc(pointer_traits::pointer_type pointer, std::ptrdiff_t offset)
	{
		PointerType p = (PointerType)((uint8_t*)(pointer)+offset);
		return p;
	}

	template <typename PointerType>
	inline PointerType array_calc(pointer_traits::pointer_type pointer, std::ptrdiff_t index)
	{
		PointerType p = &((PointerType)(pointer)[index]);
		return p;
	}

	template <typename PointerReturnType, typename TypeA, typename TypeB>
	inline PointerReturnType array_sum(TypeA a, TypeB b)
	{
		return &((PointerReturnType)(a))[b];
	}

	template <typename PointerReturnType, typename TypeA, typename TypeB>
	inline PointerReturnType array_substract(TypeA a, TypeB b)
	{
		return &((PointerReturnType)(a))[-b];
	}

	template <typename Type>
	inline Type offset_element_get(pointer_traits::pointer_type pointer, std::ptrdiff_t offset)
	{
		return *pointer_calc<Type*>(pointer, offset);
	}

	template <typename ReturnType, typename TypeA, typename TypeB>
	inline ReturnType offset_element_sum(TypeA a, TypeB b)
	{
		return *pointer_sum<ReturnType*, TypeA, TypeB>(a, b);
	}

	template <typename ReturnType, typename TypeA, typename TypeB>
	inline ReturnType offset_element_substract(TypeA a, TypeB b)
	{
		return *pointer_substract<ReturnType*, TypeA, TypeB>(a, b);
	}

	template <typename Type>
	inline Type array_element_get(pointer_traits::pointer_type pointer, std::ptrdiff_t index)
	{
		return *array_calc<Type*>(pointer, index);
	}

	template <typename ReturnType, typename TypeA, typename TypeB>
	inline ReturnType array_element_sum(TypeA a, TypeB b)
	{
		return *array_sum<ReturnType*, TypeA, TypeB>(a, b);
	}

	template <typename ReturnType, typename TypeA, typename TypeB>
	inline ReturnType array_element_substract(TypeA a, TypeB b)
	{
		return *array_substract<ReturnType*, TypeA, TypeB>(a, b);
	}

	template <typename Type>
	inline pointer_traits::function_pointer_type make_fp(Type fp)
	{
		return (pointer_traits::function_pointer_type)(fp);
	}

	template <typename Type>
	inline pointer_traits::pointer_type make_p(Type p)
	{
		return (pointer_traits::pointer_type)(p);
	}

	template <typename ReturnType>
	inline ReturnType fast_call(entry_point_type fp)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)();

		((entry_point_type&)(aero_function)) = fp;
		return aero_function();
	}

	template <typename ReturnType, typename A0>
	inline ReturnType fast_call(entry_point_type fp, A0 a0)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0);
	}

	template <typename ReturnType, typename A0, typename A1>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44, typename A45>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44, typename A45, typename A46>
	inline ReturnType fast_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45, A46 a46)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45, A46 a46);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46);
	}

	template <typename ReturnType>
	inline ReturnType std_call(entry_point_type fp)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)();

		((entry_point_type&)(aero_function)) = fp;
		return aero_function();
	}

	template <typename ReturnType, typename A0>
	inline ReturnType std_call(entry_point_type fp, A0 a0)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0);
	}

	template <typename ReturnType, typename A0, typename A1>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44, typename A45>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44, typename A45, typename A46>
	inline ReturnType std_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45, A46 a46)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45, A46 a46);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46);
	}

	template <typename ReturnType>
	inline ReturnType c_call(entry_point_type fp)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)();

		((entry_point_type&)(aero_function)) = fp;
		return aero_function();
	}

	template <typename ReturnType, typename A0>
	inline ReturnType c_call(entry_point_type fp, A0 a0)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0);
	}

	template <typename ReturnType, typename A0, typename A1>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44, typename A45>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44, typename A45, typename A46>
	inline ReturnType c_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45, A46 a46)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45, A46 a46);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46);
	}

	template <typename ReturnType, typename A0>
	inline ReturnType this_call(entry_point_type fp, A0 a0)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0);
	}

	template <typename ReturnType, typename A0, typename A1>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44, typename A45>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45);
	}

	template <typename ReturnType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44, typename A45, typename A46>
	inline ReturnType this_call(entry_point_type fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45, A46 a46)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45, A46 a46);

		((entry_point_type&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46);
	}

	/////////////////////////////////////////////////////////////////////////////////
	template <typename ReturnType, typename FPType>
	inline ReturnType generic_fast_call(FPType fp)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)();

		((FPType&)(aero_function)) = fp;
		return aero_function();
	}

	template <typename ReturnType, typename FPType, typename A0>
	inline ReturnType generic_fast_call(FPType fp, A0 a0)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44, typename A45>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44, typename A45, typename A46>
	inline ReturnType generic_fast_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45, A46 a46)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45, A46 a46);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46);
	}

	template <typename ReturnType, typename FPType>
	inline ReturnType generic_std_call(FPType fp)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)();

		((FPType&)(aero_function)) = fp;
		return aero_function();
	}

	template <typename ReturnType, typename FPType, typename A0>
	inline ReturnType generic_std_call(FPType fp, A0 a0)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44, typename A45>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44, typename A45, typename A46>
	inline ReturnType generic_std_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45, A46 a46)
	{
		ReturnType(AERO_FP_CALL_STDCALL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45, A46 a46);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46);
	}

	template <typename ReturnType, typename FPType>
	inline ReturnType generic_c_call(FPType fp)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)();

		((FPType&)(aero_function)) = fp;
		return aero_function();
	}

	template <typename ReturnType, typename FPType, typename A0>
	inline ReturnType generic_c_call(FPType fp, A0 a0)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44, typename A45>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44, typename A45, typename A46>
	inline ReturnType generic_c_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45, A46 a46)
	{
		ReturnType(AERO_FP_CALL_CDECL * aero_function)(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45, A46 a46);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46);
	}

	template <typename ReturnType, typename FPType, typename A0>
	inline ReturnType generic_this_call(FPType fp, A0 a0)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44, typename A45>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45);
	}

	template <typename ReturnType, typename FPType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16, typename A17, typename A18, typename A19, typename A20, typename A21, typename A22, typename A23, typename A24, typename A25, typename A26, typename A27, typename A28, typename A29, typename A30, typename A31, typename A32, typename A33, typename A34, typename A35, typename A36, typename A37, typename A38, typename A39, typename A40, typename A41, typename A42, typename A43, typename A44, typename A45, typename A46>
	inline ReturnType generic_this_call(FPType fp, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45, A46 a46)
	{
		ReturnType(AERO_FP_CALL_FASTCALL * aero_function)(A0 a0, void* dummy, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14, A15 a15, A16 a16, A17 a17, A18 a18, A19 a19, A20 a20, A21 a21, A22 a22, A23 a23, A24 a24, A25 a25, A26 a26, A27 a27, A28 a28, A29 a29, A30 a30, A31 a31, A32 a32, A33 a33, A34 a34, A35 a35, A36 a36, A37 a37, A38 a38, A39 a39, A40 a40, A41 a41, A42 a42, A43 a43, A44 a44, A45 a45, A46 a46);

		((FPType&)(aero_function)) = fp;
		return aero_function(a0, 0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46);
	}
}

