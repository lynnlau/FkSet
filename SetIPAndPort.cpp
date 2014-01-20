// SetIPAndPort.cpp : implementation file
//

#include "stdafx.h"
#include "FkSet.h"
#include "SetIPAndPort.h"
#include "global.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetIPAndPort dialog


CSetIPAndPort::CSetIPAndPort(CWnd* pParent /*=NULL*/)
	: CDialog(CSetIPAndPort::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetIPAndPort)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSetIPAndPort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetIPAndPort)
	DDX_Control(pDX, IDC_EDIT_APN, m_ctlEndAPN);
	DDX_Control(pDX, IDC_EDIT_PORT2, m_ctlEndPort2);
	DDX_Control(pDX, IDC_EDIT_IP2, m_ctlEndIP2);
	DDX_Control(pDX, IDC_EDIT_PORT1, m_ctlEndPort);
	DDX_Control(pDX, IDC_EDIT_IP1, m_ctlEndIP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetIPAndPort, CDialog)
	//{{AFX_MSG_MAP(CSetIPAndPort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetIPAndPort message handlers

void CSetIPAndPort::OnOK() 
{
	CString strTmp;
	sockaddr_in sa;
	unsigned char uc1,uc2,uc3,uc4;
	

	//获得主用IP，Port
	m_ctlEndIP.GetWindowText(strTmp);		
	sa.sin_addr.s_addr=inet_addr(strTmp);
	
	uc1 = sa.sin_addr.s_net;
	uc2 = sa.sin_addr.s_host;
	uc3 = sa.sin_addr.s_lh;
	uc4 = sa.sin_addr.s_impno;
	if(uc1 == 0xff && uc2 == 0xff && uc3 == 0xff && uc4 == 0xff)
	{
		AfxMessageBox("IP格式不正确。");
		m_ctlEndIP.SetFocus();
		return;
	}
	strcpy((char*)m_data.m_strEndIP , strTmp);

	m_ctlEndPort.GetWindowText(strTmp);
	m_data.m_nEndPort = atoi(strTmp);

	//获得备用IP，Port
	m_ctlEndIP2.GetWindowText(strTmp);		
	sa.sin_addr.s_addr=inet_addr(strTmp);
	
	uc1 = sa.sin_addr.s_net;
	uc2 = sa.sin_addr.s_host;
	uc3 = sa.sin_addr.s_lh;
	uc4 = sa.sin_addr.s_impno;
	if(uc1 == 0xff && uc2 == 0xff && uc3 == 0xff && uc4 == 0xff)
	{
		AfxMessageBox("IP格式不正确。");
		m_ctlEndIP2.SetFocus();
		return ;
	}
	strcpy((char*)m_data.m_strEndIP2 , strTmp);

	m_ctlEndPort2.GetWindowText(strTmp);
	m_data.m_nEndPort2 = atoi(strTmp);

	m_ctlEndAPN.GetWindowText(strTmp);
	memset(m_data.m_nEndAPN,0,sizeof(m_data.m_nEndAPN));
	memcpy((char*)m_data.m_nEndAPN,strTmp.GetBuffer(0),min(16,strTmp.GetLength()));
	//strcpy((char*)m_data.m_nEndAPN,strTmp.Mid(0,15));
		
	SaveParamFile();
	CDialog::OnOK();
}

BOOL CSetIPAndPort::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(LoadParamFile())
	{
		CString strTmp;
		m_ctlEndIP.SetWindowText(CString(m_data.m_strEndIP));
		strTmp.Format("%d",m_data.m_nEndPort);
		m_ctlEndPort.SetWindowText(strTmp);
		m_ctlEndIP2.SetWindowText(CString(m_data.m_strEndIP2));
		strTmp.Format("%d",m_data.m_nEndPort2);
		m_ctlEndPort2.SetWindowText(strTmp);
		m_ctlEndAPN.SetWindowText(CString(m_data.m_nEndAPN));
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSetIPAndPort::LoadParamFile()
{
	CFile f;
	CFileException e;
	char path[256];

	GetModulePath(path);
	strcat(path,"\\ParamIp.cfg");

	if( !f.Open( path,CFile::modeReadWrite, &e ) )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		return FALSE;
	}

	f.Read(&m_data,sizeof(IPPORTINFO));	
	f.Close();
	
	return TRUE;
}
BOOL CSetIPAndPort::SaveParamFile()
{
	CFile f;
	CFileException e;
	char path[256];
	CString strTmp;

	GetModulePath(path);
	strcat(path,"\\ParamIp.cfg");

	if( !f.Open( path, CFile::modeCreate | CFile::modeReadWrite, &e ) )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		return FALSE;
	}
	

	f.Write(&m_data,sizeof(IPPORTINFO));
	f.Close();

	return TRUE;
}
