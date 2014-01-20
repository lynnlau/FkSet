// DLGSET.cpp : implementation file
//

#include "stdafx.h"
#include "FkSet.h"
#include "DLGSET.h"
#include "global.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern unsigned char g_mac[6];
/////////////////////////////////////////////////////////////////////////////
// CDLGSET dialog


CDLGSET::CDLGSET(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGSET::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGSET)
	m_mac1 = _T("");
	m_mac2 = _T("");
	m_mac3 = _T("");
	m_mac4 = _T("");
	m_mac5 = _T("");
	m_mac6 = _T("");
	//}}AFX_DATA_INIT
}


void CDLGSET::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGSET)
	DDX_Text(pDX, IDC_EDIT2, m_mac1);
	DDV_MaxChars(pDX, m_mac1, 2);
	DDX_Text(pDX, IDC_EDIT3, m_mac2);
	DDV_MaxChars(pDX, m_mac2, 2);
	DDX_Text(pDX, IDC_EDIT4, m_mac3);
	DDV_MaxChars(pDX, m_mac3, 2);
	DDX_Text(pDX, IDC_EDIT5, m_mac4);
	DDV_MaxChars(pDX, m_mac4, 2);
	DDX_Text(pDX, IDC_EDIT6, m_mac5);
	DDV_MaxChars(pDX, m_mac5, 2);
	DDX_Text(pDX, IDC_EDIT7, m_mac6);
	DDV_MaxChars(pDX, m_mac6, 2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGSET, CDialog)
	//{{AFX_MSG_MAP(CDLGSET)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGSET message handlers

void CDLGSET::OnOK() 
{
	// TODO: Add extra validation here
	memset(m_strMac,0,sizeof(m_strMac));
#if 0
	CString strTmp;
	strTmp = "";

	GetDlgItem(IDC_EDIT2)->GetWindowText(strTmp);
	sscanf(strTmp,"%02x",&m_strMac[0]);
	
	strTmp = "";
	GetDlgItem(IDC_EDIT3)->GetWindowText(strTmp);
	sscanf(strTmp,"%02x",&m_strMac[1]);
	strTmp = "";
	GetDlgItem(IDC_EDIT4)->GetWindowText(strTmp);
	sscanf(strTmp,"%02x",&m_strMac[2]);
	strTmp = "";
	GetDlgItem(IDC_EDIT5)->GetWindowText(strTmp);
	sscanf(strTmp,"%02x",&m_strMac[3]);
	strTmp = "";
	GetDlgItem(IDC_EDIT6)->GetWindowText(strTmp);
	sscanf(strTmp,"%02x",&m_strMac[4]);
	strTmp = "";
	GetDlgItem(IDC_EDIT7)->GetWindowText(strTmp);
	sscanf(strTmp,"%02x",&m_strMac[5]);
	
	for (int i =0;i<6;i++)
	{
		if (m_strMac[i] == 0x00)
		{
	//		AfxMessageBox("请输入完整的MAC地址");
	//		return;
		}
		g_mac[i] = m_strMac[i];
	}
	strTmp = "";
#endif	
//	strTmp.Format("%02X-%02X-%02X-%02X-%02X-%02X",
//		m_strMac[0],m_strMac[1],m_strMac[2],m_strMac[3],m_strMac[4],m_strMac[5]);
//	AfxMessageBox(strTmp);
	this->UpdateData(TRUE);
	sscanf(m_mac1,"%02x",&m_strMac[0]);
	sscanf(m_mac2,"%02x",&m_strMac[1]);
	sscanf(m_mac3,"%02x",&m_strMac[2]);
	sscanf(m_mac4,"%02x",&m_strMac[3]);
	sscanf(m_mac5,"%02x",&m_strMac[4]);
	sscanf(m_mac6,"%02x",&m_strMac[5]);

	for (int i =0;i<6;i++)
	{
		g_mac[i] = m_strMac[i];
	}

	CFile f;
	CFileException e;
	char path[256];
	CString strTmp;
	
	GetModulePath(path);
	strcat(path,"\\ParamMAC.cfg");
	
	if( !f.Open( path, CFile::modeCreate | CFile::modeReadWrite, &e ) )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		return;
	}

	f.Write(g_mac,6);
	f.Close();





	CDialog::OnOK();
}

BOOL CDLGSET::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CFile f;
	CFileException e;
	char path[256];

	GetModulePath(path);
	strcat(path,"\\ParamMAC.cfg");

	if( !f.Open( path,CFile::modeReadWrite, &e ) )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
	//	return FALSE;
	}
	else
	{
		f.Read(g_mac,6);	
		f.Close();	
	}


	g_mac[0]=0x3c;
	g_mac[1]=0x08;
	g_mac[2]=0x1e;


	m_mac1.Format("%02X",g_mac[0]);
	m_mac2.Format("%02X",g_mac[1]);
	m_mac3.Format("%02X",g_mac[2]);
	m_mac4.Format("%02X",g_mac[3]);
	m_mac5.Format("%02X",g_mac[4]);
	m_mac6.Format("%02X",g_mac[5]);

	this->UpdateData(FALSE);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
