#include "stdafx.h"
#include "SerialPort.h"
#include "global.h"

CSerialPort::CSerialPort()
{
	m_hFile=NULL;
	m_hThread=NULL;
	m_hEventCloseThread=NULL;
	m_bOpened = FALSE;	
}

CSerialPort::~CSerialPort()
{
}

BOOL CSerialPort::Init(ACCONNECTDATA* pConData)
{
	_ASSERTE(pConData->m_cfg.m_icommno >=0 && pConData->m_cfg.m_icommno < 255);

	if(m_bOpened)
		return TRUE;

	char str[100];
	sprintf(str,"COM%d",pConData->m_cfg.m_icommno);

	//打开串口
	m_hFile = ::CreateFile(str,
						   GENERIC_READ|GENERIC_WRITE,
						   0,
					       0,
						   OPEN_EXISTING,
						   FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
						   0);
	
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		
		switch (::GetLastError())
		{
		case ERROR_FILE_NOT_FOUND:
			strcat(str," 为无效端口！");
			AfxMessageBox(str);
			break;
		case ERROR_ACCESS_DENIED:
			strcat(str," 已被其他程序打开！");
			AfxMessageBox(str);
			break;
		default:
			strcat(str," 打开失败！未知错误！");
			AfxMessageBox(str);
			break;
		}
		
		m_hFile = NULL;
		return FALSE;
	}
	
	//设置DCB结构
	DCB dcb;
	dcb.DCBlength = sizeof(DCB);
	if (!::GetCommState(m_hFile,&dcb))
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
		return FALSE;
	}
	m_pConData = pConData;

	dcb.BaudRate = m_pConData->m_cfg.m_iBaud;
	dcb.ByteSize = m_pConData->m_cfg.m_cbDataBit;
	dcb.Parity   = m_pConData->m_cfg.m_cbParity;
	dcb.StopBits = m_pConData->m_cfg.m_cbStopBit;
	dcb.fBinary = TRUE;
	dcb.fParity = TRUE;
	if (!::SetCommState(m_hFile,&dcb))	
	{
		
		char cherror[100];
		sprintf(cherror,"COM%d设置错误：\n可能不支持所选波特率，或停止位，或数据位",pConData->m_cfg.m_icommno);
		AfxMessageBox(cherror);
		
		CloseHandle(m_hFile);
		m_hFile = NULL;
		return FALSE;
	}

	//设置过滤事件
	if (!::SetCommMask(m_hFile,EV_RXCHAR))	
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
		return FALSE;
	}

	//设置缓冲区大小
	if (!::SetupComm(m_hFile,BUFFER_SIZE,BUFFER_SIZE))
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
		return FALSE;
	}	
	//设置超时
	COMMTIMEOUTS cto;
	if (!::GetCommTimeouts(m_hFile,&cto))	
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
		return FALSE;
	}
	cto.ReadIntervalTimeout = 20;
	cto.ReadTotalTimeoutConstant = 0;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.WriteTotalTimeoutConstant =0;
	cto.WriteTotalTimeoutMultiplier =0;
	if (!::SetCommTimeouts(m_hFile,&cto))	
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
		return FALSE;
	}
	::ZeroMemory(&dcb, sizeof(DCB));
	::ZeroMemory(&cto, sizeof(COMMTIMEOUTS));
	
	m_hEventCloseThread = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_hEventCloseThread == 0)	
	{
		::CloseHandle(m_hFile);
		m_hFile = NULL;
		return FALSE;
	}
	
	DWORD theadId;
	m_hThread= CreateThread(NULL, 0, CommThread, this, 0, &theadId);
		
	m_bOpened = TRUE;

	return TRUE;
}

DWORD  CSerialPort::CommThread(LPVOID pParam)
{
	CSerialPort *port = (CSerialPort*)pParam;
	
	OVERLAPPED olRead;
	::ZeroMemory(&olRead, sizeof(OVERLAPPED));
	olRead.hEvent=CreateEvent(NULL, TRUE, FALSE, NULL);
	if(olRead.hEvent==NULL)	
		return (UINT)-1;

	DWORD Event = 0;
	DWORD CommEvent = 0;
	DWORD dwError = 0;
	COMSTAT comstat;
	BOOL  bResult = TRUE;
	DWORD length=0;	
	DWORD dMask;

	HANDLE hEventArray[3];
	hEventArray[0] = port->m_hEventCloseThread;
	hEventArray[1] = olRead.hEvent;	
	hEventArray[2] = port->m_pConData->m_sendEvent;

	if (!port->m_hFile)
		return (UINT)-1;

	PurgeComm(port->m_hFile, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	BOOL bError=0;
	for(;;)
	{
		SetCommMask(port->m_hFile,EV_RXCHAR);
		bResult = WaitCommEvent(port->m_hFile, &dMask, &olRead);
		if(!bResult)
		{
			if(GetLastError()!=ERROR_IO_PENDING)	
				bError=0;
		}
		else
		{
			bResult = ClearCommError(port->m_hFile, &dwError, &comstat);
			if (comstat.cbInQue == 0) 
				continue;
		}
		if(bError)	
			break;
		
		Event = WaitForMultipleObjects(3, hEventArray, FALSE, INFINITE);

		switch (Event)
		{
		case 0:
				::ZeroMemory(&comstat, sizeof(COMSTAT));	
				CloseHandle(hEventArray[1]);

				ExitThread(100);
				
				break;
		case 1:	//读数据
				GetCommMask(port->m_hFile, &CommEvent);				
				if (CommEvent & EV_RXCHAR)
				{
					EnterCriticalSection(&port->m_pConData->m_recCS);

					char *pRead = port->m_pConData->m_recBuf + port->m_pConData->m_recLength;
					int nLeft = BUFFER_SIZE - port->m_pConData->m_recLength;
					
					if(nLeft == 0)
					{
						port->m_pConData->m_recLength = 0;
						pRead = port->m_pConData->m_recBuf;
						nLeft = BUFFER_SIZE;
					}
					length = port->ReadData(pRead, &olRead,nLeft);
					port->m_pConData->m_recLength += (WORD)length;
					SetEvent(port->m_pConData->m_recEvent);	

					LeaveCriticalSection(&port->m_pConData->m_recCS);
				}
				break;
		case 2:	//写数据
								
				LogHex("write->",(char*)port->m_pConData->m_sendBuf,port->m_pConData->m_sendLength);

				port->WriteData(port->m_pConData->m_sendBuf, 
								port->m_pConData->m_sendLength);
				port->m_pConData->m_sendLength= 0;	
				ResetEvent(port->m_pConData->m_sendEvent);			
				
				break;
		default:
				bError=1;
				break;
		}
		
		if(bError)	break;
	}
	
	::ZeroMemory(&comstat, sizeof(COMSTAT));	
	CloseHandle(hEventArray[1]);

	return 0;
}

DWORD CSerialPort::ReadData(LPVOID cpRXBuffer, OVERLAPPED* olRead , int size)
{
	BOOL  bResult = TRUE;
	DWORD dwError = 0;
	DWORD dwBytesRead = 0;
	COMSTAT comstat;

	bResult = ClearCommError(m_hFile, &dwError, &comstat);
	if (comstat.cbInQue == 0)	
		return 0;
	
	bResult = ReadFile(m_hFile,cpRXBuffer,size,&dwBytesRead,olRead); 
	if(!bResult)
	{
		if(GetLastError()==ERROR_IO_PENDING)
			GetOverlappedResult(m_hFile,olRead,&dwBytesRead,1);
		else	
			return 0;
	}
	WORD wBytesRead=(WORD)dwBytesRead;
	::ZeroMemory(&comstat, sizeof(COMSTAT));
	return dwBytesRead;
}

DWORD CSerialPort::WriteData(LPVOID lpBuffer, DWORD dwLength)
{
	if(!dwLength)	return 0;
	BOOL bResult=TRUE;
	DWORD length=dwLength;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	OVERLAPPED olWrite;

	if(!m_hFile)
		return 0;

	::ZeroMemory(&olWrite, sizeof(OVERLAPPED));
	olWrite.hEvent=CreateEvent(NULL, TRUE, FALSE, NULL);
	if(olWrite.hEvent==NULL)	
		return 0;
	
	ClearCommError(m_hFile,&dwErrorFlags,&ComStat);
	bResult=WriteFile(m_hFile,lpBuffer,length,&length,&olWrite);

	if(!bResult)
	{
		if(GetLastError()==ERROR_IO_PENDING)
			GetOverlappedResult(m_hFile,&olWrite,&length,TRUE);// 等待
		else	
			length=0;
			
	}
	::ZeroMemory(&ComStat, sizeof(COMSTAT));
	::CloseHandle (olWrite.hEvent);
	return length;
}

BOOL CSerialPort::UnInit()
{
	if(m_hEventCloseThread)
		SetEvent(m_hEventCloseThread);		
	if(m_hThread)
	{
		if(WAIT_TIMEOUT == WaitForSingleObject(m_hThread, 5000))
		{
			DWORD exitCode = 0;
			TerminateThread(m_hThread,exitCode);
		}
	}
		
	if(m_hThread)
		::CloseHandle(m_hThread);
	if(m_hEventCloseThread)
		::CloseHandle(m_hEventCloseThread);
	if (m_hFile)
		::CloseHandle(m_hFile);

	m_bOpened = FALSE;
	m_hThread=NULL;
	m_hEventCloseThread = NULL;
	m_hFile = NULL;
	m_pConData = NULL;
	
	return TRUE;
}
