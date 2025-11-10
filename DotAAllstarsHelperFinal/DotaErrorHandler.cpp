
#include "Main.h"
#include <iomanip>
#include <eh.h>
#include <Psapi.h>
#include "StackWalker.h"


std::string ReadRegistryKeyString(const char* registryKey, const char* registryValue)
{
	// Open the key
	HKEY hKey;
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, registryKey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
		return std::string();

	char str[256] = {};
	unsigned long dwLen = 255;
	LONG ret = RegQueryValueExA(hKey, registryValue, NULL, NULL, (LPBYTE)str, &dwLen);
	RegCloseKey(hKey);

	if (ret == ERROR_SUCCESS)
		return str;
	else
		return std::string();
}

std::string LatestOsDisplayString;

std::string GetOSDisplayString()
{
	std::string productName = ReadRegistryKeyString("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\", "ProductName");
	std::string servicePack = ReadRegistryKeyString("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\", "CSDVersion");
	std::string bitness = ReadRegistryKeyString("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\", "BuildLabEx");

	if (productName.length() == 0)
		productName = "Win98/2000?";

	if (bitness.find("amd64") != std::string::npos)
		bitness = "64-bit";
	else
		bitness = "32-bit";

	LatestOsDisplayString = productName + " " + bitness + " " + servicePack;
	return LatestOsDisplayString;
}

const char* __stdcall get_os_string(int)
{
	GetOSDisplayString();

	return LatestOsDisplayString.c_str();
}




LookupNative LookupNative_org = NULL;
LookupNative LookupNative_ptr;

int __fastcall LookupNative_my(int global, int unused, const char* funcname)
{
	if (funcname && *funcname != '\0')
	{

	}
	else
	{

	}
	int retval = LookupNative_ptr(global, unused, funcname);


	return retval;
}

LookupJassFunc LookupJassFunc_org = NULL;
LookupJassFunc LookupJassFunc_ptr;

signed int __fastcall LookupJassFunc_my(int global, int unused, const char* funcname)
{
	int funcnamefound = false;
	if (funcname && *funcname != '\0')
	{
		funcnamefound = true;

	}
	else
	{

	}

	signed int retval = LookupJassFunc_ptr(global, unused, funcname);


	if (retval == 0)
	{
		if (IsGame())
		{

		}
	}

	return retval;
}

int bDllLogEnable = true;
int __stdcall DllLogEnable(int enable)
{
	bDllLogEnable = enable;
	return 0;
}




std::string GetLastErrorAsString()
{
	//Get the error message, if any.
	unsigned long errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return "NO ERROR"; //No error message has been recorded

	LPSTR messageBuffer = NULL;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPSTR)&messageBuffer, 0, NULL);

	if (size > 2 && messageBuffer != NULL)
	{
		std::string message(messageBuffer, size);

		//Free the buffer.
		LocalFree(messageBuffer);

		return message;
	}
	return std::to_string(errorMessageID);
}



std::string url_encode(const std::string& value)
{
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i)
	{
		std::string::value_type c = (*i);

		// Keep alphanumeric and other accepted characters intact
		/*if ( isalnum( c ) || c == '-' || c == '_' || c == '.' )
		{
		escaped << c;
		continue;
		}*/

		// Any other characters are percent-encoded
		escaped << std::uppercase;
		escaped << '%' << std::setw(2) << int((unsigned char)c);
		escaped << std::nouppercase;
	}

	return escaped.str();
}


#define MAX_PROCESSES 1024 


unsigned long __stdcall FindProcess(const char* lpcszFileName)
{
	unsigned long dwProcessId = 0;
	LPDWORD lpdwProcessIds;
	char* lpszBaseName;
	void* hProcess;
	unsigned long   i, cdwProcesses;


	lpdwProcessIds = (LPDWORD)HeapAlloc(GetProcessHeap(), 0, MAX_PROCESSES * sizeof(unsigned long));
	if (lpdwProcessIds != NULL)
	{
		if (EnumProcesses(lpdwProcessIds, MAX_PROCESSES * sizeof(unsigned long), &cdwProcesses))
		{
			lpszBaseName = (char*)HeapAlloc(GetProcessHeap(), 0, MAX_PATH * sizeof(TCHAR));
			if (lpszBaseName != NULL)
			{
				cdwProcesses /= sizeof(unsigned long);
				for (i = 0; i < cdwProcesses; i++)
				{
					hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, lpdwProcessIds[i]);
					if (hProcess != NULL)
					{
						if (GetModuleBaseNameA(hProcess, NULL, lpszBaseName, MAX_PATH) > 0)
						{
							if (!lstrcmpiA(lpszBaseName, lpcszFileName))
							{
								dwProcessId = lpdwProcessIds[i];
								CloseHandle(hProcess);
								break;
							}
						}
						CloseHandle(hProcess);
					}
				}
				HeapFree(GetProcessHeap(), 0, (LPVOID)lpszBaseName);
			}
		}
		HeapFree(GetProcessHeap(), 0, (LPVOID)lpdwProcessIds);
	}

	return dwProcessId;
}

std::string GetPlatformName()
{
	if (GetModuleHandleA("iccwc3.icc"))
		return "[iCCup]";
	if (GetModuleHandleA("InputHook.dll") && GetModuleHandleA("Overlay.dll"))
		return "[Garena Plus]";
	if (GetModuleHandleA("mroc.dll"))
		return "[RGC]";
	if (FindProcess("rgc.exe") || FindProcess("myroc.exe"))
		return "[RGC]";
	if (!GetModuleHandleA("w3lh.dll"))
		return "[Unknown Or Battle.net]";

	return "[Unknown PVPGN server]";
}

std::string ConvertMemoryToHex(unsigned char* buffer, int size)
{
	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	for (int i = 0; i < size; ++i)
	{
		ss << std::setw(2) << static_cast<unsigned>(buffer[i]);
	}
	return ss.str();
}

std::string ConvertMemoryToHexReverse(unsigned char* buffer, int size)
{
	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	for (int i = size - 1; i >= 0; i--)
	{
		ss << std::setw(2) << static_cast<unsigned>(buffer[i]);
	}
	return ss.str();
}

int IsVEHex = false;


int __stdcall JassLog(int)
{
	return 0;
}
int __stdcall StartExtraErrorHandler(int)
{
	return 0;
}

int __stdcall TraceEsp_Print(int)
{
	return 0;
}

unsigned long __stdcall EXIT_CURRENT_PROCESS(LPVOID)
{
	Sleep(3000);
	try
	{
		TerminateProcess(GetCurrentProcess(), 0);
		ExitProcess(0);
	}
	catch (...)
	{

	}

	return 0;
}


LONG __stdcall DotaVectoredToSehHandler(EXCEPTION_POINTERS* ExceptionInfo)
{
	if (IsVEHex)
	{
		return 0;
	}

	if (!ExceptionInfo)
	{
		std::cerr << "Test: error1" << std::endl;
		ExceptionInfo = new	EXCEPTION_POINTERS();
	}
	if (!ExceptionInfo->ContextRecord)
	{
		std::cerr << "Test: error2" << std::endl;
		ExceptionInfo->ContextRecord = new CONTEXT();
	}

	if (!ExceptionInfo->ExceptionRecord)
	{
		std::cerr << "Test: error3" << std::endl;
		ExceptionInfo->ExceptionRecord = new EXCEPTION_RECORD();
	}


	PEXCEPTION_RECORD ex = ExceptionInfo->ExceptionRecord;

	unsigned long exceptionCode = ExceptionInfo->ExceptionRecord->ExceptionCode;



	char continueablecode[200];

	if ((exceptionCode & ERROR_SEVERITY_ERROR) != ERROR_SEVERITY_ERROR) {
		std::cerr << "Found ERROR_SEVERITY_ERROR..." << std::endl;
		sprintf_s(continueablecode, 200, "%s:%X:%X: addr:%X gamedlladdr:%X [WIN32 error: %s]\n", "Non Fatal Exception: [VEH]ERROR_SEVERITY_ERROR", ex->ExceptionCode, ex->ExceptionFlags, (unsigned int)ex->ExceptionAddress, (unsigned long)GameDll, GetLastErrorAsString().c_str());
		std::cerr << continueablecode << std::endl;
		PrintErrorLog(".\\crashlog_warcraftdll.txt", continueablecode, NULL);
		return ExceptionContinueSearch;
	}


	if (exceptionCode & APPLICATION_ERROR_MASK) {
		std::cerr << "Found APPLICATION_ERROR_MASK..." << std::endl;
		sprintf_s(continueablecode, 200, "%s:%X:%X: addr:%X gamedlladdr:%X [WIN32 error: %s]\n", "Non Fatal Exception: [VEH]APPLICATION_ERROR_MASK", ex->ExceptionCode, ex->ExceptionFlags, (unsigned int)ex->ExceptionAddress, (unsigned long)GameDll, GetLastErrorAsString().c_str());
		std::cerr << continueablecode << std::endl;
		PrintErrorLog(".\\crashlog_warcraftdll.txt", continueablecode, NULL);
		return ExceptionContinueSearch;
	}
	if ((ex->ExceptionFlags & EXCEPTION_NONCONTINUABLE) == 0)
	{
		std::cerr << "Found ExceptionContinueExecution..." << std::endl;
		sprintf_s(continueablecode, 200, "%s:%X:%X: addr:%X gamedlladdr:%X [WIN32 error: %s]\n", "Non Fatal Exception: [VEH]ExceptionContinueExecution", ex->ExceptionCode, ex->ExceptionFlags, (unsigned int)ex->ExceptionAddress, (unsigned long)GameDll, GetLastErrorAsString().c_str());
		std::cerr << continueablecode << std::endl;
		PrintErrorLog(".\\crashlog_warcraftdll.txt", continueablecode, NULL);

		//IsVEHex = true;
		return ExceptionContinueSearch;
	}


	sprintf_s(continueablecode, 200, "%s:%X:%X:  addr:%X gamedlladdr:%X [WIN32 error: %s]\n", "Critical Exception: [VEH]TopLevelExceptionFilter", ex->ExceptionCode, ex->ExceptionFlags, (unsigned int)ex->ExceptionAddress, (unsigned long)GameDll, GetLastErrorAsString().c_str());
	std::cerr << continueablecode << std::endl;
	PrintErrorLog(".\\crashlog_warcraftdll.txt", continueablecode, ExceptionInfo);

	IsVEHex = true;

	std::thread([&]() {
		EXIT_CURRENT_PROCESS(0);
		}).detach();
	return ExceptionContinueSearch;
}

LONG __stdcall DotaSehHandler(_EXCEPTION_POINTERS* ExceptionInfo)
{
	DotaVectoredToSehHandler(ExceptionInfo);
	__asm
	{
		push ExceptionInfo;
		call OriginFilter;
	}
	return ExceptionContinueSearch;
}


LPTOP_LEVEL_EXCEPTION_FILTER OriginFilter = NULL;
LPVOID NewVehFilter = NULL;


void InitTopLevelExceptionFilter()
{
	SetUnhandledExceptionFilter(0);
	SetUnhandledExceptionFilter(DotaSehHandler);
	NewVehFilter = AddVectoredExceptionHandler(1, DotaVectoredToSehHandler);
}

void ResetTopLevelExceptionFilter()
{
	if (NewVehFilter)
		RemoveVectoredExceptionHandler(NewVehFilter);
	NewVehFilter = NULL;

	SetUnhandledExceptionFilter(0);
	SetUnhandledExceptionFilter(OriginFilter);
}

void __stdcall DisableErrorHandler(int)
{
	IsVEHex = false;
	ResetTopLevelExceptionFilter();
}
void __stdcall EnableErrorHandler(int)
{
	DeleteFileA(".\\crashlog_warcraftdll.txt");
	
	IsVEHex = false;
	InitTopLevelExceptionFilter();
}

FILE* debugFile = NULL;

int callStackEntries = 0;
bool modulesStartPrint = false;

struct MDL_Entry
{
	DWORD64 base;
	DWORD size;
	std::string modname;
};

std::vector<MDL_Entry> MdlList;

std::string GetModuleByAddr(DWORD64 addr)
{
	for (auto& s : MdlList)
	{
		if (addr >= s.base && addr <= s.base + s.size)
			return s.modname;
	}

	return std::string();
}

PCONTEXT debug_context = NULL;

class MyStackWalker : public StackWalker
{
public:
	MyStackWalker() : StackWalker() {}
protected:
	virtual void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr)
	{
		(void)(szFuncName); (void)(gle); (void)(addr);
	}
	virtual void OnOutput(LPCSTR szText) {
		(void)(szText);
	}
	virtual void OnLoadModule(LPCSTR    img,
                               LPCSTR    mod,
                               DWORD64   baseAddr,
                               DWORD     size,
                               DWORD     result,
                               LPCSTR    symType,
                               LPCSTR    pdbName,
                               ULONGLONG fileVersion)
	{
		(void)(fileVersion); (void)(pdbName); (void)(fileVersion); (void)(result); (void)(symType);
		if (!modulesStartPrint)
		{
			modulesStartPrint = true;
			fprintf_s(debugFile, "\n%s\n", "Loaded modules:");
		}

		fprintf_s(debugFile, "%llX - %llX %s\n", baseAddr, baseAddr + size, img);
		MdlList.push_back({ baseAddr,size,mod });
	}
	virtual void OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry)
	{
		(void)(eType);
		if (callStackEntries == 0)
		{
			if (debug_context)
			{
				fprintf_s(debugFile, "\n%s\n", "x86 Registers:");
				fprintf_s(debugFile, "EAX=%08X  EBX=%08X  ECX=%08X  EDX=%08X\n", debug_context->Eax, debug_context->Ebx, debug_context->Ecx, debug_context->Edx);
				fprintf_s(debugFile, "ESI=%08X  EDI=%08X  EBP=%08X  ESP=%08X\n", debug_context->Esi, debug_context->Edi, debug_context->Ebp, debug_context->Esp);
				fprintf_s(debugFile, "EIP=%08X  FLG=%08X\n", debug_context->Eip, debug_context->EFlags);
				fprintf_s(debugFile, "CS=%04X  DS=%04X  ES=%04X  SS=%04X  FS=%04X  GS=%04X\n", debug_context->SegCs, debug_context->SegDs, debug_context->SegEs, debug_context->SegSs, debug_context->SegFs, debug_context->SegGs);
			}

			fprintf_s(debugFile, "\n%s\n", "Stack Trace (Using DBGHELP.DLL)");
		}

		callStackEntries++;
		if (callStackEntries < 6)
		{
			std::string modname = GetModuleByAddr(entry.baseOfImage);
			if (modname.empty())
				modname = GetModuleByAddr(entry.offset);
			if (modname.empty())
				modname = "<unknown>";

			entry.name[1023] = '\0';
			fprintf_s(debugFile, "%llX [%llX] %s [func name:%s]", entry.baseOfImage, entry.offset, modname.c_str(), entry.name);
		}
	}
};

std::string GetCurrentDateString() {
	time_t now = time(0);
	struct tm timeinfo;
	char buffer[80];

	localtime_s(&timeinfo, &now);
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

	return std::string(buffer);
}


char dbg_tmp_path[2048];
void* dbg_tmp_modules[2048];
void PrintErrorLog(const char* file, const char* error_str, EXCEPTION_POINTERS* ExceptionInfo)
{
	callStackEntries = 0;
	modulesStartPrint = false;

	fopen_s(&debugFile, file, "a+");
	if (debugFile != NULL)
	{
		fprintf_s(debugFile, "Found Exception:\"%s\"\n", error_str);
		fflush(debugFile);
		fprintf_s(debugFile, "OS:\"%s\"\n", GetOSDisplayString().c_str());
		fflush(debugFile);
		fprintf_s(debugFile, "DATE:\"%s\"\n", GetCurrentDateString().c_str());
		fflush(debugFile);

		if (ExceptionInfo)
		{
			debug_context = ExceptionInfo->ContextRecord;

			if (debug_context)
			{
				StackWalker sw;
				sw.ShowCallstack(GetCurrentThread(), ExceptionInfo->ContextRecord);
			}
		}
		else
			debug_context = NULL;

		if (!GetModuleHandleA("dbghelp.dll") || !debug_context)
		{
			if (!debug_context)
			{
				fprintf_s(debugFile, "%s\n", "Write only loaded modules:");
			}
			else
			{
				fprintf_s(debugFile, "%s\n", "No found dbghelp.dll. Write only loaded modules!");
			}
			unsigned long cbNeeded;
			if (EnumProcessModules(GetCurrentProcess(), (HMODULE*)dbg_tmp_modules, sizeof(dbg_tmp_modules), &cbNeeded))
			{
				for (unsigned int i = 0; i < (cbNeeded / sizeof(void*)); i++)
				{
					GetModuleFileNameA((HMODULE)dbg_tmp_modules[i], dbg_tmp_path, sizeof(dbg_tmp_path));
					fprintf_s(debugFile, "%X : %s\n", (DWORD)dbg_tmp_modules[i], dbg_tmp_path);
				}
			}
			fprintf_s(debugFile, "\n\n\n");
			fflush(debugFile);
			fclose(debugFile);
			debugFile = NULL;
			return;
		}

		fprintf_s(debugFile, "\n\n\n");
		fflush(debugFile);
		fclose(debugFile);
	}

	debugFile = NULL;
}


