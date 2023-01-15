#include <iostream> 
#include "Main.h"
#include "IniWriter.h"

CIniWriter::CIniWriter(const char* szFileName)
{
	memset(m_szFileName, 0x00, 255);
	memcpy(m_szFileName, szFileName, strlen(szFileName) + 1);
}
void CIniWriter::WriteInteger(const char* szSection, const  char* szKey, int iValue)
{
	char szValue[255];
	sprintf_s(szValue, 255, "%d", iValue);
	WritePrivateProfileStringA(szSection, szKey, szValue, m_szFileName);
}
void CIniWriter::WriteFloat(const char* szSection, const  char* szKey, float fltValue)
{
	char szValue[255];
	sprintf_s(szValue, 255, "%f", fltValue);
	WritePrivateProfileStringA(szSection, szKey, szValue, m_szFileName);
}
void CIniWriter::WriteBOOLean(const char* szSection, const  char* szKey, int bolValue)
{
	char szValue[255];
	sprintf_s(szValue, 255, "%s", bolValue ? "true" : "false");
	WritePrivateProfileStringA(szSection, szKey, szValue, m_szFileName);
}
void CIniWriter::WriteString(const char* szSection, const  char* szKey, const char* szValue)
{
	WritePrivateProfileStringA(szSection, szKey, szValue, m_szFileName);
}