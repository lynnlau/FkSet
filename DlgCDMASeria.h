#if !defined(AFX_DLGCDMASERIA_H__AA1EECB5_2E9C_4BC1_B2D5_A67D193C7D14__INCLUDED_)
#define AFX_DLGCDMASERIA_H__AA1EECB5_2E9C_4BC1_B2D5_A67D193C7D14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCDMASeria.h : header file
//
#include "global.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgCDMASeria dialog

class CDlgCDMASeria : public CDialog
{
// Construction
public:
	CDlgCDMASeria(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDlgCDMASeria)
	enum { IDD = IDD_DIALOG8 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCDMASeria)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

	char m_szNUM[13];
	char m_szSerisID[12];
	char m_szTermID[12];
	char m_szTermCDMA[12];
	char m_szTermpass[12];
	char m_sz8[9];
	char m_sz4[5];
	DWORD dwTermNumber;//÷’∂À ˝¡ø
	void SavePara() ;
		void LoadPara() ;
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCDMASeria)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCDMASERIA_H__AA1EECB5_2E9C_4BC1_B2D5_A67D193C7D14__INCLUDED_)
