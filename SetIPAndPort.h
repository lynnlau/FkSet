#if !defined(AFX_SETIPANDPORT_H__B7E7D4D7_59D5_456A_B14B_8C1FD664D1D1__INCLUDED_)
#define AFX_SETIPANDPORT_H__B7E7D4D7_59D5_456A_B14B_8C1FD664D1D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetIPAndPort.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetIPAndPort dialog

class CSetIPAndPort : public CDialog
{
// Construction
public:
	CSetIPAndPort(CWnd* pParent = NULL);   // standard constructor

	IPPORTINFO m_data;
// Dialog Data
	//{{AFX_DATA(CSetIPAndPort)
	enum { IDD = IDD_DIALOG3 };
	CEdit	m_ctlEndAPN;
	CEdit	m_ctlEndPort2;
	CEdit	m_ctlEndIP2;
	CEdit	m_ctlEndPort;
	CEdit	m_ctlEndIP;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetIPAndPort)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetIPAndPort)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL LoadParamFile();
	BOOL SaveParamFile();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETIPANDPORT_H__B7E7D4D7_59D5_456A_B14B_8C1FD664D1D1__INCLUDED_)
