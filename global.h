#ifndef _GLOBAL_H_
#define _GLOBAL_H_

// file name
int GetModulePath(char *path);

int mstimeout(DWORD *timep, int msec);

int RtuCodeToStr(char *Code, char* Str);
int RtuStrToCode(char *Str, char* Code);

unsigned short CRC16(unsigned char *pdata, unsigned short datalen);
int  reserve(char*  str);

//////////////////////////////////////////////////////////////////////////////////////
//
//                     DEBUG 相关
//
/////////////////////////////////////////////////////////////////////////////////////
#define  _DEBUG_OUT_            // 允许调试信息输出，代码级关闭


#define _DISK_FLAG               0x01
#define _PORT_FLAG               0x02
#define TODISK        _DISK_FLAG,  __FILE__,__LINE__
#define TOPORT        _PORT_FLAG,  __FILE__,__LINE__
#define TOALL        _PORT_FLAG | _DISK_FLAG,  __FILE__,__LINE__

// 规约定义
#define _PROTOCOL_FK2004 	1
#define _PROTOCOL_DLT698	2

void LogHexEx(char* pTip, char* pBuf, int size,char* filename,int linenum);
#ifdef _DEBUG_OUT_
	#define LogHex(a,b,c)   LogHexEx(a,b,c,__FILE__,__LINE__)
	#define LogFile(x)   LogFileDS(x,__FILE__,__LINE__)

	void LogFileDS(char* data,char* filename,int linenum);
	int LogFileFmt(int Destination, char* filename,int linenum, const char*format, ...);
	int LogFileFmtDisk(char* filename,int linenum, char*buffer);
	int DiskDebugOut(char* data,int len);

#else
	#define LogHex(a,b,c)
	#define LogFile(x)   
	int LogFileFmt(int Destination, char* filename,int linenum, const char*format, ...);
#endif


#endif