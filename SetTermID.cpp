// SetTermID.cpp : implementation file
//

#include "stdafx.h"
#include "FkSet.h"
#include "SetTermID.h"
#include "global.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetTermID dialog


CSetTermID::CSetTermID(CWnd* pParent /*=NULL*/)
	: CDialog(CSetTermID::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetTermID)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSetTermID::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTermID)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetTermID, CDialog)
	//{{AFX_MSG_MAP(CSetTermID)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetTermID message handlers

void CSetTermID::LoadPara() 
{
	char path[256];	
	GetModulePath(path);
	strcat(path,"setting.ini");


	GetPrivateProfileString("SET_TERM_ID","LAST_TERM_ID","12345678",m_szTermID,9,path);
	m_szTermID[8]=0;
	SetDlgItemText(IDC_EDIT1,m_szTermID);

}

void CSetTermID::SavePara() 
{
	char path[256];	
	GetModulePath(path);
	strcat(path,"setting.ini");

	GetDlgItemText(IDC_EDIT1,m_szTermID,9);
	m_szTermID[8]=0;
	WritePrivateProfileString("SET_TERM_ID","LAST_TERM_ID",m_szTermID,path);
	
}
void CSetTermID::OnOK() 
{
	// TODO: Add extra validation here
	SavePara() ;
	CDialog::OnOK();
}

BOOL CSetTermID::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadPara() ;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetTermID::OnCancel() 
{
	// TODO: Add extra cleanup here
	SavePara() ;
	CDialog::OnCancel();
}

void CSetTermID::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	SavePara() ;
	CDialog::OnClose();
}
