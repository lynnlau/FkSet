#include "StdAfx.h"
#include "global.h"
#include "stdio.h"
#include "stdlib.h"
#include "Mmsystem.h"
#include <afxmt.h>
#include "Afxsock.h"

#pragma comment(lib,"Winmm.lib")

int theDebugToNet=0;
int theDebugToDisk=1;

//char theProgramPath[MAX_PATH]={0x00};
CCriticalSection g_logfilemutex;

char csPhaseCode[2][4] = {'0','3','0','4','1','e','2','G'};
unsigned char g_mac[6];

// 获取本模块所在的路径
int GetModulePath(char *path)
{

//	if (theProgramPath[0]==0x00)
//	{
		::GetModuleFileName(NULL,path,MAX_PATH);

		char *pFind = strrchr(path,'\\');
		if (pFind!=NULL) 
			*(pFind+1)=0;
		else
			path=NULL;

//		strcpy(theProgramPath,path);		
//	}
//	else
//	{
//		strcpy(path,theProgramPath);
//	}

	return 1;
}

void ulong2bcdex_r(int tmp, char *pbcd, int bcdlen, char symbol)
{
	int i;

	memset(pbcd, symbol, bcdlen);
	if(tmp==0)
	{
		return;
	}
	for(i=0; (i<bcdlen)&&(tmp!=0); i++)
	{
		pbcd[i]=tmp%10;
		tmp/=10;
		pbcd[i]|=((tmp%10)<<4);
		tmp/=10;
	}

	return;
}

// RTU地址转换.STR to CODE
int RtuStrToCode(char *Str, char* Code)
{
	int i;
	int n;
//	if (strlen(Str)!=9) return -1;
	int len = strlen(Str);

	if(len == 8)
	{
		memset(Code,0,4);
		for(i = 0; i < 8; i++)
		{
			if(!isxdigit(Str[i]))
				return -1;

			if((Str[i] >='A' && Str[i] <= 'F')
				||(Str[i] >='a' && Str[i] <= 'f'))
			{

				Code[i/2]|=((_toupper(Str[i])-'A'+10)<<((i%2)*4));
			}
			else
			{
				Code[i/2]|=((Str[i]-'0')<<((i%2)*4));
			}
			
		}
		
		return 0;
	}

	if((len!=9)&&(len!=12)) 
	{
		return -1;//  吉林地址四个字节bcd码
	}
	for (i=0;i<len;i++)
	{
		n = Str[i];
		if (isdigit(n)==FALSE) return -2;
	}

	Code[1]= (Str[0]-'0')*16+(Str[1]-'0');
	Code[0]= (Str[2]-'0')*16+(Str[3]-'0');

	n = atoi(&Str[4]);
	//if (n>65535) return -3;
	if (n>99999999) return -3;

	int addrbytes=((len==9)?2:4);
	//memcpy((void*)&Code[2],(void*)&n,2);
	int naddr = n;
	if(len==12) // bcd
	{
		ulong2bcdex_r(n,Code+2,4,0);
	}
	else
	{
		memcpy((void*)&Code[2],(void*)&naddr,addrbytes);
	}
#if 0
	CString strtmp="";
	for (i =0;i<6;i++)
	{
		strtmp.Format("%02x",Code[i]);
	//	AfxMessageBox(strtmp);
	}
#endif
	return 0;
}

int RtuCodeToStr(char *Code, char* Str)
{
	int n=0;

	Str[0] = ((Code[1]&0xF0)>>4)+'0';
	Str[1] = ((Code[1]&0x0F))+'0';
	Str[2] = ((Code[0]&0xF0)>>4)+'0';
	Str[3] = ((Code[0]&0x0F))+'0';

	memcpy((void*)&n,(void*)&Code[2],2);

	sprintf(&Str[4],"%05d",n);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
//
//                     DEBUG 相关
//
/////////////////////////////////////////////////////////////////////////////////////


// 格式化输出调试信息

#ifdef _DEBUG_OUT_

int DiskDebugOut(char* data,int len)
{
	CString strTmp;
	char file[MAX_PATH];
	g_logfilemutex.Lock();

	GetModulePath(file);
	CTime tm = CTime::GetCurrentTime();

	strTmp.Format("run%04d%02d%02d.log",
		tm.GetYear(),
		tm.GetMonth(),
		tm.GetDay());
	strcat(file,strTmp);

	if (theDebugToDisk)
	{
		FILE* fp = fopen(file,"ab");
		if(fp != NULL)
		{
			fwrite(data,len,1,fp);
			fclose(fp);
		}
	}

	g_logfilemutex.Unlock();

	return 0;
}

void LogFileDS(char* data,char* filename,int linenum)
{
	char temp[32];
	struct tm *newtime;
	time_t aclock;

	time( &aclock );                 /* Get time in seconds */
	newtime = localtime( &aclock );  /* Convert time to struct */
									/* tm form */
	sprintf(temp,"%02d-%02d ",newtime->tm_mon+1,newtime->tm_mday);
	DiskDebugOut(temp,strlen(temp));
	
	sprintf(temp,"%02d:%02d:%02d ",newtime->tm_hour,newtime->tm_min,newtime->tm_sec);
	DiskDebugOut(temp,strlen(temp));
	filename = strrchr(filename,'\\')+1;
	DiskDebugOut(filename,strlen(filename));
	sprintf(temp," %4d: ",linenum);
	DiskDebugOut(temp,strlen(temp));
	DiskDebugOut(data,strlen(data));
	DiskDebugOut("\r\n",strlen("\r\n"));
}

int LogFileFmt(int Destination, char* filename,int linenum, const char*format, ...)
{
	int nRet;
	char sDebug[300];
	va_list args;
	va_start(args, format);
	nRet = vsprintf(sDebug, format, args);
	va_end(args);

	if ((Destination &_DISK_FLAG) ==_DISK_FLAG)
	{
		LogFileFmtDisk(filename,linenum, sDebug);
	}

	return (nRet);
}

// 格式化输出调试信息到磁盘文件
int LogFileFmtDisk(char* filename,int linenum, char*buffer)
{
	char temp[32];
	struct tm *newtime;
	time_t aclock;

	time( &aclock );                 /* Get time in seconds */
	newtime = localtime( &aclock );  /* Convert time to struct */
									/* tm form */

	/* Print local time as a string */
	sprintf(temp,"%02d-%02d %02d:%02d:%02d ",newtime->tm_mon+1,newtime->tm_mday,newtime->tm_hour,newtime->tm_min,newtime->tm_sec);
	DiskDebugOut(temp,strlen(temp));
	filename = strrchr(filename,'\\')+1;
	DiskDebugOut(filename,strlen(filename));
	sprintf(temp," %4d: ",linenum);
	DiskDebugOut(temp,strlen(temp));
	DiskDebugOut(buffer,strlen(buffer));
	DiskDebugOut("\r\n",strlen("\r\n"));

	return 0;
}


void LogHexEx(char* pTip, char* pBuf, int size,char* filename,int linenum)
{
	char temp[32];
	char sDebug[300];

	struct tm *newtime;
	time_t aclock;

	time( &aclock );                 /* Get time in seconds */
	newtime = localtime( &aclock );  /* Convert time to struct */
									/* tm form */
	sprintf(temp,"%02d-%02d ",newtime->tm_mon+1,newtime->tm_mday);
	DiskDebugOut(temp,strlen(temp));

	sprintf(temp,"%02d:%02d:%02d ",newtime->tm_hour,newtime->tm_min,newtime->tm_sec);
	DiskDebugOut(temp,strlen(temp));
	filename = strrchr(filename,'\\')+1;
	DiskDebugOut(filename,strlen(filename));
	sprintf(temp," %4d: ",linenum);
	DiskDebugOut(temp,strlen(temp));

	DiskDebugOut(pTip, strlen(pTip));
	int i=0;
	sDebug[0]=0;
	while(i<size)
	{
		sprintf(temp,"%02X ",(unsigned char)(*pBuf++));
		i++;

#if 1
		strcat(sDebug,temp);
		int len=strlen(sDebug);
		if ((len>sizeof(sDebug)-10)||(i==size)) 
		{
			DiskDebugOut(sDebug,len);
			sDebug[0]=0;
		}
#else			
		DiskDebugOut(temp,3);
#endif
	}
	DiskDebugOut("\r\n",strlen("\r\n"));
}


#else
int LogFileFmt(int Destination, char* filename,int linenum, const char*format, ...)
{
	return 0;
}
#endif


unsigned short CRC16(unsigned char *byte, unsigned short nbyte)
/* 16位CRC校验 */
{
	unsigned short itemp=0;
	unsigned short i, j;
	unsigned short iflag;	
	for(i=0; i<nbyte; i++)
	{
		itemp^=(byte[i]<<8);
		for(j=0; j<8; j++)
		{
			iflag=itemp&0x8000;
			itemp<<=1;
			if(iflag)
			{
				itemp^=0x1021;
			}
		}
	}
	LogFileFmt(TOALL,"crc is %d",itemp);

	return itemp;
}

int reserve(char*  str)   
{   
	char stemp[256] = "";
	memcpy(stemp,str,strlen(str));
	int len = strlen(str);
	for (int i=0;i<len;i++)
	{
		str[i] = stemp[len-1-i];
	}
	return len;
}

