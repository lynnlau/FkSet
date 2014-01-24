// FkSetDlg.h : header file
//

#if !defined(AFX_FKSETDLG_H__16BF1361_523D_4D17_8A2F_C757BFAF1F46__INCLUDED_)
#define AFX_FKSETDLG_H__16BF1361_523D_4D17_8A2F_C757BFAF1F46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EditListCtrl.h"
#include "SerialPort.h"
#include "DlgMACSet.h"
#include "DlgCDMASeria.h"
/////////////////////////////////////////////////////////////////////////////
// CFkSetDlg dialog

class CFkSetDlg : public CDialog
{
// Construction
public:
	CFkSetDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFkSetDlg)
	enum { IDD = IDD_FKSET_DIALOG };
	CComboBox	m_PluseNum;
	CComboBox	m_selParity;
	CComboBox	m_sel4853;
	CComboBox	m_sel4852;
	CComboBox	m_sel4851;
	CEdit	m_ctlResult;
	CComboBox	m_ctlB;
	CComboBox	m_ctlCom;
	CEditListCtrl	m_AddressList;
	int		m_nAcPort;
	int		m_n4851Port;
	int		m_n4852Port;
	CString	m_editmt1;
	CString	m_editmt2;
	//CString	m_editmt3;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFkSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK();
	virtual void OnCancel();


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFkSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnClickOk();
	afx_msg void OnClose();
	afx_msg void OnOpenCom();
	afx_msg void OnCloseCom();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnButton10();
	afx_msg void OnButton11();
	afx_msg void OnButton12();
	afx_msg void OnButton13();
	afx_msg void OnButton14();
	afx_msg void OnButton15();
	afx_msg void OnButton16();
	afx_msg void OnCheck1();
	afx_msg void OnButton17();
	afx_msg void OnButton18();
	afx_msg void OnButton19();
	afx_msg void OnButton20();
	afx_msg void OnButton21();
	afx_msg void OnRadioFk();
	afx_msg void OnRadio698();
	afx_msg void OnRadioPwlong();
	afx_msg void OnRadioPwsht();
	afx_msg void OnButton22();
	afx_msg void OnButton23();
	afx_msg void OnButton24();
	afx_msg void OnButton26();
	afx_msg void OnButton27();
	afx_msg void OnButton29();
	afx_msg void OnButton30();
	afx_msg void OnButton28();
	afx_msg void OnButtonSetMac();
	afx_msg void OnButtonRdMac();
	afx_msg void OnButtonRdVer();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReturnList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDoor();
	afx_msg void OnButton33();
	afx_msg void OnButton36();
	afx_msg void OnButtonClrAC();
	afx_msg void OnButton37();
	afx_msg void OnButtonSetid();
	afx_msg void OnButton38();
	afx_msg void OnButton25();
	afx_msg void OnButtonUpmodem();
	afx_msg void OnEsam();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CSerialPort				m_port;
	ACCONNECTDATA			m_conData;
	CDlgMACSet m_macdlg;
	CDlgCDMASeria m_CDMAsERIA;
	char compareCHAR[8];
	void ExitFkSetData();
	void SetButtonState();
	void SetCheckTmu(unsigned char cType, unsigned char* pReport, unsigned short len, BOOL bPwdLong=FALSE,void* param1 = NULL, void* param2 = NULL, void* param3 = NULL);
	unsigned char GetBcd(unsigned char uSrc);
	int AnalyticData(unsigned char* pReport, unsigned short& nReportLen,int nItem);

	void find_SOH(unsigned char* pReport, unsigned short& nReportLen,int i);
	void RefreshNum();
	void EnableAllButton(BOOL b);
	void AddStr2Edit(const CString& str);
	int AnalyticPortinfo(unsigned char* pBuf, unsigned short nAppLen);
	int AnalyticAFN10(unsigned char* pBuf, unsigned short nAppLen);
	void LoadParamFile();
	void SaveParamFile();
	int GetMeteraddr(int index,char *pAddr) ;
	int MakeSetMacFrame(BYTE *pReport,CString &strAddr);
	void MakeSerialFrame();
	int MakeSetSERFrame(BYTE *pReport,CString &strAddr);
	int MakeSetSERFrametwo(BYTE *pReport,CString &strAddr);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FKSETDLG_H__16BF1361_523D_4D17_8A2F_C757BFAF1F46__INCLUDED_)
