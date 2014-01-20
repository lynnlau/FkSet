// DlgCDMAPara.cpp : implementation file
//

#include "stdafx.h"
#include "FkSet.h"
#include "DlgCDMAPara.h"
#include "global.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCDMAPara dialog


CDlgCDMAPara::CDlgCDMAPara(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCDMAPara::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCDMAPara)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCDMAPara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCDMAPara)
	DDX_Control(pDX, IDC_EDIT_CDMAUSR, m_usr);
	DDX_Control(pDX, IDC_EDIT_CDMAPWD, m_pwd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCDMAPara, CDialog)
	//{{AFX_MSG_MAP(CDlgCDMAPara)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCDMAPara message handlers

void CDlgCDMAPara::OnOK() 
{
	// TODO: Add extra validation here

	SaveParamFile();
	CDialog::OnOK();
}

BOOL CDlgCDMAPara::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(LoadParamFile())
	{
		m_usr.SetWindowText(m_data.usr);
		m_pwd.SetWindowText(m_data.pwd);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgCDMAPara::LoadParamFile()
{
	CFile f;
	CFileException e;
	char path[256];

	GetModulePath(path);
	strcat(path,"\\ParamCDMA.cfg");

	if( !f.Open( path,CFile::modeReadWrite, &e ) )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		return FALSE;
	}

	f.Read(&m_data,sizeof(CDMAPara));	
	f.Close();
	
	return TRUE;
}
BOOL CDlgCDMAPara::SaveParamFile()
{
	CFile f;
	CFileException e;
	char path[256];
	CString strTmp;

	GetModulePath(path);
	strcat(path,"\\ParamCDMA.cfg");

	if( !f.Open( path, CFile::modeCreate | CFile::modeReadWrite, &e ) )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		return FALSE;
	}
	m_usr.GetWindowText(strTmp);
	strncpy(m_data.usr,strTmp,32);

	m_pwd.GetWindowText(strTmp);
	strncpy(m_data.pwd,strTmp,32);	

	f.Write(&m_data,sizeof(CDMAPara));
	f.Close();

	return TRUE;
}
