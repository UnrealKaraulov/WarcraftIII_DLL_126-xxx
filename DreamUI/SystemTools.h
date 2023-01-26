#ifndef SYSTEMTOOLS_H_
#define SYSTEMTOOLS_H_

namespace SystemTools {
	struct ModuleInfo {
		void* handle;
		size_t image_size;
	};

	struct ThreadInfo {
		void* start_address;
	};

	size_t GetVMAllocationSize(void* start);
	bool GetModuleInfo(const TCHAR* module_name, ModuleInfo* info);
	bool GetThreadInfo(unsigned long tid, ThreadInfo* info);

	bool Init();
	void Cleanup();
}

#endif