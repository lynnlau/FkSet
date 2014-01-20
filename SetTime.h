#if !defined(AFX_SETTIME_H__5C8E86E6_A959_43E0_8132_1693BEF6A15B__INCLUDED_)
#define AFX_SETTIME_H__5C8E86E6_A959_43E0_8132_1693BEF6A15B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetTime.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetTime dialog

class CSetTime : public CDialog
{
// Construction
public:
	CSetTime(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetTime)
	enum { IDD = IDD_DIALOG1 };
	CComboBox	m_com;
	int		m_nMin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetTime)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTIME_H__5C8E86E6_A959_43E0_8132_1693BEF6A15B__INCLUDED_)
