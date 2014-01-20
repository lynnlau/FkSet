#if !defined(AFX_SETALARM_H__6D69EC22_B817_4C18_A912_E8B004465176__INCLUDED_)
#define AFX_SETALARM_H__6D69EC22_B817_4C18_A912_E8B004465176__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetAlarm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetAlarm dialog

class CSetAlarm : public CDialog
{
// Construction
public:
	CSetAlarm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetAlarm)
	enum { IDD = IDD_DIALOG2 };
	CComboBox	m_comLimitTime;
	CComboBox	m_comAlarmDelay;
	CComboBox	m_com;
	int		m_nAlarm;
	int		m_nDelay;
	int		m_nTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetAlarm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetAlarm)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETALARM_H__6D69EC22_B817_4C18_A912_E8B004465176__INCLUDED_)
