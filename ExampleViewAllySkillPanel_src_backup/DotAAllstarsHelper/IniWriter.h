#ifndef INIWRITER_H
#define INIWRITER_H
class CIniWriter
{
public:
 CIniWriter(const char*  szFileName);
 void WriteInteger( const char* szSection, const  char* szKey, int iValue);
 void WriteFloat( const char* szSection, const  char* szKey, float fltValue);
 void WriteBOOLean( const char* szSection, const  char* szKey, BOOL bolValue);
 void WriteString( const char* szSection, const  char* szKey, const char* szValue);
private:
 char m_szFileName[255];
};
#endif //INIWRITER_H