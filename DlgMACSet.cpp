// DlgMACSet.cpp : implementation file
//

#include "stdafx.h"
#include "FkSet.h"
#include "DlgMACSet.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMACSet dialog


CDlgMACSet::CDlgMACSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMACSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMACSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMACSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMACSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMACSet, CDialog)
	//{{AFX_MSG_MAP(CDlgMACSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMACSet message handlers
/*
����GetPrivateProfileInt() ��˽�г�ʼ���ļ���ȡ������ֵ
����GetPrivateProfileString() ��˽�г�ʼ���ļ���ȡ�ַ�����ֵ
����GetProfileInt ��win.ini ��ȡ����ֵ
����GetProfileString ��win.ini ��ȡ�ַ���ֵ
����WritePrivateProfileString д�ַ�����˽�г�ʼ���ļ�
����WriteProfileString д�ַ�����win.ini
  
	
*/

void CDlgMACSet::LoadPara() 
{
	char path[256];	
	GetModulePath(path);
	strcat(path,"setting.ini");
	char szMAC[13]={0};	
	char szTermAddr[13]={0};	
	int Result ;
//	Result	= GetPrivateProfileInt("SET_MAC_ADDR","START_TERM_ADDR",4369,path);
	//�Ƿ�BCD��
	Result	= GetPrivateProfileInt("SET_MAC_ADDR","ADDR_IS_BCD",0,path);

	m_para.bAddrIsBCD=Result?TRUE:FALSE;
	if(	m_para.bAddrIsBCD)
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(1);	
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);	
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(1);
	}


	GetPrivateProfileString("SET_MAC_ADDR","START_TERM_ADDR","111104369",m_para.szStratTermAddr,13,path);
	SetDlgItemText(IDC_EDIT1,m_para.szStratTermAddr);

	m_para.dwTermNumber	= GetPrivateProfileInt("SET_MAC_ADDR","TERM_NUMBER",1,path);
	SetDlgItemInt(IDC_EDIT8,m_para.dwTermNumber);	

	GetPrivateProfileString("SET_MAC_ADDR","START_MAC_ADDR","3C081E000001",m_para.szStratMAC,13,path);
	SetDlgItemText(IDC_EDIT10,m_para.szStratMAC);
}

void CDlgMACSet::SavePara() 
{
	char path[256];	
	GetModulePath(path);
	strcat(path,"setting.ini");
	char szTemp[13]={0};	
	char szTermAddr[13]={0};	

	//	Result	= GetPrivateProfileInt("SET_MAC_ADDR","START_TERM_ADDR",4369,path);
	//�Ƿ�BCD��
	
	if(	((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck())
	{
		m_para.bAddrIsBCD=TRUE;
	}
	else
	{
		m_para.bAddrIsBCD=FALSE;
	}
	sprintf(szTemp,"%d",m_para.bAddrIsBCD);
	WritePrivateProfileString("SET_MAC_ADDR","ADDR_IS_BCD",szTemp,path);	

	szTemp[0]=0;
	GetDlgItemText(IDC_EDIT1,m_para.szStratTermAddr,12);
	WritePrivateProfileString("SET_MAC_ADDR","START_TERM_ADDR",m_para.szStratTermAddr,path);

	
	szTemp[0]=0;
	GetDlgItemText(IDC_EDIT8,szTemp,10);
	m_para.dwTermNumber =atoi(szTemp);
	WritePrivateProfileString("SET_MAC_ADDR","TERM_NUMBER",szTemp,path);

	szTemp[0]=0;
	GetDlgItemText(IDC_EDIT10,m_para.szStratMAC,13);
	m_para.szStratMAC[12]=0;
	WritePrivateProfileString("SET_MAC_ADDR","START_MAC_ADDR",m_para.szStratMAC,path);
	
}

void CDlgMACSet::OnOK() 
{
	// TODO: Add extra validation here
	SavePara();
	CDialog::OnOK();
}

BOOL CDlgMACSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadPara();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
