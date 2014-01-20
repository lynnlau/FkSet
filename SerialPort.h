#ifndef __SERIALPORT_H
#define __SERIALPORT_H

#include "afxwin.h"
#include "FkSetData.h"

class CSerialPort  
{
public:
	CSerialPort();
	virtual ~CSerialPort();
	
	DWORD ReadData(LPVOID cpRXBuffer,OVERLAPPED* olRead,int size=BUFFER_SIZE);
	DWORD WriteData(LPVOID lpBuffer,DWORD dwLength);
	DWORD WriteData(CString str);
	BOOL IsOpen (void) const	{ return m_bOpened; }
	BOOL UnInit();
	BOOL Init(ACCONNECTDATA* pConData);
	ACCONNECTDATA* m_pConData;
private:
	HANDLE m_hFile;
		
	static DWORD WINAPI CommThread(LPVOID pParam);
	HANDLE			    m_hThread;	    	//接收辅助线程
	HANDLE				m_hEventCloseThread;//关闭串口辅助线程
	BOOL				m_bOpened;			//串口是否打开
};


#endif // __SERIALPORT_H
