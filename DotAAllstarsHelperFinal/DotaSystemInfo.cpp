#include "Main.h"

#include <gl\gl.h>
#include <gl\glu.h>

enum class SystemInfo : unsigned int
{
	PHYS_BYTES = 0,
	VIRTUAL_BYTES = 1,
	PAGE_BYTES = 2,
	PHYS_BYTES_CUR = 3,
	VIRTUAL_BYTES_CUR = 4,
	PAGE_BYTES_CUR = 5,
	PHYS_LOAD_PERCENT = 6,
	PROC_ARCH = 7,
	PROC_COUNT = 8,
	PROC_NAME = 9,
	PROC_SPEED = 10,
	PROC_VENDOR = 11,
	MOTHERBOARD = 12,
	BIOS = 13,
	GPU_INFO = 14,
	GPU_VENDOR = 15,
	GPU_VERSION = 16,
	OS_VER = 17,
	OS_NAME = 18

};

int GetProcCount()
{
	int processors = 0;
	for (int i = 0; i < 32; i++)
	{
		HKEY hKey;
		long lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
			("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\" + std::to_string(i)).c_str(),
			0,
			KEY_READ,
			&hKey);

		if (lError != ERROR_SUCCESS)
		{
			break;
		}

		processors++;
	}

	return processors;
}

std::string ProcSpeedRead(int id)
{
	char Buffer[_MAX_PATH];
	unsigned long BufSize = _MAX_PATH;
	unsigned long dwMHz = _MAX_PATH;
	HKEY hKey;

	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\" + std::to_string(id)).c_str(),
		0,
		KEY_READ,
		&hKey);

	if (lError != ERROR_SUCCESS)
	{
		return "N/A";
	}

	RegQueryValueExA(hKey, "~MHz", NULL, NULL, (LPBYTE)&dwMHz, &BufSize);

	sprintf_s(Buffer, "%u", dwMHz);

	return Buffer;
}

std::string ProcNameRead(int id)
{
	char Buffer[_MAX_PATH];
	unsigned long BufSize = _MAX_PATH;
	HKEY hKey;

	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\" + std::to_string(id)).c_str(),
		0,
		KEY_READ,
		&hKey);

	if (lError != ERROR_SUCCESS)
	{
		return "N/A";
	}

	// query the key:
	unsigned long dwType = REG_SZ;

	RegQueryValueExA(hKey, "ProcessorNameString", NULL, &dwType, (LPBYTE)Buffer, &BufSize);

	return Buffer;
}

std::string ProcVendorRead(int id)
{
	char Buffer[_MAX_PATH];
	unsigned long BufSize = _MAX_PATH;
	HKEY hKey;

	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\" + std::to_string(id)).c_str(),
		0,
		KEY_READ,
		&hKey);

	if (lError != ERROR_SUCCESS)
	{
		return "N/A";
	}

	// query the key:
	unsigned long dwType = REG_SZ;

	RegQueryValueExA(hKey, "VendorIdentifier", NULL, &dwType, (LPBYTE)Buffer, &BufSize);

	return Buffer;
}

std::string MotherBoard_Manufactured()
{
	char Buffer[_MAX_PATH];
	unsigned long BufSize = _MAX_PATH;
	HKEY hKey;

	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"HARDWARE\\DESCRIPTION\\System\\BIOS",
		0,
		KEY_READ,
		&hKey);

	if (lError != ERROR_SUCCESS)
	{
		return "N/A";
	}

	// query the key:
	unsigned long dwType = REG_SZ;

	RegQueryValueExA(hKey, "BaseBoardManufacturer", NULL, &dwType, (LPBYTE)Buffer, &BufSize);

	return Buffer;
}

std::string MotherBoard_Model()
{
	char Buffer[_MAX_PATH];
	unsigned long BufSize = _MAX_PATH;
	HKEY hKey;

	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"HARDWARE\\DESCRIPTION\\System\\BIOS",
		0,
		KEY_READ,
		&hKey);

	if (lError != ERROR_SUCCESS)
	{
		return "N/A";
	}

	// query the key:
	unsigned long dwType = REG_SZ;

	RegQueryValueExA(hKey, "BaseBoardProduct", NULL, &dwType, (LPBYTE)Buffer, &BufSize);

	return Buffer;
}

std::string GetMotherBoardInfo()
{
	return MotherBoard_Manufactured() + " /// " + MotherBoard_Model();
}


std::string BIOS_Vendor()
{
	char Buffer[_MAX_PATH];
	unsigned long BufSize = _MAX_PATH;
	HKEY hKey;

	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"HARDWARE\\DESCRIPTION\\System\\BIOS",
		0,
		KEY_READ,
		&hKey);

	if (lError != ERROR_SUCCESS)
	{
		return "N/A";
	}

	// query the key:
	unsigned long dwType = REG_SZ;

	RegQueryValueExA(hKey, "BIOSVendor", NULL, &dwType, (LPBYTE)Buffer, &BufSize);

	return std::string(Buffer);
}


std::string BIOS_ReleaseData()
{
	char Buffer[_MAX_PATH];
	unsigned long BufSize = _MAX_PATH;
	HKEY hKey;

	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"HARDWARE\\DESCRIPTION\\System\\BIOS",
		0,
		KEY_READ,
		&hKey);

	if (lError != ERROR_SUCCESS)
	{
		return "N/A";
	}

	// query the key:
	unsigned long dwType = REG_SZ;

	RegQueryValueExA(hKey, "BIOSReleaseDate", NULL, &dwType, (LPBYTE)Buffer, &BufSize);

	return  std::string(Buffer);
}


std::string BIOS_Version()
{
	char Buffer[_MAX_PATH];
	unsigned long BufSize = _MAX_PATH;
	HKEY hKey;

	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"HARDWARE\\DESCRIPTION\\System\\BIOS",
		0,
		KEY_READ,
		&hKey);

	if (lError != ERROR_SUCCESS)
	{
		return "N/A";
	}

	// query the key:
	unsigned long dwType = REG_SZ;

	RegQueryValueExA(hKey, "BIOSVersion", NULL, &dwType, (LPBYTE)Buffer, &BufSize);

	return  std::string(Buffer);
}

std::string GetBiosInfo()
{
	return BIOS_Vendor() + " /// " + BIOS_ReleaseData() + " /// " + BIOS_Version();
}

std::string GetOSversion()
{
	char Buffer[_MAX_PATH];
	unsigned long BufSize = _MAX_PATH;
	HKEY hKey;

	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
		0,
		KEY_READ,
		&hKey);

	if (lError != ERROR_SUCCESS)
	{
		return "N/A";
	}

	// query the key:
	unsigned long dwType = REG_SZ;

	RegQueryValueExA(hKey, "CurrentVersion", NULL, &dwType, (LPBYTE)Buffer, &BufSize);

	return  std::string(Buffer);
}


std::string GetOSname()
{
	char Buffer[_MAX_PATH];
	unsigned long BufSize = _MAX_PATH;
	HKEY hKey;

	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
		0,
		KEY_READ,
		&hKey);

	if (lError != ERROR_SUCCESS)
	{
		return "N/A";
	}

	// query the key:
	unsigned long dwType = REG_SZ;

	RegQueryValueExA(hKey, "ProductName", NULL, &dwType, (LPBYTE)Buffer, &BufSize);

	return  std::string(Buffer);
}

char GlobalBuf[1024];
bool FirstInit = true;

const char* __stdcall GetSystemInfo2(SystemInfo info, unsigned int DIV/*or arg2*/)
{
	GlobalBuf[0] = '\0';

	if (FirstInit)
	{
		FirstInit = false;
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			0,
			(unsigned char)PFD_SUPPORT_OPENGL,    // Flags
			0,        // The kind of framebuffer. RGBA or palette.
			0,                   // Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			0,                   // Number of bits for the depthbuffer
			8,                    // Number of bits for the stencilbuffer
			0,                    // Number of Aux buffers in the framebuffer.
			(unsigned char)PFD_UNDERLAY_PLANE,
			0,
			0, 0, 0
		};

		HDC ourWindowHandleToDeviceContext = GetDC(Warcraft3Window);
		if (!ourWindowHandleToDeviceContext)
			return "Error #1";
		int  letWindowsChooseThisPixelFormat;
		letWindowsChooseThisPixelFormat = ChoosePixelFormat(ourWindowHandleToDeviceContext, &pfd);
		SetPixelFormat(ourWindowHandleToDeviceContext, letWindowsChooseThisPixelFormat, &pfd);

		HGLRC ourOpenGLRenderingContext = wglCreateContext(ourWindowHandleToDeviceContext);
		if (!ourOpenGLRenderingContext)
			return "Error #2`";
		wglMakeCurrent(ourWindowHandleToDeviceContext, ourOpenGLRenderingContext);
	}


	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo);
	switch (info)
	{
	case SystemInfo::PROC_ARCH:
		sprintf_s(GlobalBuf, "%s", "UNKNOWN");
		if (siSysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
		{
			sprintf_s(GlobalBuf, "%s", "AMD64");
		}
		if (siSysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
		{
			sprintf_s(GlobalBuf, "%s", "IA64");
		}
		if (siSysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
		{
			sprintf_s(GlobalBuf, "%s", "x86");
		}
		break;
	case SystemInfo::PROC_COUNT:
		sprintf_s(GlobalBuf, "%i", GetProcCount());
		break;
	case SystemInfo::PROC_NAME:
		sprintf_s(GlobalBuf, "%s", ProcNameRead(DIV).c_str());
		break;
	case SystemInfo::PROC_SPEED:
		sprintf_s(GlobalBuf, "%s", ProcSpeedRead(DIV).c_str());
		break;
	case SystemInfo::PROC_VENDOR:
		sprintf_s(GlobalBuf, "%s", ProcVendorRead(DIV).c_str());
		break;
	default:
		break;
	}

	switch (info)
	{
	case SystemInfo::MOTHERBOARD:
		sprintf_s(GlobalBuf, "%s", GetMotherBoardInfo().c_str());
		break;
	case  SystemInfo::BIOS:
		sprintf_s(GlobalBuf, "%s", GetBiosInfo().c_str());
		break;
	default:
		break;
	}

	switch (info)
	{
	case SystemInfo::GPU_VENDOR:
		if (glGetString(GL_VENDOR))
			sprintf_s(GlobalBuf, "%s", (char*)glGetString(GL_VENDOR));
		break;
	case SystemInfo::GPU_INFO:
		if (glGetString(GL_RENDERER))
			sprintf_s(GlobalBuf, "%s", (char*)glGetString(GL_RENDERER));
		break;
	case SystemInfo::GPU_VERSION:
		if (glGetString(GL_VERSION))
			sprintf_s(GlobalBuf, "%s", (char*)glGetString(GL_VERSION));
		break;
	default:
		break;
	}

	switch (info)
	{
	case SystemInfo::OS_VER:
		sprintf_s(GlobalBuf, "%s", GetOSversion().c_str());
		break;
	case  SystemInfo::OS_NAME:
		sprintf_s(GlobalBuf, "%s", GetOSname().c_str());
		break;
	default:
		break;
	}


	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);

	if (DIV == 0)
		DIV = 1;

	switch (info)
	{
	case SystemInfo::PHYS_BYTES:
		sprintf_s(GlobalBuf, "%u", (unsigned long)(statex.ullTotalPhys / DIV));
		break;
	case SystemInfo::VIRTUAL_BYTES:
		sprintf_s(GlobalBuf, "%u", (unsigned long)(statex.ullTotalVirtual / DIV));
		break;
	case SystemInfo::PAGE_BYTES:
		sprintf_s(GlobalBuf, "%u", (unsigned long)(statex.ullTotalPageFile / DIV));
		break;
	case SystemInfo::PHYS_BYTES_CUR:
		sprintf_s(GlobalBuf, "%u", (unsigned long)(statex.ullAvailPhys / DIV));
		break;
	case SystemInfo::VIRTUAL_BYTES_CUR:
		sprintf_s(GlobalBuf, "%u", (unsigned long)(statex.ullAvailVirtual / DIV));
		break;
	case SystemInfo::PAGE_BYTES_CUR:
		sprintf_s(GlobalBuf, "%u", (unsigned long)(statex.ullAvailPageFile / DIV));
		break;
	case SystemInfo::PHYS_LOAD_PERCENT:
		sprintf_s(GlobalBuf, "%u", (unsigned long)(statex.dwMemoryLoad));
		break;
	default:
		break;
	}
	if (GlobalBuf[0] == '\0')
		sprintf_s(GlobalBuf, "N/A");
	return GlobalBuf;
}