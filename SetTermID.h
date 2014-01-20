#if !defined(AFX_SETTERMID_H__8A40AB40_D320_4230_832B_3DF7B7632C2F__INCLUDED_)
#define AFX_SETTERMID_H__8A40AB40_D320_4230_832B_3DF7B7632C2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetTermID.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetTermID dialog

class CSetTermID : public CDialog
{
// Construction
public:
	char m_szTermID[10];
	CSetTermID(CWnd* pParent = NULL);   // standard constructor
	void LoadPara() ;
	void SavePara() ;
// Dialog Data
	//{{AFX_DATA(CSetTermID)
	enum { IDD = IDD_DIALOG7 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetTermID)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetTermID)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTERMID_H__8A40AB40_D320_4230_832B_3DF7B7632C2F__INCLUDED_)
