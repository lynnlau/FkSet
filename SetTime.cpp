// SetTime.cpp : implementation file
//

#include "stdafx.h"
#include "FkSet.h"
#include "SetTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetTime dialog


CSetTime::CSetTime(CWnd* pParent /*=NULL*/)
	: CDialog(CSetTime::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetTime)
	m_nMin = -1;
	//}}AFX_DATA_INIT
}


void CSetTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTime)
	DDX_Control(pDX, IDC_COMBO1, m_com);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nMin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetTime, CDialog)
	//{{AFX_MSG_MAP(CSetTime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetTime message handlers

BOOL CSetTime::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_com.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetTime::OnOK() 
{	
	CString strTmp;
	m_com.GetWindowText(strTmp);
	
	
	CDialog::OnOK();
	m_nMin = atoi(strTmp);	
}
