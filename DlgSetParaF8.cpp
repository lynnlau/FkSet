// DlgSetParaF8.cpp : implementation file
//

#include "stdafx.h"
#include "FkSet.h"
#include "DlgSetParaF8.h"
#include "Global.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetParaF8 dialog


CDlgSetParaF8::CDlgSetParaF8(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetParaF8::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetParaF8)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSetParaF8::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetParaF8)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetParaF8, CDialog)
	//{{AFX_MSG_MAP(CDlgSetParaF8)
	ON_BN_CLICKED(IDOK, OnSet)
	ON_BN_CLICKED(IDC_BUTTON1, OnRead)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetParaF8 message handlers
BOOL CDlgSetParaF8::OnInitDialog()
{
	CDialog::OnInitDialog();

	LoadParamFile();

	((CComboBox*)GetDlgItem(IDC_COMBO2))->SetCurSel(m_para.nTypeOnLine-1); 
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(m_para.nTypeClientSvr); 
	((CComboBox*)GetDlgItem(IDC_COMBO3))->SetCurSel(m_para.nTypeTCPUDP);

	SetDlgItemInt(IDC_EDIT1,m_para.nIntervalForever);
	SetDlgItemInt(IDC_EDIT2,m_para.nRetryPassive);
	SetDlgItemInt(IDC_EDIT3,m_para.nIntervalPassive);

	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck((m_para.nFlgOnline>>0)&1); 
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck((m_para.nFlgOnline>>1)&1);
	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck((m_para.nFlgOnline>>2)&1);
	((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck((m_para.nFlgOnline>>3)&1);
	((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck((m_para.nFlgOnline>>4)&1);
	((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck((m_para.nFlgOnline>>5)&1);
	((CButton*)GetDlgItem(IDC_CHECK7))->SetCheck((m_para.nFlgOnline>>6)&1);
	((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck((m_para.nFlgOnline>>7)&1);
	((CButton*)GetDlgItem(IDC_CHECK9))->SetCheck((m_para.nFlgOnline>>8)&1);
	((CButton*)GetDlgItem(IDC_CHECK10))->SetCheck((m_para.nFlgOnline>>9)&1);
	((CButton*)GetDlgItem(IDC_CHECK11))->SetCheck((m_para.nFlgOnline>>10)&1);
	((CButton*)GetDlgItem(IDC_CHECK12))->SetCheck((m_para.nFlgOnline>>11)&1);
	((CButton*)GetDlgItem(IDC_CHECK13))->SetCheck((m_para.nFlgOnline>>12)&1);
	((CButton*)GetDlgItem(IDC_CHECK14))->SetCheck((m_para.nFlgOnline>>13)&1);
	((CButton*)GetDlgItem(IDC_CHECK15))->SetCheck((m_para.nFlgOnline>>14)&1);
	((CButton*)GetDlgItem(IDC_CHECK16))->SetCheck((m_para.nFlgOnline>>15)&1);
	((CButton*)GetDlgItem(IDC_CHECK17))->SetCheck((m_para.nFlgOnline>>16)&1);
	((CButton*)GetDlgItem(IDC_CHECK18))->SetCheck((m_para.nFlgOnline>>17)&1);
	((CButton*)GetDlgItem(IDC_CHECK19))->SetCheck((m_para.nFlgOnline>>18)&1);
	((CButton*)GetDlgItem(IDC_CHECK20))->SetCheck((m_para.nFlgOnline>>19)&1);
	((CButton*)GetDlgItem(IDC_CHECK21))->SetCheck((m_para.nFlgOnline>>20)&1);
	((CButton*)GetDlgItem(IDC_CHECK22))->SetCheck((m_para.nFlgOnline>>21)&1);
	((CButton*)GetDlgItem(IDC_CHECK23))->SetCheck((m_para.nFlgOnline>>22)&1);
	((CButton*)GetDlgItem(IDC_CHECK24))->SetCheck((m_para.nFlgOnline>>23)&1);
	
	return TRUE;
}




BOOL CDlgSetParaF8::LoadParamFile()
{
	CFile f;
	CFileException e;
	char path[256];
	
	GetModulePath(path);
	strcat(path,"\\ParamF8.cfg");
	
	if( !f.Open( path,CFile::modeReadWrite, &e ) )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
//Ä¬ÈÏ²ÎÊý
		m_para.nTypeOnLine=1; //
		m_para.nType0=0; 
		m_para.nTypeClientSvr=1;
		m_para.nType1=0; 	
		m_para.nTypeTCPUDP=0; 
		m_para.nIntervalForever=3600;
		m_para.nRetryPassive=3;
		m_para.nIntervalPassive=1;
		m_para.nFlgOnline = 0xFFFFFF;

		return FALSE;
	}
	
	f.Read(&m_para,sizeof(F8Para));	
	f.Close();
	
	return TRUE;
}
BOOL CDlgSetParaF8::SaveParamFile()
{
	CFile f;
	CFileException e;
	char path[256];
	CString strTmp;
	
	GetModulePath(path);
	strcat(path,"\\ParamF8.cfg");
	
	if( !f.Open( path, CFile::modeCreate | CFile::modeReadWrite, &e ) )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		return FALSE;
	}

	m_para.nTypeOnLine = ((CComboBox*)GetDlgItem(IDC_COMBO2))->GetCurSel()+1; 
	m_para.nTypeClientSvr = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel(); 
	m_para.nTypeTCPUDP = ((CComboBox*)GetDlgItem(IDC_COMBO3))->GetCurSel();
	
	m_para.nIntervalForever=GetDlgItemInt(IDC_EDIT1);
	m_para.nRetryPassive=GetDlgItemInt(IDC_EDIT2);
	m_para.nIntervalPassive=GetDlgItemInt(IDC_EDIT3);
	
	int nIDs[]={
		IDC_CHECK1,
		IDC_CHECK2,
		IDC_CHECK3,
		IDC_CHECK4,
		IDC_CHECK5,
		IDC_CHECK6,
		IDC_CHECK7,
		IDC_CHECK8,
		IDC_CHECK9,
		IDC_CHECK10,
		IDC_CHECK11,
		IDC_CHECK12,
		IDC_CHECK13,
		IDC_CHECK14,
		IDC_CHECK15,
		IDC_CHECK16,
		IDC_CHECK17,
		IDC_CHECK18,
		IDC_CHECK19,
		IDC_CHECK20,
		IDC_CHECK21,
		IDC_CHECK22,
		IDC_CHECK23,
		IDC_CHECK24,
		};
	int i;
	m_para.nFlgOnline = 0;
	for(i = 0; i < 24; i++)
	{
		int nChk = ((CButton*)GetDlgItem(nIDs[i]))->GetCheck();
		if(nChk)
		{
			m_para.nFlgOnline |= (1 << i);
		}	
	}

	f.Write(&m_para,sizeof(F8Para));
	return TRUE;
}

void CDlgSetParaF8::OnCancel() 
{
	// TODO: Add extra cleanup here
	SaveParamFile();
	CDialog::OnCancel();
}

void CDlgSetParaF8::OnRead() 
{
	// TODO: Add your control notification handler code here
	SaveParamFile();
	m_bSet = FALSE;
	OnOK();	
}
void CDlgSetParaF8::OnSet() 
{
	// TODO: Add your control notification handler code here
	SaveParamFile();
	m_bSet = TRUE;
	OnOK();
}
