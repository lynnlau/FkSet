// FkSetData.h: interface for the CFkSetData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FKSETDATA_H__3493AC7C_C701_4D05_A577_79AF3A79C4EA__INCLUDED_)
#define AFX_FKSETDATA_H__3493AC7C_C701_4D05_A577_79AF3A79C4EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define		BUFFER_SIZE				5120

// ͨ������
typedef struct tagCOMMPORTCFG { 
	int m_cDevice; // device no.
	
	int m_icommno;   // ���ں�
	int m_iBaud;	 //������ 
	BYTE m_cbDataBit;	 //����λ 8λ:0/7λ:1/6λ:2/
	BYTE m_cbStopBit;	 //ֹͣλ 1λ:0/1.5λ:1/2λ:2/
	BYTE m_cbParity;	 //��żУ�� ��У��:0/��У��:1/żУ��:2/
}COMMPORTCFG;

// ���ڷ��ͽ�������
typedef struct tagACCONNECTDATA 
{
	COMMPORTCFG		m_cfg;

	char			m_recBuf[BUFFER_SIZE];
	unsigned short	m_recLength;
	HANDLE			m_recEvent;
	CRITICAL_SECTION	m_recCS;
	
	char			m_sendBuf[BUFFER_SIZE];
	unsigned short	m_sendLength;
	HANDLE			m_sendEvent;
	

}ACCONNECTDATA;
typedef struct
{
	unsigned short	AreaCode;
	unsigned short	UnitAddr;
	unsigned char	HostAddr;
}LMUADDR;
typedef struct	
{
//	LMUCOMM	CommInfo;		//ͨѶ����
	LMUADDR		Address;			//�ն˵�ַ-�ն˷���
	unsigned short		AreaCode;	//�ն���������
	unsigned short		UintAddr[9];	//�ն˵�ַ
	unsigned short		DA;		//�ն˽��������������
		//1-8, ��ʾ8�����ַ, 0, ��ʾ����ַ
	unsigned char		AddrFlag;	//��ַʹ�ñ�־
		//bit0, function of using address, 0-single, 1-group
		//bit7, address group valid, 0-invalid, 1-valid
	unsigned char		HostPass[4*4];	//��վ����
	unsigned char		FCB;
	unsigned char		CtrlCode;		//������վ�Ŀ�����
	unsigned char		AppFC;		//������վ��Ӧ�ò㹦����
	unsigned char		PSEQ;
	unsigned char		RSEQ;
	unsigned char		Timeout;		//����վ��ʱ, min
	unsigned char		TimeLabel[7];	//����վʱ���ǩ
}LMUACKINFO;

class CFkSetData  
{
public:
	CFkSetData();
	virtual ~CFkSetData();

};

#endif // !defined(AFX_FKSETDATA_H__3493AC7C_C701_4D05_A577_79AF3A79C4EA__INCLUDED_)
