// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__32711418_E2ED_4CEC_82A5_014F159CED82__INCLUDED_)
#define AFX_STDAFX_H__32711418_E2ED_4CEC_82A5_014F159CED82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

typedef struct tagIPPORTINFO
{
	unsigned char	m_strEndIP[16]; //主用Ip地址
	unsigned short  m_nEndPort;		//主用端口号
	unsigned char	m_strEndIP2[16];//备用Ip地址
	unsigned short  m_nEndPort2;	//备用端口号
	unsigned char   m_nEndAPN[50];	//APN
}IPPORTINFO;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__32711418_E2ED_4CEC_82A5_014F159CED82__INCLUDED_)
