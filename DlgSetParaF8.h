#if !defined(AFX_DLGSETPARAF8_H__4620FDAE_6FE0_47C2_9329_EDFC9C057E2F__INCLUDED_)
#define AFX_DLGSETPARAF8_H__4620FDAE_6FE0_47C2_9329_EDFC9C057E2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetParaF8.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSetParaF8 dialog
typedef struct tagF8Para
{
	BYTE nTypeOnLine:2; //
	BYTE nType0:2; 
	BYTE nTypeClientSvr:2;
	BYTE nType1:1; 	
	BYTE nTypeTCPUDP:1; 
	WORD nIntervalForever;
	BYTE nRetryPassive;
	BYTE nIntervalPassive;
	DWORD nFlgOnline;
}F8Para;
class CDlgSetParaF8 : public CDialog
{
// Construction
public:
	CDlgSetParaF8(CWnd* pParent = NULL);   // standard constructor
	F8Para m_para;
	BOOL m_bSet;
	BOOL LoadParamFile();
	BOOL SaveParamFile();
// Dialog Data
	//{{AFX_DATA(CDlgSetParaF8)
	enum { IDD = IDD_DIALOG5 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetParaF8)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetParaF8)
		virtual BOOL OnInitDialog();
	afx_msg void OnSet();
	virtual void OnCancel();
	afx_msg void OnRead();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETPARAF8_H__4620FDAE_6FE0_47C2_9329_EDFC9C057E2F__INCLUDED_)
