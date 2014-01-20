#if !defined(AFX_DLGMACSET_H__73723742_2D36_4BDC_B8FA_5537F285F5EE__INCLUDED_)
#define AFX_DLGMACSET_H__73723742_2D36_4BDC_B8FA_5537F285F5EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMACSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMACSet dialog

typedef struct tagMACInfo
{
	char szStratTermAddr[13];	
	DWORD dwStratTermAddr;	
	DWORD dwTermNumber;//终端数量
	CHAR szStratMAC[13];//起始MAC地址
	DWORD dwStratMAC;//起始MAC地址的16进制形式
	BOOL bAddrIsBCD;
}MACParaInfo;

class CDlgMACSet : public CDialog
{
// Construction
public:
	CDlgMACSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMACSet)
	enum { IDD = IDD_DIALOG6 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMACSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMACSet)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	MACParaInfo m_para;
	void LoadPara();
	void SavePara();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMACSET_H__73723742_2D36_4BDC_B8FA_5537F285F5EE__INCLUDED_)
