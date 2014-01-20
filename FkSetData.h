// FkSetData.h: interface for the CFkSetData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FKSETDATA_H__3493AC7C_C701_4D05_A577_79AF3A79C4EA__INCLUDED_)
#define AFX_FKSETDATA_H__3493AC7C_C701_4D05_A577_79AF3A79C4EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define		BUFFER_SIZE				5120

// 通道设置
typedef struct tagCOMMPORTCFG { 
	int m_cDevice; // device no.
	
	int m_icommno;   // 串口号
	int m_iBaud;	 //波特率 
	BYTE m_cbDataBit;	 //数据位 8位:0/7位:1/6位:2/
	BYTE m_cbStopBit;	 //停止位 1位:0/1.5位:1/2位:2/
	BYTE m_cbParity;	 //奇偶校验 无校验:0/奇校验:1/偶校验:2/
}COMMPORTCFG;

// 串口发送接收数据
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
//	LMUCOMM	CommInfo;		//通讯参数
	LMUADDR		Address;			//终端地址-终端返回
	unsigned short		AreaCode;	//终端行政区码
	unsigned short		UintAddr[9];	//终端地址
	unsigned short		DA;		//终端交采所属测量点号
		//1-8, 表示8个组地址, 0, 表示单地址
	unsigned char		AddrFlag;	//地址使用标志
		//bit0, function of using address, 0-single, 1-group
		//bit7, address group valid, 0-invalid, 1-valid
	unsigned char		HostPass[4*4];	//主站密码
	unsigned char		FCB;
	unsigned char		CtrlCode;		//本次主站的控制字
	unsigned char		AppFC;		//本次主站的应用层功能码
	unsigned char		PSEQ;
	unsigned char		RSEQ;
	unsigned char		Timeout;		//启动站延时, min
	unsigned char		TimeLabel[7];	//启动站时间标签
}LMUACKINFO;

class CFkSetData  
{
public:
	CFkSetData();
	virtual ~CFkSetData();

};

#endif // !defined(AFX_FKSETDATA_H__3493AC7C_C701_4D05_A577_79AF3A79C4EA__INCLUDED_)
