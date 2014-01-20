#if !defined(AFX_DLGSET_H__CB89FB71_7B37_49B0_B90F_53612C17E405__INCLUDED_)
#define AFX_DLGSET_H__CB89FB71_7B37_49B0_B90F_53612C17E405__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGSET.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLGSET dialog
extern unsigned char g_mac[6];
class CDLGSET : public CDialog
{
// Construction
public:
	CDLGSET(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGSET)
	enum { IDD = IDD_DIALOGMAC };
	CString	m_mac1;
	CString	m_mac2;
	CString	m_mac3;
	CString	m_mac4;
	CString	m_mac5;
	CString	m_mac6;
	//}}AFX_DATA
	unsigned char m_strMac[10];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGSET)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGSET)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	BOOL LoadParamFile();
	BOOL SaveParamFile();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSET_H__CB89FB71_7B37_49B0_B90F_53612C17E405__INCLUDED_)
