#if !defined(AFX_DLGCDMAPARA_H__3E7665C5_1308_44E8_90AF_16CE1D3F6878__INCLUDED_)
#define AFX_DLGCDMAPARA_H__3E7665C5_1308_44E8_90AF_16CE1D3F6878__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCDMAPara.h : header file
//
struct CDMAPara {
	char usr[32];
	char pwd[32];
};
/////////////////////////////////////////////////////////////////////////////
// CDlgCDMAPara dialog

class CDlgCDMAPara : public CDialog
{
// Construction
public:
	CDlgCDMAPara(CWnd* pParent = NULL);   // standard constructor
	CDMAPara m_data;
// Dialog Data
	//{{AFX_DATA(CDlgCDMAPara)
	enum { IDD = IDD_DIALOG4 };
	CEdit	m_usr;
	CEdit	m_pwd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCDMAPara)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCDMAPara)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	BOOL LoadParamFile();
	BOOL SaveParamFile();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCDMAPARA_H__3E7665C5_1308_44E8_90AF_16CE1D3F6878__INCLUDED_)
