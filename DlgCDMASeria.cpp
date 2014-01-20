// DlgCDMASeria.cpp : implementation file
//

#include "stdafx.h"
#include "FkSet.h"
#include "DlgCDMASeria.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCDMASeria dialog


CDlgCDMASeria::CDlgCDMASeria(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCDMASeria::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCDMASeria)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCDMASeria::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCDMASeria)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCDMASeria, CDialog)
	//{{AFX_MSG_MAP(CDlgCDMASeria)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCDMASeria message handlers

void CDlgCDMASeria::OnOK() 
{
	// TODO: Add extra validation here
	SavePara();
	CDialog::OnOK();
}
void CDlgCDMASeria::SavePara() 
{
		
	char path[256];	
	GetModulePath(path);
	strcat(path,"setting.ini");
	
	

	//szTemp[0]=0;
	GetDlgItemText(IDC_EDIT9,m_szNUM,10);
	dwTermNumber =atoi(m_szNUM);
//	WritePrivateProfileString("SET_MAC_ADDR","TERM_NUMBER",szTemp,path);

	GetDlgItemText(IDC_EDIT1,m_szSerisID,13);


	GetDlgItemText(IDC_EDIT8,m_szTermID,10);
	
	//m_sz = 
	memcpy(m_sz4,m_szSerisID,4);m_sz4[4]=0;
	memcpy(m_sz8,m_szSerisID+4,8);m_sz8[8]=0;

	//	memset(m_sz8,m_szSerisID,sizeof(rtuAddress));
	m_szTermID[9]=0;
	WritePrivateProfileString("SET_SERIA","FIRST_SERIA_ID",m_szSerisID,path);
	WritePrivateProfileString("SET_SERIA","NUMBER",m_szNUM,path);
	

	WritePrivateProfileString("SET_TML_ADDR","START_TERM_ADDR",m_szTermID,path);

	
}
void CDlgCDMASeria::LoadPara() 
{
	char path[256];	
	GetModulePath(path);
	strcat(path,"setting.ini");
	char szMAC[13]={0};	
	char szTermAddr[13]={0};	
	int Result ;
//	Result	= GetPrivateProfileInt("SET_MAC_ADDR","START_TERM_ADDR",4369,path);
	//ÊÇ·ñBCDÂë
	Result	= GetPrivateProfileInt("SET_MAC_ADDR","ADDR_IS_BCD",0,path);

	
	


	GetPrivateProfileString("SET_SERIA","FIRST_SERIA_ID","123456789121",m_szSerisID,13,path);
	SetDlgItemText(IDC_EDIT1,m_szSerisID);

	dwTermNumber	= GetPrivateProfileInt("SET_SERIA","NUMBER",1,path);
	SetDlgItemInt(IDC_EDIT9,dwTermNumber);	

	GetPrivateProfileString("SET_TML_ADDR","START_TERM_ADDR","111104369",m_szTermID,10,path);
	SetDlgItemText(IDC_EDIT8,m_szTermID);
}

BOOL CDlgCDMASeria::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadPara() ;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
