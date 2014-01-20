// FkSet.h : main header file for the FKSET application
//

#if !defined(AFX_FKSET_H__F557A608_137C_4A48_8D4A_50EEBF3F5277__INCLUDED_)
#define AFX_FKSET_H__F557A608_137C_4A48_8D4A_50EEBF3F5277__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFkSetApp:
// See FkSet.cpp for the implementation of this class
//

class CFkSetApp : public CWinApp
{
public:
	CFkSetApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFkSetApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFkSetApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FKSET_H__F557A608_137C_4A48_8D4A_50EEBF3F5277__INCLUDED_)
