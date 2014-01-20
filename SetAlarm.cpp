// SetAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "FkSet.h"
#include "SetAlarm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetAlarm dialog


CSetAlarm::CSetAlarm(CWnd* pParent /*=NULL*/)
	: CDialog(CSetAlarm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetAlarm)
	m_nAlarm = -1;
	m_nDelay = -1;
	m_nTime = -1;
	//}}AFX_DATA_INIT
}


void CSetAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetAlarm)
	DDX_Control(pDX, IDC_COMBO3, m_comLimitTime);
	DDX_Control(pDX, IDC_COMBO2, m_comAlarmDelay);
	DDX_Control(pDX, IDC_COMBO1, m_com);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nAlarm);
	DDX_CBIndex(pDX, IDC_COMBO2, m_nDelay);
	DDX_CBIndex(pDX, IDC_COMBO3, m_nTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetAlarm, CDialog)
	//{{AFX_MSG_MAP(CSetAlarm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetAlarm message handlers

BOOL CSetAlarm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_com.SetCurSel(0);
	m_comAlarmDelay.SetCurSel(0);
	m_comLimitTime.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
