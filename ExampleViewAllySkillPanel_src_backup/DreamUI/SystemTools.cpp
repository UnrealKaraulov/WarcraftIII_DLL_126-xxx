#include "stdafx.h"
#include "SystemTools.h"
#include <winternl.h>

namespace SystemTools {
	bool g_inited = false;

	typedef enum _THREAD_INFORMATION_CLASS {
		ThreadBasicInformation,
		ThreadTimes,
		ThreadPriority,
		ThreadBasePriority,
		ThreadAffinityMask,
		ThreadImpersonationToken,
		ThreadDescriptorTableEntry,
		ThreadEnableAlignmentFaultFixup,
		ThreadEventPair,
		ThreadQuerySetWin32StartAddress,
		ThreadZeroTlsCell,
		ThreadPerformanceCount,
		ThreadAmILastThread,
		ThreadIdealProcessor,
		ThreadPriorityBoost,
		ThreadSetTlsArrayAddress,
		ThreadIsIoPending,
		ThreadHideFromDebugger
	} THREAD_INFORMATION_CLASS, *PTHREAD_INFORMATION_CLASS;

	typedef NTSTATUS (WINAPI *NtQueryInformationThread_t)(
	  __in       HANDLE ThreadHandle,
	  __in       THREAD_INFORMATION_CLASS ThreadInformationClass,
	  __inout    PVOID ThreadInformation,
	  __in       ULONG ThreadInformationLength,
	  __out_opt  PULONG ReturnLength
	);

	NtQueryInformationThread_t NtQueryInformationThread;

	size_t GetVMAllocationSize(void* start) {
		size_t rv = 0;
		MEMORY_BASIC_INFORMATION mbi;
		BYTE* ptr = (BYTE*)start;
		size_t q_result_bytes = VirtualQuery(ptr, &mbi, sizeof(mbi));
		if (q_result_bytes) {
			void* base = mbi.AllocationBase;
			size_t region_size = mbi.RegionSize;
			BYTE* ptr = (BYTE*)start;
			while(q_result_bytes && mbi.AllocationBase == base) {
				ptr += region_size;
				rv += region_size;
				q_result_bytes = VirtualQuery(ptr, &mbi, sizeof(mbi));
			}
		}
		return rv;
	}

	bool GetModuleInfo(const TCHAR* module_name, ModuleInfo* info) {
		bool rv = false;
		HMODULE handle = GetModuleHandle(module_name);
		if (handle) {
			info->handle = handle;
			if ((info->image_size = GetVMAllocationSize((void*)handle)) > 0) {
				rv = true;
			}
		}
		return rv;
	}

	bool GetThreadInfo(DWORD tid, ThreadInfo* info) {
		bool rv = false;
		HANDLE handle = OpenThread(THREAD_QUERY_INFORMATION, FALSE, tid);
		if (handle) {
			DWORD start_addr = NULL;
			ULONG rsize = 0;
			if (NT_SUCCESS(NtQueryInformationThread(handle, ThreadQuerySetWin32StartAddress, &start_addr, 4, &rsize)) && start_addr) {
				info->start_address = (void*)start_addr;
				rv = true;
			}
			CloseHandle(handle);
		}
		return rv;
	}

	bool Init() {
		bool rv = false;
		HMODULE ntdll = GetModuleHandle("ntdll.dll");
		if (ntdll) {
			if (NULL != (NtQueryInformationThread = (NtQueryInformationThread_t)GetProcAddress(ntdll, "NtQueryInformationThread")) )
			{
				rv = true;
			}
		}
		g_inited = rv;
		return rv;
	}

	void Cleanup() {
		if (g_inited) {

		}
	}
}