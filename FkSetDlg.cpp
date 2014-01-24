

#include "stdafx.h"
#include "FkSet.h"
#include "FkSetDlg.h"

#include "global.h"
#include "SetAlarm.h"
#include "SetIPAndPort.h"
#include "DLGSET.h"
#include "DlgCDMAPara.h"
#include "DlgSetParaF8.h"
#include "SetTermID.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		ANALY_RIGHT				1// 解析成功
#define		ANALY_DATA_LESS			2// 解析成功
#define		ANALY_WRONG				3// frame出错

#define		MIN_SERVER_PACKET_LEN	14

// 终端类型
#define		TERMINAL_TYPE_GW698		0 //国网 专变、集中器
#define		TERMINAL_TYPE_FK		1
//---------FK----------
//保电解除
BYTE SendData1[] = {0X68, 0X39, 0X00, 0X39, 0X00, 0X68, 0X4A, 0X11, 0X11, 0X11, 0X11, 0X02, 0X05, 0X70, 0X00, 0X00, 0X01, 
					0X04, 0X55, 0X55, 0XB4, 0X16};

//配置电能表 （一路本地交采、两路电能表1口2口）
BYTE SendData2[] = {0X68, 0X09, 0X01, 0X09, 0X01, 0X68, 0X4a, 0X11, 0X11, 0X16, 0X11, 0X02, 0X04, 0X71, 0X00, 0X00, 0X02, 
					0X01, 0X03, 0X01, 0X01, 0X01, 0X02, 0X01, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 
					0X00, 0X40, 0X02, 0X02, 0X41, 0X01, 0X01, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 
					0X00, 0X40, 0X03, 0X03, 0X42, 0X01, 0X01, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 
					0X00, 0X40, 0X00, 0X00, 0Xe7, 0X16};
//配置脉冲(2路脉冲)
/*
BYTE SendData3[] = {0X68, 0X65, 0X00, 0X65, 0X00, 0X68, 0X4A, 0X11, 0X11, 0X16, 0X11, 0X02, 0X04, 0X73, 0X00, 0X00, 0X04, 
					0X01, 0X02, 0X01, 0X04, 0X00, 0X00, 0X19, 0X02, 0X04, 0X01, 0X00, 0X19, 0X01, 0X00, 0X52, 0X16};
*/
BYTE SendData3[] = {0X68, 0X8D, 0X00, 0X8D, 0X00, 0X68, 0X4A, 0X11, 0X12, 0X05, 0X00, 0X02, 0X04, 0X73, 0X00, 0X00, 0X04, 
					0X01, 0X04, 0X01, 0X04, 0X00, 0X00, 0X19, 0X02, 0X05, 0X00, 0X00, 0X19, 0X03, 0X06, 0X00, 0X00, 0X19, 
					0X04, 0X07, 0X00, 0X00, 0X19, 0X00, 0X00, 0X78, 0X16};

//抄表间隔设置F24
BYTE SendData4[] = {0X68, 0X3d, 0X00, 0X3d, 0X00, 0X68, 0X4a, 0X11, 0X11, 0X19, 0X11, 0X02, 0X04, 0X72, 0X00, 0X00, 0X80, 
					0X02, 0X01, 0X00, 0X00, 0X91, 0X16};
//4路遥信
BYTE SendData5[] = {0X68, 0X45, 0X00, 0X45, 0X00, 0X68, 0X4A, 0X11, 0X11, 0X11, 0X11, 0X02, 0X04, 0X70, 0X00, 0X00, 0X08, 
					0X01, 0X0F, 0X0F, 0X0F, 0X55, 0X55, 0XE4, 0X16};
//总加组
BYTE SendData6[] = {0X68, 0X49, 0X00, 0X49, 0X00, 0X68, 0X4A, 0X11, 0X11, 0X11, 0X11, 0X02, 0X04, 0X70, 0X00, 0X00, 0X20, 
					0X01, 0X01, 0X01, 0X01, 0X00, 0X55, 0X55, 0XD2, 0X16};
//1路：报警
BYTE SendData7[] = {0X68, 0X3D, 0X00, 0X3D, 0X00, 0X68, 0X4A, 0X11, 0X11, 0X11, 0X11, 0X02, 0X05, 0X70, 0X01, 0X01, 0X01, 
					0X00, 0X01, 0X55, 0X55, 0XB3, 0X16};
//2路：报警
BYTE SendData8[] = {0X68, 0X3D, 0X00, 0X3D, 0X00, 0X68, 0X4A, 0X11, 0X11, 0X11, 0X11, 0X02, 0X05, 0X70, 0X02, 0X01, 0X01, 
					0X00, 0X01, 0X55, 0X55, 0XB4, 0X16};
//时段控（功控灯亮）
BYTE SendData9[] = {0X68, 0X41, 0X00, 0X41, 0X00, 0X68, 0X4A, 0X11, 0X11, 0X11, 0X11, 0X02, 0X05, 0X70, 0X01, 0X01, 0X01, 
					0X01, 0XFF, 0X01, 0X55, 0X55, 0XB3, 0X16};
//解除，功控灯灭
BYTE SendData10[] ={0X68, 0X39, 0X00, 0X39, 0X00, 0X68, 0X4A, 0X11, 0X11, 0X11, 0X11, 0X02, 0X05, 0X70, 0X01, 0X01, 0X01, 
					0X02, 0X55, 0X55, 0XB4, 0X16};
//电控灯亮
BYTE SendData11[] ={0X68, 0X39, 0X00, 0X39, 0X00, 0X68, 0X4A, 0X11, 0X11, 0X11, 0X11, 0X02, 0X05, 0X70, 0X01, 0X01, 0X40, 
					0X01, 0X55, 0X55, 0XF2, 0X16};
//解除，电控灯灭
BYTE SendData12[] ={0X68, 0X39, 0X00, 0X39, 0X00, 0X68, 0X4A, 0X11, 0X11, 0X11, 0X11, 0X02, 0X05, 0X70, 0X01, 0X01, 0X40, 
					0X02, 0X55, 0X55, 0XF3, 0X16};
//保电投入
BYTE SendData13[] ={0X68, 0X3D, 0X00, 0X3D, 0X00, 0X68, 0X4A, 0X11, 0X11, 0X11, 0X11, 0X02, 0X05, 0X70, 0X00, 0X00, 0X01, 
					0X03, 0X10, 0X55, 0X55, 0XC3, 0X16};
//参数及数据初始化：
BYTE SendData14[] ={0X68, 0X39, 0X00, 0X39, 0X00, 0X68, 0X41, 0X11, 0X11, 0X11, 0X11, 0X02, 0X01, 0X70, 0X00, 0X00, 0X04, 
					0X00, 0X55, 0X55, 0XA6, 0X16};
//数据初始化
BYTE SendData15[] ={0X68, 0X39, 0X00, 0X39, 0X00, 0X68, 0X41, 0X11, 0X11, 0X11, 0X11, 0X02, 0X01, 0X70, 0X00, 0X00, 0X02, 
					0X00, 0X55, 0X55, 0XA4, 0X16};
//硬件初始化
BYTE SendData16[] ={0X68, 0X39, 0X00, 0X39, 0X00, 0X68, 0X41, 0X11, 0X11, 0X11, 0X11, 0X02, 0X01, 0X70, 0X00, 0X00, 0X01, 
					0X00, 0X55, 0X55, 0XA3, 0X16};
//修改IP以及Port
BYTE SendData17[] ={0X68,0XA9,0X00,0XA9,0X00,0X68,0X4A,0X11,0X11,0X11,0X11,0X02,0X04,0X70,0X00,0X00,0X04,
0X00,0X11,0X11,0X11,0X11,0X11,0X11,0X11,0X11,0X11,0X11,0X11,0X11,0X11,0X11,0X11,0X11,0X11,0X11,0X11,0X11,0X11,
0X11,0X11,0X11,0X11,0X11,0X11,0X11,0X55,0X55,0X8E,0X16
};

//对时间
BYTE SendData18[] ={0X68,0X51,0X00,0X51,0X00,0X68,0X4A,0X11,0X11,0X11,0X11,0X02,0X05,0X70,0X00,0X00,0X40,0X03,
0X11,0X11,0X11,0X11,0X11,0X11,0X55,0X55,0X58,0X16
};


//终端事件记录配置F8
BYTE SendData19[] ={0X68, 0X79, 0X00, 0X79, 0X00, 0X68, 0X4A, 0X11, 0X12, 0X05, 0X00, 0X02, 0X04, 0X72, 0X00, 0X00, 0X80, 
					0X00, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 
					0X00, 0X00, 0X5A, 0X16};


//F9终端状态量及变位标志
BYTE SendData20[] ={0X68,0X31,0X00,0X31,0X00,0X68,0X4B,0X11,0X11,0X11,0X11,0X02,0X0C,0X60,0X00,0X00,0X01,0X01,0XFF,0X16
};


//版本信息
BYTE SendData21[] ={0X68, 0X31, 0X00, 0X31, 0X00, 0X68, 0X4B, 0XFF, 0XFF, 0XFF, 0XFF, 0X02, 0X0C, 0X60, 0X00, 0X00, 0X01, 
					0X00, 0X05, 0X16};



//---------------dlt698------
//保电解除
BYTE SendData6981[] = {0x68,0X39,0X00,0X39,0X00,0X68,0X4A,0X11,0X11,0X11,0X11,0X02,0X05,0X70,0X00,0X00,0X01,0X04,0X55,0X55,0XB4,0X16
};//?

//配置电能表 （两路电能表1口，装置序号2，测量点2，端口可变）
BYTE SendData6982[] = {0X68,0XAD,0X00,0XAD,0X00,0X68,0X4A,0X11,0X11,0X11,0X11,0X02,0X04,0X70,0X00,0X00,0X02,0X01,0X01,0X00,
0X02,0X00,0X02,0X00,0X41,0X01,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X04,0X01,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X55,0X55,0X84,0X16};
//配置脉冲(2路脉冲)
/*
BYTE SendData3[] = {0X68, 0X65, 0X00, 0X65, 0X00, 0X68, 0X4A, 0X11, 0X11, 0X16, 0X11, 0X02, 0X04, 0X73, 0X00, 0X00, 0X04, 
					0X01, 0X02, 0X01, 0X04, 0X00, 0X00, 0X19, 0X02, 0X04, 0X01, 0X00, 0X19, 0X01, 0X00, 0X52, 0X16};
*/
BYTE SendData6983_1[] = {0x68, 0x8A, 0x00, 0x8A, 0x00, 0x68, 0x5A, 0x11, 0x22, 0x33, 0x44, 0x02, 0x04, 0x60, 
	0x00, 0x00, 0x04, 0x01, 0x01, 0x01, 0x04, 0x00, 0x01, 0x00, 0x67, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x16, };

BYTE SendData6983_2[] = {0x68, 0x9E, 0x00, 0x9E, 0x00, 0x68, 0x7A, 0x11, 0x22, 0x33, 0x44, 0x02, 0x04, 0x60, 0x00, 0x00, 0x04, 0x01, 0x02, 0x01, 0x04, 0x00, 0x01, 0x00, 0x02, 0x04, 0x01, 0x01, 0x00, 0x67, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8F, 0x16, };

BYTE SendData6983_3[] = {0x68, 0xB2, 0x00, 0xB2, 0x00, 0x68, 0x5A, 0x99, 0x99, 0xFF, 0xFF, 0x02, 0x04, 0x60, 0x00, 0x00, 0x04, 0x01, 0x03, 0x01, 0x04, 0x00, 0x01, 0x00, 0x02, 0x04, 0x01, 0x01, 0x00, 0x03, 0x05, 0x00, 0x01, 0x00, 0x67, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x16, };

BYTE SendData6983_4[] = {0x68, 0xC6, 0x00, 0xC6, 0x00, 0x68, 0x7A, 0x99, 0x99, 0xFF, 0xFF, 0x02, 0x04, 0x60, 0x00, 0x00, 0x04, 0x01, 0x04, 0x01, 0x04, 0x00, 0x01, 0x00, 0x02, 0x04, 0x01, 0x01, 0x00, 0x03, 0x05, 0x00, 0x01, 0x00, 0x04, 0x05, 0x01, 0x01, 0x00, 0x67, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2B, 0x16, };

BYTE SendData6983_5[] = {0x68, 0xDA, 0x00, 0xDA, 0x00, 0x68, 0x5A, 0x99, 0x99, 0xFF, 0xFF, 0x02, 0x04, 0x60, 0x00, 0x00, 0x04, 0x01, 0x05, 0x01, 0x04, 0x00, 0x01, 0x00, 0x02, 0x04, 0x01, 0x01, 0x00, 0x03, 0x05, 0x00, 0x01, 0x00, 0x04, 0x05, 0x01, 0x01, 0x00, 0x05, 0x06, 0x00, 0x01, 0x00, 0x67, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x16, };

BYTE SendData6983_6[] = {0x68, 0xEE, 0x00, 0xEE, 0x00, 0x68, 0x7A, 0x99, 0x99, 0xFF, 0xFF, 0x02, 0x04, 0x60, 0x00, 0x00, 0x04, 0x01, 0x06, 0x01, 0x04, 0x00, 0x01, 0x00, 0x02, 0x04, 0x01, 0x01, 0x00, 0x03, 0x05, 0x00, 0x01, 0x00, 0x04, 0x05, 0x01, 0x01, 0x00, 0x05, 0x06, 0x00, 0x01, 0x00, 0x06, 0x06, 0x01, 0x01, 0x00, 0x67, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x16, };


//?







//抄表间隔设置F33
BYTE SendData6984[] = {0X68,0X85,0X00,0X85,0X00,0X68,0X4A,0X11,0X11,0X11,0X11,0X02,0X04,0X70,0X00,0X00,0X01,0X04,0X01,
						0X01,0X00,0X00,0X01,0X00,0X00,0X00,0X10,0X13,0X01,0X00,0X00,0X00,0X01,0X02,0X00,0X50,0X23,0X55,
						0X55,0X3B,0X16};//
						
//4路遥信
//BYTE SendData6985[] = {0X68,0X41,0X00,0X41,0X00,0X68,0X4A,0X11,0X11,0X11,0X11,0X02,0X04,0X70,0X00,0X00,0X08,0X01,0X0F,0X0F,0X55,0X55,0XD5,0X16
//};//
//2路遥信
BYTE SendData6985[] = {0x68,0x7A,0x00,0x7A,0x00,0x68,0x4A,0x11,0x11,0x08,0x00,0x02,0x04,0x70,0x00,0x00,0x08,0x01,0xC3,0xC3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x79,0x16
};//

//总加组
BYTE SendData6986[] = {0X68,0X49,0X00,0X49,0X00,0X68,0X4A,0X11,0X11,0X11,0X11,0X02,0X04,0X70,0X00,0X00,0X20,0X01,0X01,0X01,0X01,0X00,0X55,0X55,0XD2,0X16
};
//1路：报警
BYTE SendData6987[] = {0X68, 0X3D, 0X00, 0X3D, 0X00, 0X68, 0X4A, 0X11, 0X11, 0X11, 0X11, 0X02, 0X05, 0X70, 0X01, 0X01, 0X01, 
					0X00, 0X01, 0X55, 0X55, 0XB3, 0X16};//?

//2路：报警
BYTE SendData6988[] = {0X68, 0X3D, 0X00, 0X3D, 0X00, 0X68, 0X4A, 0X11, 0X11, 0X11, 0X11, 0X02, 0X05, 0X70, 0X02, 0X01, 0X01, 
					0X00, 0X01, 0X55, 0X55, 0XB4, 0X16};//?
//时段控（功控灯亮）
BYTE SendData6989[] = {0X68,0X41,0X00,0X41,0X00,0X68,0X4A,0X11,0X11,0X11,0X11,0X02,0X05,0X70,0X01,0X01,0X01,0X01,0XFF,0X00,0X55,0X55,0XB2,0X16
};//?
//解除，功控灯灭
BYTE SendData69810[] ={0X68,0X39,0X00,0X39,0X00,0X68,0X4A,0X11,0X11,0X11,0X11,0X02,0X05,0X70,0X01,0X01,0X01,0X02,0X55,0X55,0XB4,0X16};//?
//电控灯亮
BYTE SendData69811[] ={0X68,0X39,0X00,0X39,0X00,0X68,0X4A,0X11,0X11,0X11,0X11,0X02,0X05,0X70,0X01,0X01,0X80,0X01,0X55,0X55,0X32,0X16};//?
//解除，电控灯灭
BYTE SendData69812[] ={0x68,0X39,0X00,0X39,0X00,0X68,0X4A,0X11,0X11,0X11,0X11,0X02,0X05,0X70,0X01,0X01,0X80,0X02,0X55,0X55,0X33,0X16};//?
//保电投入
BYTE SendData69813[] ={0X68,0X3D,0X00,0X3D,0X00,0X68,0X4A,0X11,0X11,0X11,0X11,0X02,0X05,0X70,0X00,0X00,0X01,0X03,0X10,0X55,0X55,0XC3,0X16
};//?
//参数及数据初始化：f3
BYTE SendData69814[] ={0X68,0X39,0X00,0X39,0X00,0X68,0X41,0X11,0X11,0X11,0X11,0X02,0X01,0X70,0X00,0X00,0X04,0X00,0X55,0X55,0XA6,0X16
};
//数据初始化
BYTE SendData69815[] ={0X68,0X39,0X00,0X39,0X00,0X68,0X41,0X11,0X11,0X11,0X11,0X02,0X01,0X70,0X00,0X00,0X02,0X00,0X55,0X55,0XA4,0X16
};
//硬件初始化
BYTE SendData69816[] ={0X68,0X39,0X00,0X39,0X00,0X68,0X41,0X11,0X11,0X11,0X11,0X02,0X01,0X70,0X00,0X00,0X01,0X00,0X55,0X55,0XA3,0X16
};
//修改IP以及Port
BYTE SendData69817[] ={0X68,0XA9,0X00,0XA9,0X00,0X68,0X4A,0X11,0X11,0X11,0X11,0X02,0X04,0X70,0X00,0X00,0X04,0X00,0X00,
						0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
						0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X55,0X55,0X20,0X16};//
//对时间
BYTE SendData69818[] ={0X68, 0X51, 0X00, 0X51, 0X00, 0X68, 0X4A, 0X11, 0X11, 0X11, 0X11, 0X02, 0X05, 0X60, 0X00, 0X00, 0X40, 
					0X03, 0X00, 0X24, 0X17, 0X26, 0XB0, 0X07, 0X00, 0X00, 0X50, 0X16};


//终端事件记录配置F8
BYTE SendData69819[] ={0X68, 0X79, 0X00, 0X79, 0X00, 0X68, 0X4A, 0X11, 0X12, 0X05, 0X00, 0X02, 0X04, 0X72, 0X00, 0X00, 0X80, 
					0X00, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 
					0X00, 0X00, 0X5A, 0X16};


//F9终端状态量及变位标志
BYTE SendData69820[] ={0X68, 0X31, 0X00, 0X31, 0X00, 0X68, 0X4B, 0X11, 0X12, 0X05, 0X00, 0X02, 0X0C, 0X60, 0X00, 0X00, 0X01, 
					0X01, 0XE3, 0X16};


//版本信息
BYTE SendData69821[] ={0X68,0X31,0X00,0X31,0X00,0X68,0X4B,0X11,0X11,0X11,0X11,0X02,0X09,0X60,0X00,0X00,0X01,0X00,0XFB,0X16};

// 参数及数据初始化f4
BYTE SendData69822[] ={0X68,0X39,0X00,0X39,0X00,0X68,0X41,0X11,0X11,0X11,0X11,0X02,0X01,0X70,0X00,0X00,0X08,0X00,0X55,0X55,0XAA,0X16
};

// 配置交采，测量点1，装置序号1，端口可设
BYTE SendData69823[] ={0x68,0XAD,0X00,0XAD,0X00,0X68,0X4A,0X11,0X11,0X12,0X11,0X02,0X04,0X70,0X00,0X00,0X02,0X01,0X01,0X00,0X01,0X00,
0X01,0X00,0X03,0X02,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X04,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X55,0X55,0XBF,0X16
};


// 跳4轮闸
//BYTE SendData69824[] ={0x68,0x49,0x00,0x49,0x00,0x68,0x4A,0x11,0x11,0x14,0x11,0x02,0x05,0x70,0x0F,0x01,0x01,0x00,0x10,0x10,0x10,0x10,0x55,0x55,0xCB,0x16
//};
// 跳2轮闸
BYTE SendData69824[] ={0x68,0x8A,0x00,0x8A,0x00,0x68,0x4A,0x11,0x11,0x08,0x00,0x02,0x05,0x73,0x01,0x01,0x01,0x00,0x11,0x02,0x01,0x01,0x00,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x17,0x16
};

// 端口配置
BYTE SendData69825[] ={0X68,0X31,0X00,0X31,0X00,0X68,0X4B,0X11,0X11,0X11,0X11,0X02,0X09,0X60,0X00,0X00,0X02,0X00,0XFB,0X16
};

// 4轮合闸
//BYTE SendData69826[] ={0x68,0x39,0x00,0x39,0x00,0x68,0x4A,0x11,0x11,0x11,0x11,0x02,0x05,0x70,0x0F,0x01,0x02,0x00,0x55,0x55,0xC1,0x16
//};
// 2轮合闸
BYTE SendData69826[] ={0x68,0x72,0x00,0x72,0x00,0x68,0x4A,0x11,0x11,0x08,0x00,0x02,0x05,0x74,0x03,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF5,0x16
};

// 设置f25，脉冲测量点
BYTE SendData69827_1[] ={0x68,0x65,0x00,0x65,0x00,0x68,0x4A,0x11,0x11,0x11,0x11,0x02,0x04,0x70,0x08,0x01,0x01,0x03,0x01,0x00,0x01,0x00,0x00,0x22,0x15,0x00,0x00,0x01,0x02,0x55,0x55,0x63,0x16 
};
BYTE SendData69827_2[] ={0x68, 0xDA, 0x00, 0xDA, 0x00, 0x68, 0x5A, 0x99, 0x99, 0xFF, 0xFF, 0x02, 0x04, 0x60, 0x08, 0x01, 0x01, 0x03, 0x01, 0x00, 0x01, 0x00, 0x00, 0x22, 0x15, 0x00, 0x00, 0x01, 0x02, 0x10, 0x01, 0x01, 0x03, 0x01, 0x00, 0x01, 0x00, 0x00, 0x22, 0x15, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8A, 0x16, };

// F57，告警声音打开
BYTE SendData69828[] ={0X68,0X46,0X00,0X46,0X00,0X68,0X4A,0X11,0X11,0X09,0X00,0X02,0X04,0X70,0X00,0X00,0X01,0X07,0XFF,0XFF,0XFF,0X00,0X00,0XF0,0X16

};

// added on 20110222, 485读表透抄测试
//BYTE SendData69829_4851[] ={0X68,0X96,0X00,0X96,0X00,0X68,0X4B,0X11,0X11,0X11,0X11,0X02,0X10,0X60,0X00,0X00,0X01,0X00,
//0X02,0X7B,0X82,0X01,0X14,0XFE,0XFE,0XFE,0XFE,0X68,0X99,0X99,0X99,0X99,0X99,0X99,0X68,0X11,0X04,0X33,0X33,0X34,0X33,0X48,0X16,
///*0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,*/0XB4,0X16};

BYTE SendData69829_4851[] ={0X68,0XA2,0X00,0XA2,0X00,0X68,0X4B,0X11,0X11,0X11,0X11,0X02,0X10,0X60,0X00,0X00,0X01,0X00,0X02,0X7B,0X85,0X01,0X14,0x00,0XFE,0XFE,0XFE,0XFE,0X68,0X42,0X00,0X00,0X00,0X00,0X00,0X68,0X11,0X04,0X33,0X33,0X34,0X33,0Xf4,0X16,0X00,0X00,0X02,0X16};


// 32
BYTE SendData69832_mac[] ={0x68,0x62,0x00,0x62,0x00,0x68,0x4A,0x11,0x11,0x11,0x11,0x02,0xF1,0x70,0x01,0x01,0x01,0x14,0x55,0x55,0x08,0x00,0x11,0x22,0x33,0x44,0x9B,0xC0,0xAA,0xAA,0x65,0x16};

BYTE SendData69833_mac[] ={0x68,0x32,0x00,0x32,0x00,0x68,0x4B,0x99,0x99,0x99,0x99,0x02,0xF1,0x61,0x01,0x01,0x02,0x14,0x4D,0x16};

BYTE SendData69834_ver[] = {0x68,0x32,0x00,0x32,0x00,0x68,0x4B,0x99,0x99,0x99,0x99,0x02,0x09,0x62,0x00,0x00,0x80,0x0A,0xD8,0x16};

// 设置门节点有效
BYTE SendData69835_door[] = {0x68,0x42,0x00,0x42,0x00,0x68,0x4A,0x11,0x11,0x11,0x11,0x02,0x04,0x70,0x00,0x00,0x08,0x01,0xFF,0xFF,0x00,0x00,0x0B,0x16};
// 
BYTE SendDataSetCDMA[] = {0x68, 0x3E, 0x01, 0x3E, 0x01, 0x68, 0x4B, 0x11, 0x11, 0x07, 0x00, 0x02, 0xF1, 0x60, 0x00, 0x00, 0x02, 0x0E, 0x01, 0x63, 0x61, 0x72, 0x64, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x61, 0x72, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x11, 0x16, };
BYTE SendDataGetCDMA[] = {0x68, 0x35, 0x00, 0x35, 0x00, 0x68, 0x4B, 0x99, 0x99, 0xFF, 0xFF, 0x02, 0xF1, 0x60, 0x00, 0x00, 0x02, 0x0E, 0x00, 0xDE, 0x16};
BYTE SendDataClrAC[] = {0x68, 0x46, 0x00, 0x46, 0x00, 0x68, 0x4B, 0x99, 0x99, 0xFF, 0xFF, 0x02, 0xF1, 0x70, 0x01, 0x01, 0x20, 0x09, 0x11, 0x33, 0x55, 0x77, 0x01, 0x1A, 0x16, };

BYTE SendDataSetF8[] = {0x68, 0x92, 0x00, 0x92, 0x00, 0x68, 0x7A, 0x11, 0x22, 0x33, 0x44, 0x02, 0x04, 0x60, 0x00, 0x00, 0x80, 0x00, 0x11, 0x58, 0x02, 0x03, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA4, 0x16, };
BYTE SendDataGetF8[] = {0x68, 0x32, 0x00, 0x32, 0x00, 0x68, 0x5B, 0x11, 0x22, 0x33, 0x44, 0x02, 0x0A, 0x60, 0x00, 0x00, 0x80, 0x00, 0xF1, 0x16, };

BYTE SendDataSetF225[] = {0x68,0x92,0x00,0x92,0x00,0x68,0x4A,0x03,0x35,0xF6,0x1F,0x02,0x04,0x70,0x00,0x00,0x01,0x1C,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCE,0x16 };

//读取内部、外部电池电压
BYTE ReadBattVol[] = {0x68,0x32,0x00,0x32,0x00,0x68,0x5B,0x11,0x11,0x11,0x11,0x02,0x0C,0x77,0x00,0x00,0x10,0x02,0x35,0x16};

//打开终端消息认证
BYTE OpenMessageAuth[] = {0x68,0x7E,0x00,0x7E,0x00,0x68,0x4A,0x11,0x11,0x01,0x00,0x02,0x04,0x70,0x00,0x00,0x10,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF2,0x16};
//读取ESAM序列号
BYTE ReadEsamSEQ[] = {0x68,0x72,0x00,0x72,0x00,0x68,0x4B,0x11,0x11,0x01,0x00,0x02,0x06,0x71,0x00,0x00,0x04,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEC,0x16};
//关闭终端消息认证
BYTE CloseMessageAuth[] = {68,0x7E,0x00,0x7E,0x00,0x68,0x4A,0x11,0x11,0x01,0x00,0x02,0x04,0x72,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x0A,0xFD,0x89,0x37,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xBC,0x16};

#define TYPE_SET_CDMA 100
#define TYPE_GET_CDMA 101
#define TYPE_CLR_AC   102
#define TYPE_SET_F8 103
#define TYPE_GET_F8 104
#define TYPE_SET_225 105

//终端类型
int g_RtuType = TERMINAL_TYPE_GW698;
BOOL g_bPwdLong = TRUE;
BOOL g_PortInfo = FALSE;
BOOL g_bAddrLong = FALSE;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}
BOOL CAboutDlg::OnInitDialog()
{
	CString strInfo("");
	//
	CDialog::OnInitDialog();

	strInfo+="版本说明：\r\n";
	strInfo+="1.1版本加入了：\r\n";
	strInfo+="  1.支持国网终端的设置；\r\n";
	strInfo+="  2.密码字节数可以选择2或16，即短密码和长密码；\r\n";
	strInfo+="  3.国网终端配置电能表时，485读表端口为端口1和4。\r\n";
	
	strInfo+="1.2版本加入了(2010-04-06)：\r\n";
	strInfo+="  1.设置交采测量点；\r\n";
	strInfo+="  2.交采和抄表端口可设；\r\n";
	strInfo+="  3.支持端口信息读取;\r\n";
	strInfo+="  4.加入合闸。\r\n";
	
	strInfo+="1.3版本加入了:(20110222)\r\n";
	strInfo+="  1.支持山东485集中器三个端口的透抄；\r\n";
	strInfo+="  2.支持F57告警声音允许打开;\r\n";
	strInfo+="  3.可以设置奇偶校验。(山东使用115200,无校验)\r\n";

	strInfo+="1.4版本修改了:(20111009)\r\n";
	strInfo+="  1.APN设置不能达到16位的问题\r\n";	

	strInfo+="1.5版本修改了:(20111009)\r\n";
	strInfo+="  1.清除APN前后的空格\r\n";	

	strInfo+="1.6版本修改了:(20120726)\r\n";
	strInfo+="  1.加入了mac地址的设置、读取\r\n";	
	strInfo+="  2.加入了内部软件版本号的读取\r\n";	

	strInfo+="1.7版本修改了:(20120817)\r\n";
	strInfo+="  1.加入了设置门节点有效状态\r\n";	

	strInfo+="1.7版本修改了:(20120817)\r\n";
	strInfo+="  1.加入了设置门节点有效状态\r\n";	

	strInfo+="1.8版本修改了:(20120919)\r\n";
	strInfo+="  1.增加CDMA用户名密码的查询设置\r\n";
	strInfo+="  2.增加交采清表底功能\r\n";
	strInfo+="  3.增加对8位16进制地址的支持\r\n";

	strInfo+="1.9版本修改了:(20121122)\r\n";
	strInfo+="  1.增加对多路脉冲的设置\r\n";
	strInfo+="  2.MAC地址改为由80-XX-XX...改为08-XX-XX...\r\n";

	
	strInfo+="2.0版本修改了:(20121122)\r\n";
	strInfo+="  1.增加对参数F8终端上行通信工作方式的设置\r\n";

	strInfo+="2.1版本修改了:(20130118)\r\n";
	strInfo+="  1.MAC地址设置时可以自由设置\r\n";

	strInfo+="2.2版本修改了:(20130121)\r\n";
	strInfo+="  1.MAC地址设置时改为设置某批次终端的起始地址和数量，\r\n";
	strInfo+="    及对应的起始MAC地址。以方便批量设置。\r\n";
	strInfo+="  2.修改设置IP时会把0开头的数字解析为八进制数字的问题\r\n";

	strInfo+="2.3版本修改了:(20130202)\r\n";
	strInfo+="  1.修改MAC地址设置时的错误\r\n";	
	GetDlgItem(IDC_EDIT_INFO)->SetWindowText(strInfo);

	strInfo+="2.4版本修改了:(20130222)\r\n";
	strInfo+="  1.为福建地区增加了设置终端资产编码F225\r\n";	
	GetDlgItem(IDC_EDIT_INFO)->SetWindowText(strInfo);	
	return TRUE;
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFkSetDlg dialog

CFkSetDlg::CFkSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFkSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFkSetDlg)
	m_nAcPort = 1;
	m_n4851Port = 2;
	m_editmt1 = _T("");
	m_editmt2 = _T("");
	//m_editmt3 = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFkSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFkSetDlg)
	//DDX_Control(pDX, IDC_COMBO_PLUSENUM, m_PluseNum);
	DDX_Control(pDX, IDC_COMBO_PARITY, m_selParity);
	
	DDX_Control(pDX, IDC_COMBO5, m_sel4852);
	DDX_Control(pDX, IDC_COMBO4, m_sel4851);
	DDX_Control(pDX, IDC_EDIT, m_ctlResult);
	DDX_Control(pDX, IDC_COMBO_B, m_ctlB);
	DDX_Control(pDX, IDC_COMBO_COM, m_ctlCom);
	DDX_Control(pDX, IDC_LIST, m_AddressList);
	//DDX_Text(pDX, IDC_EDIT_ACPORT, m_nAcPort);
	//DDV_MinMaxInt(pDX, m_nAcPort, 0, 32);
	//DDX_Text(pDX, IDC_EDIT_4851, m_n4851Port);
	//DDV_MinMaxInt(pDX, m_n4851Port, 0, 32);
	DDX_Text(pDX, IDC_EDITMT1, m_editmt1);
	DDV_MaxChars(pDX, m_editmt1, 12);
	DDX_Text(pDX, IDC_EDITMT2, m_editmt2);
	DDV_MaxChars(pDX, m_editmt2, 12);
	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFkSetDlg, CDialog)
	//{{AFX_MSG_MAP(CFkSetDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(ID_CLICK_OK, OnClickOk)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_OPEN_COM, OnOpenCom)
	ON_BN_CLICKED(IDC_CLOSE_COM, OnCloseCom)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
	ON_BN_CLICKED(IDC_BUTTON18, OnButton18)
	ON_BN_CLICKED(IDC_BUTTON19, OnButton19)
	ON_BN_CLICKED(IDC_BUTTON20, OnButton20)
	ON_BN_CLICKED(IDC_BUTTON21, OnButton21)
	ON_BN_CLICKED(IDC_BUTTON22, OnButton22)
	ON_BN_CLICKED(IDC_BUTTON23, OnButton23)
	ON_BN_CLICKED(IDC_BUTTON24, OnButton24)
	ON_BN_CLICKED(IDC_BUTTON26, OnButton26)
	ON_BN_CLICKED(IDC_BUTTON27, OnButton27)
	ON_BN_CLICKED(IDC_BUTTON29, OnButton29)
	ON_BN_CLICKED(IDC_BUTTON30, OnButton30)
	ON_BN_CLICKED(IDC_BUTTON28, OnButton28)
	ON_BN_CLICKED(IDC_BUTTON32, OnButtonSetMac)
	ON_BN_CLICKED(IDC_BUTTON35, OnButtonRdMac)
	ON_BN_CLICKED(IDC_BUTTON34, OnButtonRdVer)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST, OnKillfocusList)
	ON_NOTIFY(NM_RETURN, IDC_LIST, OnReturnList)
	ON_BN_CLICKED(IDC_BUTTON_DOOR, OnButtonDoor)
	ON_BN_CLICKED(IDC_BUTTON33, OnButton33)
	ON_BN_CLICKED(IDC_BUTTON36, OnButton36)
	ON_BN_CLICKED(IDC_BUTTON_CLRAC, OnButtonClrAC)
	ON_BN_CLICKED(IDC_BUTTON37, OnButton37)
	ON_BN_CLICKED(IDC_BUTTON_SETID, OnButtonSetid)
	ON_BN_CLICKED(IDC_BUTTON38, OnButton38)
	ON_BN_CLICKED(IDC_BUTTON_UPMODEM, OnButtonUpmodem)
	ON_BN_CLICKED(IDC_ESAM, OnEsam)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFkSetDlg message handlers

BOOL CFkSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_conData.m_recEvent = CreateEvent(NULL , TRUE , FALSE , NULL);
	m_conData.m_sendEvent = CreateEvent(NULL , TRUE , FALSE , NULL);
	m_conData.m_recLength = 0;
	m_conData.m_sendLength = 0;

	InitializeCriticalSection(&m_conData.m_recCS);
	
	m_AddressList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES );
	m_AddressList.InsertColumn(0,_T("序号"),LVCFMT_LEFT,50);
	m_AddressList.InsertColumn(1,_T("终端地址 L=9或12"),LVCFMT_LEFT,120);
	m_AddressList.InsertColumn(2,_T("备注"),LVCFMT_LEFT,337);
	
	m_ctlCom.SetCurSel(0);
	m_ctlB.SetCurSel(3);

	SetButtonState();

	LoadParamFile();

	//((CButton*)GetDlgItem(IDC_RADIO_698))->SetCheck(TRUE);
	g_RtuType = TERMINAL_TYPE_GW698;
	//((CButton*)GetDlgItem(IDC_RADIO_PWLONG))->SetCheck(TRUE);
	//g_bPwdLong = TRUE;	

//	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
//	GetDlgItem(IDC_BUTTON7)->EnableWindow(FALSE);
//	GetDlgItem(IDC_BUTTON19)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON22)->EnableWindow(TRUE);
	
	m_sel4851.SetCurSel(1);
	m_sel4852.SetCurSel(2);
	m_sel4853.SetCurSel(3);
	m_selParity.SetCurSel(2);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFkSetDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFkSetDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFkSetDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFkSetDlg::OnOK()
{
}

void CFkSetDlg::OnCancel()
{	
}

void CFkSetDlg::OnAdd() 
{
	int index;
	int count = m_AddressList.GetItemCount();
	CString strTmp;

	if(count == 0)
	{
		index = m_AddressList.InsertItem(m_AddressList.GetItemCount(),"1");
		m_AddressList.SetItemText(index,1,"111100001");
	}
	else
	{
		strTmp.Format("%d",m_AddressList.GetItemCount()+1);
		index = m_AddressList.InsertItem(m_AddressList.GetItemCount(),strTmp);
		
		strTmp = m_AddressList.GetItemText(count - 1,1);
		char straddr[20],strarea[20];
	//	strncpy(straddr,strTmp+4,8);
		strcpy(straddr,strTmp);
		strncpy(strarea,strTmp,4);
		int nNext = atoi(straddr+4) + 1;
		int nArea = atoi(strarea);

		if (strlen(strTmp) == 9)
		{
		strTmp.Format("%04d%05d",nArea,nNext);
		}
		else if (strlen(strTmp) == 8)
		{
		strTmp.Format("%04d%04d",nArea,nNext);
		}
		else
		{
		strTmp.Format("%04d%08d",nArea,nNext);
		}
		

		m_AddressList.SetItemText(index,1,strTmp);
	}
}

void CFkSetDlg::OnDel() 
{
	int i;
	CUIntArray arr;
	POSITION pos = m_AddressList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = m_AddressList.GetNextSelectedItem(pos);
			arr.Add(nItem);
		}
	}
	for(i = arr.GetSize() - 1 ; i >=0 ;i--)
	{
		m_AddressList.DeleteItem(arr[i]);
	}
	RefreshNum();
}

void CFkSetDlg::OnClickOk() 
{
	ExitFkSetData();
}

void CFkSetDlg::OnClose() 
{
	ExitFkSetData();
}

void CFkSetDlg::ExitFkSetData()
{
	if(m_conData.m_recEvent != NULL)
	{
		CloseHandle(m_conData.m_recEvent);
		m_conData.m_recEvent = NULL;
	}
	if(m_conData.m_sendEvent != NULL)
	{
		CloseHandle(m_conData.m_sendEvent);
		m_conData.m_sendEvent = NULL;
	}
	
	DeleteCriticalSection(&m_conData.m_recCS);	
	
	SaveParamFile();

	CDialog::OnOK();
}

void CFkSetDlg::OnOpenCom() 
{
	CString strTmp;
	
	m_ctlB.GetWindowText(strTmp);
	m_conData.m_cfg.m_icommno = m_ctlCom.GetCurSel() + 1;
	m_conData.m_cfg.m_iBaud = atoi(strTmp);
	m_conData.m_cfg.m_cbDataBit = 8;
	m_conData.m_cfg.m_cbStopBit = 0;
	m_conData.m_cfg.m_cbParity = m_selParity.GetCurSel();
//	m_conData.m_cfg.m_cbParity = 0;
	if(!m_port.IsOpen())
	{
		m_port.Init(&m_conData);
	}

	SetButtonState();
}

void CFkSetDlg::OnCloseCom() 
{
	if(m_port.IsOpen())
	{
		m_port.UnInit();
	}
	SetButtonState();
}

void CFkSetDlg::SetButtonState()
{
	if(m_port.IsOpen())
	{
		GetDlgItem(IDC_OPEN_COM)->EnableWindow(0);

		GetDlgItem(IDC_CLOSE_COM)->EnableWindow(1);
	}
	else
	{
		GetDlgItem(IDC_OPEN_COM)->EnableWindow(1);

		GetDlgItem(IDC_CLOSE_COM)->EnableWindow(0);
	}
}

//计算校验和
unsigned char CalcCS_r(unsigned char *pdata, short datalen)
{
	unsigned char checksum=0;
	short i;

	for(i=0; i<datalen; i++)
	{
		checksum+=pdata[i];
	}

	return checksum;
}

int CalcCRC16_r(unsigned char *pdata, int datalen)
{
	int tmpCRC=0;
	int i;
	int j;
	int iflag;

	for(i=0; i<datalen; i++)
	{
		tmpCRC^=(pdata[i]<<8);
		for(j=0; j<8; j++)
		{
			iflag=tmpCRC&0x8000;
			tmpCRC<<=1;
			if(iflag)
			{
				tmpCRC^=0x1021;
			}
		}
	}

	return tmpCRC;
}

void CFkSetDlg::SetCheckTmu(unsigned char cType, unsigned char* pReport, unsigned short len,BOOL bPwdLong, void* param1, void* param2, void* param3)
{
	CString		strTmp,strTmp1;
	char		rtuAddress[6];
	LMUACKINFO	lmuinfo;
	memset(rtuAddress,0,sizeof(rtuAddress));
	memset(&lmuinfo,0,sizeof(LMUACKINFO));
	int tmp,analy,k;
	char		cTemp;
	int Dtlen = 0;
	
	if(!m_port.IsOpen())
	{
		AfxMessageBox("串口没有打开.");
		return;
	}
	
	//EnableAllButton(0);

	for(int i = 0 ; i < m_AddressList.GetItemCount() ; i++)
	{
		if(m_AddressList.GetCheck(i))
		{
			//判断终端的地址
			CString		strAddr;
			strAddr = m_AddressList.GetItemText(i,1);
			//if(RtuStrToCode(strTmp.GetBuffer(9),rtuAddress) != 0)
			if(RtuStrToCode(strAddr.GetBuffer(12),rtuAddress) != 0)
			{
				AfxMessageBox("终端地址不合法!!!\r\n地址必须为9或12个字符（吉林为12），\r\n不足请在第四个字符后补零!!!");

				break;
			}
			
			unsigned char c1 = rtuAddress[0];
			unsigned char c2 = rtuAddress[1];
			unsigned char c3 = rtuAddress[2];
			unsigned char c4 = rtuAddress[3];
			unsigned char c5 = rtuAddress[4];
			unsigned char c6 = rtuAddress[5];
			lmuinfo.Address.AreaCode = c2*256+c1;			
			lmuinfo.Address.UnitAddr = c4*256+c3;
			lmuinfo.Address.HostAddr = 0x51;
			
			pReport[7] = (unsigned char)lmuinfo.Address.AreaCode;
			pReport[8] = (unsigned char)(lmuinfo.Address.AreaCode>>8);
			pReport[9] = (unsigned char)lmuinfo.Address.UnitAddr;
			pReport[10] = (unsigned char)(lmuinfo.Address.UnitAddr>>8);
//			pReport[11] = (unsigned char)lmuinfo.Address.HostAddr;
			
			if(cType == 38)
			{
			//	compareCHAR
				int iSerial;
				int nReportLen = 18;
				char szSeria[12];
			//	MakeSerialFrame(pReport,(BYTE*)param1);
				iSerial=MakeSetSERFrame(pReport,strAddr);
				if(iSerial<0)
					continue;
					
				sprintf(szSeria,"%08d",iSerial);
				memcpy(pReport+nReportLen, (BYTE*)szSeria,8);
				nReportLen+=8;
			//	int nCrc16 =CalcCRC16_r(pReport+18,8);
			//	memcpy(compareCHAR,)

			}
			if(cType == 40)
			{
			//	compareCHAR
				 
				int iSerial;
				int nReportLen = 18;
				char szSeria[32];
				memset(szSeria, 0,32);

				iSerial=MakeSetSERFrame(pReport,strAddr);//获得序列号
				
				strncpy(szSeria,m_CDMAsERIA.m_sz4,4);
				sprintf(szSeria+4,"%08d",iSerial);		
			//	MakeSerialFrame(pReport,(BYTE*)param1);
				
				char sxNx[32];
				memset(sxNx, 0,32);
				for(int m=0;m<12;m++)
				{
					sxNx[m]= szSeria[11-m];

					
				}
				//
			
				pReport[nReportLen++] = 1;
				memcpy(pReport+nReportLen, (BYTE*)sxNx,32);				
				memcpy(pReport+nReportLen+32, (BYTE*)szSeria,32);	
				nReportLen+=64;
				int nCrc16 =CalcCRC16_r(pReport+18,8);

			}


			if(cType == 4)
			{
				//读取电池电压，代码待写。。。
			}

			if(cType == 7)
			{
				pReport[14] = (unsigned char)param1;
				cTemp = (((unsigned char)param2) << 4) | ((unsigned char)param3);
				pReport[18] = cTemp;
			}

			if(cType == 17)
			{
				IPPORTINFO* p = (IPPORTINFO*)param1;
				sockaddr_in sa;
				int nReportLen = 18;

				sa.sin_addr.s_addr=inet_addr((char*)p->m_strEndIP);	
				sscanf((char*)p->m_strEndIP,"%u.%u.%u.%u",
					&sa.sin_addr.s_net,
					&sa.sin_addr.s_host,
					&sa.sin_addr.s_lh,
					&sa.sin_addr.s_impno);
				pReport[nReportLen++] = sa.sin_addr.s_net;
				pReport[nReportLen++] = sa.sin_addr.s_host;
				pReport[nReportLen++] = sa.sin_addr.s_lh;
				pReport[nReportLen++] = sa.sin_addr.s_impno;
				memcpy((void*)&pReport[nReportLen],&p->m_nEndPort,2);
				nReportLen += 2;
				
				sa.sin_addr.s_addr=inet_addr((char*)p->m_strEndIP2);
				sscanf((char*)p->m_strEndIP2,"%u.%u.%u.%u",
					&sa.sin_addr.s_net,
					&sa.sin_addr.s_host,
					&sa.sin_addr.s_lh,
					&sa.sin_addr.s_impno);
				pReport[nReportLen++] = sa.sin_addr.s_net;
				pReport[nReportLen++] = sa.sin_addr.s_host;
				pReport[nReportLen++] = sa.sin_addr.s_lh;
				pReport[nReportLen++] = sa.sin_addr.s_impno;
				memcpy((void*)&pReport[nReportLen],&p->m_nEndPort2,2);
				nReportLen += 2;
				if(g_RtuType == TERMINAL_TYPE_FK)
				{
					nReportLen += 12;
				}
				CString strAPN(p->m_nEndAPN);
				strAPN.TrimLeft();
				strAPN.TrimRight();
				memset((void*)&pReport[nReportLen],0,16);
				strncpy((char*)&pReport[nReportLen],(LPCTSTR)strAPN,16);
			}

			if(cType == 18)
			{
				CString strTmp;
				int	nTmp;
				unsigned char cTmp;
				CTime tm = CTime::GetCurrentTime();
				
				pReport[23] = GetBcd((unsigned char)(tm.GetYear() % 100));
				cTmp = GetBcd((unsigned char)tm.GetMonth());
				nTmp = tm.GetDayOfWeek();
				if(nTmp == 1)
					nTmp = 7;
				else
					nTmp--;
				cTmp |= ((unsigned char)nTmp)<<5;
				pReport[22] = cTmp;

				pReport[21] = GetBcd((unsigned char)tm.GetDay());
				pReport[20] = GetBcd((unsigned char)tm.GetHour());
				pReport[19] = GetBcd((unsigned char)tm.GetMinute());
				pReport[18] = GetBcd((unsigned char)tm.GetSecond());
			}

			if ((cType == 29)||(cType == 30)||(cType == 31))
			{
				pReport[42] = CalcCS_r(pReport+28,14);
			}
			
			if (cType == 32) // 设置mac地址
			{
/*				
				int nReportLen = 18;
				pReport[nReportLen++] = 0x55;
				pReport[nReportLen++] = 0x55;
				pReport[nReportLen++] = g_mac[0];
				pReport[nReportLen++] = g_mac[1];
				pReport[nReportLen++] = g_mac[2];
				pReport[nReportLen++] = g_mac[3];
				pReport[nReportLen++] = g_mac[4];
				pReport[nReportLen++] = g_mac[5];
#if 0
				int nReportLen = 18;
				pReport[nReportLen++] = 0x55;
				pReport[nReportLen++] = 0x55;
				pReport[nReportLen++] = 0x08;
				pReport[nReportLen++] = 0x00;
				pReport[nReportLen++] = rtuAddress[1];
				pReport[nReportLen++] = rtuAddress[0];
				pReport[nReportLen++] = rtuAddress[3];
				pReport[nReportLen++] = rtuAddress[2];

#endif
			//	memcpy(pReport+nReportLen,param1,6);
			//	nReportLen += 6;
				int nCrc16 =CalcCRC16_r(pReport+18,8);

				memcpy(pReport+nReportLen,&nCrc16,2);
				nReportLen += 2;
				pReport[nReportLen++] = 0xAA;
				pReport[nReportLen++] = 0xAA;
*/
				
		
			
				strTmp="";
				int rtn = MakeSetMacFrame(pReport,strAddr);
				switch(rtn)
				{
				case -1:
					strTmp="终端地址不匹配";
					break;
				case -2:
					strTmp="终端地址超出范围";
					break;
				default:
					break;
				}
	
				if(rtn < 0)
				{
					m_AddressList.SetItemText(i, 2, strTmp);
					continue;
				}
			}
			if (cType == TYPE_SET_CDMA) // 
			{
				int nReportLen = 18;
				pReport[nReportLen++] = 1;
				memcpy(pReport+nReportLen, (BYTE*)param1+32,32);				
				memcpy(pReport+nReportLen+32, (BYTE*)param1,32);	
				nReportLen+=64;
				int nCrc16 =CalcCRC16_r(pReport+18,8);
			}

			if (cType == TYPE_SET_F8) // 
			{
				int nReportLen = 18;	
				F8Para *p1 = (F8Para *)param1;
				pReport[nReportLen++] = *((BYTE*)param1);
				
				memcpy(pReport+nReportLen,&(p1->nIntervalForever),2);	
				nReportLen+=2;
				pReport[nReportLen++] = p1->nRetryPassive;
				pReport[nReportLen++] = p1->nIntervalPassive;
	
				memcpy(pReport+nReportLen,&(p1->nFlgOnline),3);
				nReportLen+=3;	

				int nCrc16 =CalcCRC16_r(pReport+18,8);
			}
			if (cType == TYPE_SET_225) // 
			{
				int nReportLen = 18;	
				memcpy(pReport+nReportLen, (BYTE*)param1,8);
				nReportLen+=8;
			//	int nCrc16 =CalcCRC16_r(pReport+18,8);
			}
		

			memcpy(m_conData.m_sendBuf,pReport,len);
#if 0
			if((bPwdLong)&&(pReport[12]!=0x09)&&(pReport[12]!=0x0c)&&(pReport[12]!=0xf1))
			{
				memcpy((VOID *)(&Dtlen), (VOID *)(pReport+1), 2);
				Dtlen >>= 2;
				Dtlen += 14;
				int tmplen = (Dtlen<<2)|0x02; // 新规约用2表示本规约
				memcpy((VOID *)(m_conData.m_sendBuf+1), (VOID *)(&tmplen), 2);
				memcpy((VOID *)(m_conData.m_sendBuf+3), (VOID *)(&tmplen), 2);
				len += 14;
				memset(m_conData.m_sendBuf+len-18,0,16);
			}
#endif
			int templen = len;
	
			int tmplen = 0;
			memcpy((VOID *)(&Dtlen), (VOID *)(pReport+1), 2);
			Dtlen >>= 2;
			if(bPwdLong)
			{
				if((pReport[12]!=0x09)&&(pReport[12]!=0x0c)&&(pReport[12]!=0xf1))	
				{
					Dtlen += 14;

					tmplen = (Dtlen<<2)|0x02; // 新规约用2表示本规约
					memcpy((VOID *)(m_conData.m_sendBuf+1), (VOID *)(&tmplen), 2);
					memcpy((VOID *)(m_conData.m_sendBuf+3), (VOID *)(&tmplen), 2);
					templen += 14;
					memset(m_conData.m_sendBuf+templen-18,0,16);
				}
				else
				{
					tmplen = (Dtlen<<2)|0x02; // 新规约用2表示本规约
					memcpy((VOID *)(m_conData.m_sendBuf+1), (VOID *)(&tmplen), 2);
					memcpy((VOID *)(m_conData.m_sendBuf+3), (VOID *)(&tmplen), 2);

				}
			}

			// 长地址的处理
			if(strlen(strAddr)==12)
			{
				memmove(m_conData.m_sendBuf+13,m_conData.m_sendBuf+11,templen-11);
				memcpy(m_conData.m_sendBuf+9,rtuAddress+2,4);
				Dtlen += 2;
				tmplen = (Dtlen<<2)|0x02; 
				memcpy((VOID *)(m_conData.m_sendBuf+1), (VOID *)(&tmplen), 2);
				memcpy((VOID *)(m_conData.m_sendBuf+3), (VOID *)(&tmplen), 2);
				templen += 2;
				g_bAddrLong = TRUE;
			}
			else
				g_bAddrLong = FALSE;

			m_conData.m_sendBuf[templen - 2] = CalcCS_r((unsigned char*)&m_conData.m_sendBuf[6],templen - 8);
			m_conData.m_sendBuf[templen - 1] = 0x16;


			
			m_conData.m_recLength = 0;
			if(cType != 40&&cType != 41)
			{
				m_AddressList.SetItemText(i, 2, "");	
			}
				

			for(int j = 0 ; j < 1 ; j++)
			{
				
				m_conData.m_sendLength = templen;
				SetEvent(m_conData.m_sendEvent);												

				//set edit
				CTime tm = CTime::GetCurrentTime();
				AddStr2Edit("\n");
				strTmp.Format("串口发送(%d-%02d-%02d %02d:%02d:%02d):", tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
	
				AddStr2Edit(strTmp);

				strTmp = "";
				for( k = 0 ; k < templen ; k++)
				{
					strTmp1.Format("%02X ", (BYTE)m_conData.m_sendBuf[k]);
					strTmp += strTmp1;
				}
				AddStr2Edit(strTmp);

				
				

				do 
				{
					tmp = WaitForSingleObject(m_conData.m_recEvent ,6000);
					
					if(tmp == WAIT_TIMEOUT)
					{
						break;
					}
					
					EnterCriticalSection(&m_conData.m_recCS);
					analy= AnalyticData((unsigned char*)m_conData.m_recBuf , m_conData.m_recLength,i);
					ResetEvent(m_conData.m_recEvent);
					LeaveCriticalSection(&m_conData.m_recCS);
					
				} while(analy == ANALY_DATA_LESS);
				
				if(tmp == WAIT_TIMEOUT)
					continue;				
				
				
				if(analy == ANALY_RIGHT)
				{			
					break;
				}
			}


			if(j == 1)
			{
				m_AddressList.SetItemText(i, 2, "发送数据超时.");
				//AfxMessageBox("发送数据超时.");
				EnableAllButton(1);
				if (g_RtuType == TERMINAL_TYPE_FK)
				{
				//	GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
//					GetDlgItem(IDC_BUTTON7)->EnableWindow(TRUE);
				//	GetDlgItem(IDC_BUTTON19)->EnableWindow(TRUE);
				//	GetDlgItem(IDC_BUTTON22)->EnableWindow(FALSE);
				}
				else
				{
				//	GetDlgItem(IDC_BUTTON4)->EnableWindow(0);
//					GetDlgItem(IDC_BUTTON7)->EnableWindow(0);
				//	GetDlgItem(IDC_BUTTON19)->EnableWindow(0);
				//	GetDlgItem(IDC_BUTTON22)->EnableWindow(1);

				}
				g_PortInfo = FALSE;
				
				return;
			}

			if (m_conData.m_recBuf[12]==0
				&&m_conData.m_recBuf[17]==0)
			{
				if (m_conData.m_recBuf[16]==1)
				{
					strTmp="全部确认！";
				} 
				else
				{
					strTmp="全部否认！";
				}
			
				m_AddressList.SetItemText(i, 2, strTmp);
				continue;
			}


			if(cType == 20)
			{
				strTmp = "";
/*
				cTemp = m_conData.m_recBuf[18];
				for(j = 0 ; j < 8 ; j++)
				{
					if(cTemp & (1 << j))
						strTmp += "合";
					else
						strTmp += "分";
				}
				
				strTmp += " ";	
*/
				cTemp = m_conData.m_recBuf[19];
				for(j = 0 ; j < 8 ; j++)
				{
					if(cTemp & (1 << j))
						strTmp += "√";
					else
						strTmp += "×";
				}

				m_AddressList.SetItemText(i, 2, strTmp);
			}
			
			if (cType == TYPE_SET_CDMA) // 
			{
				if (m_conData.m_recBuf[18]==0) 
				{
					strTmp = "设置CDMA参数 成功 ！";
				}
				else
				{
					strTmp = "设置CDMA参数 失败 ！";
				}
				m_AddressList.SetItemText(i, 2, strTmp);
			}

			if (cType == TYPE_GET_CDMA) // 
			{
				if (m_conData.m_recBuf[18]==0) 
				{	
					char tmp[32+1]={0};
					strTmp = "用户名:";
					memcpy(tmp,m_conData.m_recBuf+19+32,32);
					strTmp += (char*)tmp;
					strTmp += "    密码:";
					memcpy(tmp,m_conData.m_recBuf+19,32);
					strTmp += (char*)tmp;
					strTmp += "    ";
				}
				else
				{
					strTmp = "读取失败";
				}


				m_AddressList.SetItemText(i, 2, strTmp);
			}
			if (cType == 39) // 
			{
				char tmp[32+1]={0};
			//	strTmp = "条码:";
				memcpy(tmp,m_conData.m_recBuf+22,8);
				
				int it = atoi(tmp);
				if (it== MakeSetSERFrame(pReport,strAddr)) 
				{	
				
					strTmp = "对比编码成功 ";
				}
				else
				{
					strTmp = "对比编码失败 ";
				}


				m_AddressList.SetItemText(i, 2, strTmp);
			}

			if (cType == TYPE_CLR_AC) // 
			{
				if (m_conData.m_recBuf[18]==0) 
				{
					strTmp = "清交采表底 成功 ！";
				}
				else
				{
					strTmp = "清交采表底 失败 ！";
				}
				m_AddressList.SetItemText(i, 2, strTmp);
			}
			if (cType == 41) // 
			{	char szTT[100];
				m_AddressList.GetItemText(i, 2,szTT,100);
				strTmp=szTT;
				if (m_conData.m_recBuf[18]==0) 
				{	
					char tmp[32+1]={0};
					strTmp += "用户名:";
					memcpy(tmp,m_conData.m_recBuf+19+32,32);
					strTmp += (char*)tmp;
					strTmp += "    密码:";
					memcpy(tmp,m_conData.m_recBuf+19,32);
					strTmp += (char*)tmp;
					strTmp += "    ";
				}
				else
				{
					strTmp += "CDMA参数对比失败";
				}


				m_AddressList.SetItemText(i, 2, strTmp);
			}
			if (cType == TYPE_GET_F8) // 
			{
				BYTE TempBuff[8];	
				F8Para *p1 = (F8Para *)param1;
				TempBuff[0] = *((BYTE*)param1);
				
				memcpy(TempBuff+1,&(p1->nIntervalForever),2);	
				TempBuff[3] = p1->nRetryPassive;
				TempBuff[4] = p1->nIntervalPassive;
				memcpy(TempBuff+5,&(p1->nFlgOnline),3);
				
				if (memcmp(TempBuff,m_conData.m_recBuf+18,8)==0)
				{
					strTmp = "参数F8 一致！";
				}
				else
				{
					strTmp = "参数F8 不一致！";
				}
				m_AddressList.SetItemText(i, 2, strTmp);

			}

		}
	}

	EnableAllButton(1);
	if (g_RtuType == TERMINAL_TYPE_FK)
	{
//		GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
//		GetDlgItem(IDC_BUTTON7)->EnableWindow(TRUE);
///		GetDlgItem(IDC_BUTTON19)->EnableWindow(TRUE);
//		GetDlgItem(IDC_BUTTON22)->EnableWindow(FALSE);
	}
	else
	{
//		GetDlgItem(IDC_BUTTON4)->EnableWindow(0);
//		GetDlgItem(IDC_BUTTON7)->EnableWindow(0);
//		GetDlgItem(IDC_BUTTON19)->EnableWindow(0);
//		GetDlgItem(IDC_BUTTON22)->EnableWindow(1);

	}
	g_PortInfo = FALSE;


}

void CFkSetDlg::OnButton1() 
{
	if(g_RtuType == TERMINAL_TYPE_FK)
	{
		SetCheckTmu(1,SendData1,sizeof(SendData1));
	}
	else
	{
		SetCheckTmu(1,SendData6981,sizeof(SendData6981),g_bPwdLong);
	}
}

void CFkSetDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	if(g_RtuType == TERMINAL_TYPE_FK)
	{
		SetCheckTmu(2,SendData2,sizeof(SendData2));
	}
	else
	{
		UpdateData(TRUE);

		SendData6982[24] = (SendData6982[24]&0xf0)|(m_n4851Port&0x0f);
		SetCheckTmu(2,SendData6982,sizeof(SendData6982),g_bPwdLong);
	}

}

void CFkSetDlg::OnButton3() 
{ 
	SetCheckTmu(3,SendData6983_2,sizeof(SendData6983_2),g_bPwdLong);
}

void CFkSetDlg::OnButton4() 
{



	SetCheckTmu(1,ReadBattVol,sizeof(ReadBattVol) ,g_bPwdLong);
	//AfxMessageBox("读取电池电压，代码待写");

}

void CFkSetDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	
	if(g_RtuType == TERMINAL_TYPE_FK)
	{
		SetCheckTmu(5,SendData5,sizeof(SendData5));
	}
	else
	{
		SetCheckTmu(5,SendData6985,sizeof(SendData6985),g_bPwdLong);
	}
}

void CFkSetDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	//SetCheckTmu(6,SendData6,sizeof(SendData6));
	if(g_RtuType == TERMINAL_TYPE_FK)
	{
		SetCheckTmu(6,SendData6,sizeof(SendData6));
	}
	else
	{
		SetCheckTmu(6,SendData6986,sizeof(SendData6986),g_bPwdLong);
	}
}

void CFkSetDlg::OnButton7() 
{
	CSetAlarm dlg;
	CString strTmp;

	if(dlg.DoModal() == IDOK)
	{
		int nAlarm =1<<(dlg.m_nAlarm);

		int nDelay = dlg.m_nDelay;

		int nTime = dlg.m_nTime;

		//SetCheckTmu(7,SendData7,sizeof(SendData7),(void*)nAlarm, (void*)nDelay, (void*)nTime);
		if(g_RtuType == TERMINAL_TYPE_FK)
		{
			SetCheckTmu(7,SendData7,sizeof(SendData7),FALSE,(void*)nAlarm, (void*)nDelay, (void*)nTime);
		}
		else
		{
			SetCheckTmu(7,SendData6987,sizeof(SendData6987),g_bPwdLong,(void*)nAlarm, (void*)nDelay, (void*)nTime);
		}
	}	
}

void CFkSetDlg::OnButton8() 
{
	m_ctlResult.SetWindowText("");
}

void CFkSetDlg::OnButton9() 
{
	// TODO: Add your control notification handler code here
	//SetCheckTmu(9,SendData9,sizeof(SendData9));
	if(g_RtuType == TERMINAL_TYPE_FK)
	{
		SetCheckTmu(9,SendData9,sizeof(SendData9));
	}
	else
	{
		SetCheckTmu(9,SendData6989,sizeof(SendData6989),g_bPwdLong);
	}
}

void CFkSetDlg::OnButton10() 
{
	// TODO: Add your control notification handler code here
	//SetCheckTmu(10,SendData10,sizeof(SendData10));
		if(g_RtuType == TERMINAL_TYPE_FK)
	{
		SetCheckTmu(10,SendData10,sizeof(SendData10));
	}
	else
	{
		SetCheckTmu(10,SendData69810,sizeof(SendData69810),g_bPwdLong);
	}
}

void CFkSetDlg::OnButton11() 
{
	// TODO: Add your control notification handler code here
	//SetCheckTmu(11,SendData11,sizeof(SendData11));
		if(g_RtuType == TERMINAL_TYPE_FK)
	{
		SetCheckTmu(11,SendData11,sizeof(SendData11));
	}
	else
	{
		SetCheckTmu(11,SendData69811,sizeof(SendData69811),g_bPwdLong);
	}
}

void CFkSetDlg::OnButton12() 
{
	// TODO: Add your control notification handler code here
	//SetCheckTmu(12,SendData12,sizeof(SendData12));
		if(g_RtuType == TERMINAL_TYPE_FK)
	{
		SetCheckTmu(12,SendData12,sizeof(SendData12));
	}
	else
	{
		SetCheckTmu(12,SendData69812,sizeof(SendData69812),g_bPwdLong);
	}
}

void CFkSetDlg::OnButton13() 
{
	// TODO: Add your control notification handler code here
	//SetCheckTmu(13,SendData13,sizeof(SendData13));
		if(g_RtuType == TERMINAL_TYPE_FK)
	{
		SetCheckTmu(13,SendData13,sizeof(SendData13));
	}
	else
	{
		SetCheckTmu(13,SendData69813,sizeof(SendData69813),g_bPwdLong);
	}
}

void CFkSetDlg::OnButton14() 
{
	// TODO: Add your control notification handler code here
	//SetCheckTmu(14,SendData14,sizeof(SendData14));
		if(g_RtuType == TERMINAL_TYPE_FK)
	{
		SetCheckTmu(14,SendData14,sizeof(SendData14));
	}
	else
	{
		SetCheckTmu(14,SendData69814,sizeof(SendData69814),g_bPwdLong);
	}
}

void CFkSetDlg::OnButton15() 
{
	// TODO: Add your control notification handler code here
	//SetCheckTmu(15,SendData15,sizeof(SendData15));
		if(g_RtuType == TERMINAL_TYPE_FK)
	{
		SetCheckTmu(15,SendData15,sizeof(SendData15));
	}
	else
	{
		SetCheckTmu(15,SendData69815,sizeof(SendData69815),g_bPwdLong);
	}
}

void CFkSetDlg::OnButton16() 
{
	
	SetCheckTmu(16,SendData69816,sizeof(SendData69816),g_bPwdLong);
	
}

void CFkSetDlg::OnCheck1() 
{
	CButton* pWin = (CButton*)GetDlgItem(IDC_CHECK1);
	BOOL bCheck = pWin->GetCheck();

	for(int i = 0 ; i < m_AddressList.GetItemCount() ; i++)
	{
		m_AddressList.SetCheck(i,bCheck);
	}
}

void CFkSetDlg::OnButton17() 
{
	CSetIPAndPort dlg;
	if(dlg.DoModal() == IDOK)
	{		
		
		SetCheckTmu(17,SendData69817,sizeof(SendData69817),g_bPwdLong,(void*)&dlg.m_data);
	}	
}


//对时
void CFkSetDlg::OnButton18() 
{
	//SetCheckTmu(18,SendData18,sizeof(SendData18));
		if(g_RtuType == TERMINAL_TYPE_FK)
	{
		SetCheckTmu(18,SendData18,sizeof(SendData18));
	}
	else
	{
		SetCheckTmu(18,SendData69818,sizeof(SendData69818),g_bPwdLong);
	}
}

void CFkSetDlg::OnButton19() 
{
	//SetCheckTmu(19,SendData19,sizeof(SendData19));	
/*	if(g_RtuType == TERMINAL_TYPE_FK)
	{
		SetCheckTmu(19,SendData19,sizeof(SendData19));
	}
	else
	{
		SetCheckTmu(19,SendData69819,sizeof(SendData69819),g_bPwdLong);
	}*/
	CDlgSetParaF8 dlg;
	if(dlg.DoModal() == IDOK)
	{		
		//SetCheckTmu(17,SendData17,sizeof(SendData17),(void*)&dlg.m_data);
		if (dlg.m_bSet)
		{
			SetCheckTmu(TYPE_SET_F8,SendDataSetF8,sizeof(SendDataSetF8),FALSE,(void*)&dlg.m_para);
		} 
		else
		{
			SetCheckTmu(TYPE_GET_F8,SendDataGetF8,sizeof(SendDataGetF8),FALSE,(void*)&dlg.m_para);
		}

	}
}

void CFkSetDlg::OnButton20() 
{
	//SetCheckTmu(20,SendData20,sizeof(SendData20));	
		if(g_RtuType == TERMINAL_TYPE_FK)
	{
		SetCheckTmu(20,SendData20,sizeof(SendData20));
	}
	else
	{
		SetCheckTmu(20,SendData69820,sizeof(SendData69820),g_bPwdLong);
	}
}


void CFkSetDlg::OnButton21() 
{
	//SetCheckTmu(21,SendData21,sizeof(SendData21));	
		if(g_RtuType == TERMINAL_TYPE_FK)
	{
		SetCheckTmu(21,SendData21,sizeof(SendData21));
	}
	else
	{
		SetCheckTmu(21,SendData69821,sizeof(SendData69821),g_bPwdLong);
	}
}

unsigned char CFkSetDlg::GetBcd(unsigned char uSrc)
{
	unsigned char uDest;
	int nTmp;
	nTmp = uSrc % 10;

	uDest = 0;
	uDest = nTmp;
	
	nTmp = uSrc / 10;
	
	uDest |= (unsigned char)nTmp<<4;
	return uDest;
}

int CFkSetDlg::AnalyticData(unsigned char* pReport, unsigned short& nReportLen,int nItem)
{
	CString strTmp, strTmp1;
	int k;
	BYTE crc = 0;
	unsigned short datalen;
	if( nReportLen < MIN_SERVER_PACKET_LEN)//接收数据长度小于最小桢长度
	{
		return ANALY_DATA_LESS;//不予处理,等待接收数据长度大于等于MIN_SERVER_PACKET_LEN
	}
	switch(pReport[0])
	{
	case 0x68:		

		if((*(pReport+1) != *(pReport+3))||(*(pReport+2) != *(pReport+4)))
		{ 
//			WriteLog("两个长度不相等!");
//			find_SOH(pReport,nReportLen,1);
			return ANALY_WRONG;
		}
		//判断规约标识
	//	if((*(pReport+1)&0x03)!=0x01)	/*规约标识不正确*/	
	//	{
//			WriteLog("规约标识不正确!");
//			find_SOH(pReport,nReportLen,1);
	//		return ANALY_WRONG;			
	//	}
		memcpy((VOID *)(&datalen), (VOID *)(pReport+1), 2);
		datalen>>=2;
		if(nReportLen <(datalen+8))		//检测长度
		{
			return ANALY_DATA_LESS; 
		}		
		crc = CalcCS_r(pReport+6,datalen);//控制域到应用层

		if(pReport[datalen+7] != 0x16)
		{
//			find_SOH(pReport,nReportLen,1);
			return ANALY_WRONG;
		}
		if(pReport[datalen+6] != crc)
		{
//			WriteLog("校验和不对!");
//			find_SOH(pReport,nReportLen,1);
			return ANALY_WRONG;
		}
		break;
	default:
//		find_SOH(pReport,nReportLen,1);
		return ANALY_WRONG;
	}

	LogHex("read->",(char*)pReport,datalen+8);

	//set edit
	CTime tm = CTime::GetCurrentTime();
	strTmp.Format("串口接收(%d-%02d-%02d %02d:%02d:%02d):", tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
	AddStr2Edit(strTmp);

	strTmp = "";
	for( k = 0 ; k < datalen+8 ; k++)
	{
		strTmp1.Format("%c", (BYTE)pReport[k]);
		strTmp += strTmp1;
	}
	AddStr2Edit(strTmp);

	strTmp = "";
	for( k = 0 ; k < datalen+8 ; k++)
	{
		strTmp1.Format("%02X ", (BYTE)pReport[k]);
		strTmp += strTmp1;
	}
	AddStr2Edit(strTmp);
//	AddStr2Edit("\r\n");
	// 解析
	if(g_PortInfo)
		AnalyticPortinfo(pReport+18,datalen+8-18-2);
//	g_PortInfo = FALSE;

	int lenstart=12;
	if(g_bAddrLong)
		lenstart = 12+2;
	
	if (pReport[lenstart] == 0x09)
	{
		if ((pReport[lenstart+4] == 0x80)&&(pReport[lenstart+5] == 0x0a))
		{
				char strtmp[50];
				sprintf(strtmp,"内部版本%s",pReport+lenstart+6);
				AddStr2Edit(strtmp);
		}
	}
	else if (pReport[lenstart] == 0x10) // afn10
	{
		AnalyticAFN10(pReport+lenstart+6,datalen+8-18-2);
	}
	else if (pReport[lenstart] == 0xf1) // afn10
	{
		if ((pReport[lenstart+4] == 0x01)&&(pReport[lenstart+5] == 0x14))
		{
			if (pReport[lenstart+6] == 0x00)
			{
				CString temp="设置mac地址成功!";
				AddStr2Edit("设置mac地址成功!");
				m_AddressList.SetItemText(nItem, 2, temp);
			}
		}
		if ((pReport[lenstart+4] == 0x02)&&(pReport[lenstart+5] == 0x14))
		{
			if (pReport[lenstart+6] == 0x00)
			{
				char strtmp[50];
				sprintf(strtmp,"MAC地址：%02X-%02X-%02X-%02X-%02X-%02X",
					pReport[lenstart+7],pReport[lenstart+8],pReport[lenstart+9],pReport[lenstart+10],pReport[lenstart+11],pReport[lenstart+12]);
				AddStr2Edit(strtmp);
				m_AddressList.SetItemText(nItem, 2, strtmp);
			}
		}

	}

	find_SOH(pReport,nReportLen,datalen+8);
	return ANALY_RIGHT;	
}

// 应用层解析
int CFkSetDlg::AnalyticPortinfo(unsigned char* pBuf, unsigned short nAppLen)
{
	CString strTmp, strTmp1;
	int k = 0;
	
	if(nAppLen < 17) return -1;

	strTmp = "";
	strTmp1.Format("脉冲量输入路数: %d\r\n",pBuf[0]);
	strTmp += strTmp1;
	strTmp1.Format("开关量输入路数: %d\r\n",pBuf[1]);
	strTmp += strTmp1;
	strTmp1.Format("直流模拟量量输入路数: %d\r\n",pBuf[2]);
	strTmp += strTmp1;
	strTmp1.Format("开关量输出路数(轮次): %d\r\n",pBuf[3]);
	strTmp += strTmp1;
	strTmp1.Format("支持的抄电能表/交采最多个数: %d\r\n",*(short*)(&pBuf[4]));
	strTmp += strTmp1;
	strTmp1.Format("支持的终端上行通信最大接收缓冲区字节数: %d\r\n",*(short*)(&pBuf[6]));
	strTmp += strTmp1;
	strTmp1.Format("支持的终端上行通信最大发送缓冲区字节数: %d\r\n",*(short*)(&pBuf[8]));
	strTmp += strTmp1;

	strTmp1.Format("终端MAC地址1段: %d\r\n",pBuf[10]);
	strTmp += strTmp1;
	strTmp1.Format("终端MAC地址2段: %d\r\n",pBuf[11]);
	strTmp += strTmp1;
	strTmp1.Format("终端MAC地址3段: %d\r\n",pBuf[12]);
	strTmp += strTmp1;
	strTmp1.Format("终端MAC地址4段: %d\r\n",pBuf[13]);
	strTmp += strTmp1;
	strTmp1.Format("终端MAC地址5段: %d\r\n",pBuf[14]);
	strTmp += strTmp1;
	strTmp1.Format("终端MAC地址6段: %d\r\n",pBuf[15]);
	strTmp += strTmp1;
	
	strTmp1.Format("通信端口数量: %d\r\n",pBuf[16]);
	strTmp += strTmp1;

	unsigned char cTmp = 0;
	for( k = 0 ; k < (nAppLen-17)/12; k++)
	{
		cTmp = pBuf[17+k*12];
		strTmp1.Format("---端口 %d:",cTmp&0x1f);
		strTmp += strTmp1;
		switch((cTmp&0x30)>>5) // bit 5,6
		{
			case 0:
				strTmp1="RS485接口,";
				
				break;
			case 1:
				strTmp1="RS232接口,";
				break;
			case 2:
				strTmp1="串行接口连接窄带低压载波通信模块,";
				break;
			default:
				strTmp1="";
				break;

		}
		strTmp += strTmp1;
		
		cTmp = (pBuf[18+k*12]&0xE0)>>5; // bit 13_15

		switch(cTmp)
		{
			case 0:				
				strTmp1="专变、公变抄表";
				break;
			case 1:
				strTmp1="变电站抄表";

				break;
			case 2:
				strTmp1="台区低压抄表";

				break;
			case 3:
				strTmp1="当地用户侧数据共享";

				break;
			default:
				strTmp1="";
				break;
		}
		strTmp += strTmp1+"\r\n";

		// 
		strTmp1.Format("支持的最高波特率:%ld:\r\n",*(int*)&pBuf[19+12*k]);
		strTmp += strTmp1;
		strTmp1.Format("设备个数:%d:\r\n",*(short*)&pBuf[23+12*k]);
		strTmp += strTmp1;
		strTmp1.Format("最大接收缓冲区字节数:%d:\r\n",*(short*)&pBuf[25+12*k]);
		strTmp += strTmp1;
		strTmp1.Format("最大发送缓冲区字节数:%d:\r\n",*(short*)&pBuf[27+12*k]);
		strTmp += strTmp1+"\r\n";
	
	}

	
	AddStr2Edit(strTmp);
	return 0;
}

// 应用层解析
int CFkSetDlg::AnalyticAFN10(unsigned char* pBuf, unsigned short nAppLen)
{
	CString strTmp, strTmp1;
	int k = 0;
	
	if(nAppLen < 17) return -1;

	int rlen = pBuf[1]+pBuf[2]*256;

	if ( rlen >= 20)
	{
		char result[10];
		int i = 0;
		for( i=0;i<4;i++)
		{
			pBuf[3+rlen-6+i] -= 0x33;
			result[3-i] = (pBuf[3+rlen-6+i]/16*10)+pBuf[3+rlen-6+i]%16;
		}
		strTmp.Format("端口 %d  透抄结果：",pBuf[0]);
		for(i=0;i<4;i++)
		{
			strTmp1.Format("%02d",result[i]);
			strTmp += strTmp1;
		}
		strTmp1.Format("---透抄成功!---\r\n",pBuf[10]);
		strTmp += strTmp1;

	}
	
	AddStr2Edit(strTmp);
	return 0;
}


// 应用层解析
//int CFkSetDlg::AnalyticAFNF1(unsigned char* pBuf, unsigned short nAppLen)
//{
//	return 0;
//}

void CFkSetDlg::find_SOH(unsigned char* pReport, unsigned short& nReportLen,int i)
{
	int j;

	if(i==0)
	{
		return;
	}

	while((pReport[i] != 0x68)&&(i < nReportLen))
	{
		i++;
	}

	for(j=0; i<nReportLen; i++, j++)		//进行搬动
	{
		pReport[j] = pReport[i];
	}

	nReportLen = j;	//修改有效长度
	return;
}

void CFkSetDlg::RefreshNum()
{
	CString strTmp;
	for(int i = 0 ; i < m_AddressList.GetItemCount() ; i++)
	{			
		strTmp.Format("%d",i+1);
		m_AddressList.SetItemText(i,0,strTmp);			
	}	
}

void CFkSetDlg::EnableAllButton(BOOL b)
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON7)->EnableWindow(b);	
	GetDlgItem(IDC_BUTTON9)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON10)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON11)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON12)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON13)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON14)->EnableWindow(b);
	//GetDlgItem(IDC_BUTTON15)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON16)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON17)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON18)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON19)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON20)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON21)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON22)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON23)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON24)->EnableWindow(b);
	//GetDlgItem(IDC_BUTTON25)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON26)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON27)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON28)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON32)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON33)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON34)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON35)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_DOOR)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON34)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON36)->EnableWindow(b);
	GetDlgItem(IDC_BUTTON_CLRAC)->EnableWindow(b);

}

void CFkSetDlg::AddStr2Edit(const CString& str)
{
	int nLen = m_ctlResult.GetWindowTextLength();
	if (nLen >= 30000 - 1)
	{
		m_ctlResult.SetWindowText("");
	}

	m_ctlResult.SetSel(nLen,nLen,NULL);   
	m_ctlResult.ReplaceSel(str + "\r\n"); 
}

void CFkSetDlg::LoadParamFile()
{
	CFile f;
	CFileException e;
	char path[256];
	char strTmp1[16];
	int nCount;
	CString strTmp;

	GetModulePath(path);
	strcat(path,"\\FkSetInfo.cfg");

	if( !f.Open( path,CFile::modeReadWrite, &e ) )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
/*
		strTmp1[0] = '2';		
		strTmp1[1] = '\0';
		SetDlgItemText(IDC_COMBO_PLUSENUM,strTmp1);	*/
		strTmp1[0] = '2';		
		strTmp1[1] = '\0';	
		m_PluseNum.SelectString(0,strTmp1);
		return;
	}

	f.Read(&nCount,sizeof(nCount));
	
	for(int i = 0 ; i < nCount ; i++)
	{
		f.Read(strTmp1,13);
		

		strTmp.Format("%d",i + 1);
		int index = m_AddressList.InsertItem(m_AddressList.GetItemCount(),strTmp);
		m_AddressList.SetItemText(index,1,strTmp1);		
	}
	memset(strTmp1,0,sizeof(strTmp1));
	f.Read(strTmp1,2);
	int n = atol(strTmp1);
	if(!(n > 0 && n <=4))
	{
		strTmp1[0] = '2';		
		strTmp1[1] = '\0';
		
	}
	
	m_PluseNum.SelectString(0,strTmp1);	
	

		

	f.Close();
}

void CFkSetDlg::SaveParamFile()
{
	CFile f;
	CFileException e;
	char path[256];
	CString strTmp;
	int nCount;


	GetModulePath(path);
	strcat(path,"\\FkSetInfo.cfg");

	if( !f.Open( path, CFile::modeCreate | CFile::modeReadWrite, &e ) )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		return;
	}
	
	nCount = m_AddressList.GetItemCount();
	f.Write(&nCount,sizeof(nCount));

	for(int i = 0 ; i < nCount ; i++)
	{
		strTmp = m_AddressList.GetItemText(i,1);		
		f.Write(strTmp.Mid(0,12),13);
	}
	GetDlgItemText(IDC_COMBO_PLUSENUM,strTmp);
	f.Write(strTmp.Mid(0,1),2);
	f.Close();
}






//void CFkSetDlg::OnRadioPwlong() 
//{
	// TODO: Add your control notification handler code here
//		if (((CButton*)GetDlgItem(IDC_RADIO_PWLONG))->GetCheck())
//	{
//		g_bPwdLong = TRUE;
//	}
//	else
//	{
//		g_bPwdLong = FALSE;	
//	}

//}

//void CFkSetDlg::OnRadioPwsht() 
//{
	// TODO: Add your control notification handler code here
//	if (((CButton*)GetDlgItem(IDC_RADIO_PWSHT))->GetCheck())
//	{
//		g_bPwdLong = FALSE;
//	}
//	else
//	{
//		g_bPwdLong = TRUE;	
//	}
	
//}

void CFkSetDlg::OnButton22() 
{
	// TODO: Add your control notification handler code here
	if(g_RtuType == TERMINAL_TYPE_FK)
	{
		
	}
	else
	{
		SetCheckTmu(22,SendData69822,sizeof(SendData69822),g_bPwdLong);
	}
}

// 配置交采
void CFkSetDlg::OnButton23() 
{
	// TODO: Add your control notification handler code here
	//	SendData6982[24] = (SendData6982[24]&0xf0)|(m_nAcPort&0x0f);
	if(g_RtuType == TERMINAL_TYPE_FK)
	{
		
	}
	else
	{
		UpdateData(TRUE);
		SendData69823[24] = (SendData6982[24]&0xf0)|(m_nAcPort&0x0f);

		SetCheckTmu(23,SendData69823,sizeof(SendData69823),g_bPwdLong);
	}

}

//跳2轮闸
void CFkSetDlg::OnButton24() 
{
		SetCheckTmu(24,SendData69824,sizeof(SendData69824),g_bPwdLong);	
}


// 4轮合闸
void CFkSetDlg::OnButton26() 
{
	// TODO: Add your control notification handler code here
	if(g_RtuType == TERMINAL_TYPE_FK)
	{
	
	}
	else
	{
		SetCheckTmu(26,SendData69826,sizeof(SendData69826),g_bPwdLong);
	}
	
}

void CFkSetDlg::OnButton27() 
{
	// TODO: Add your control notification handler code here
	if(g_RtuType == TERMINAL_TYPE_FK)
	{
	
	}
	else
	{
		int n = GetDlgItemInt(IDC_COMBO_PLUSENUM);
		if(n==1||n==2)
		{
			SetCheckTmu(27,SendData69827_1,sizeof(SendData69827_1),g_bPwdLong);
		}
		else if(n == 3||n==4)
		{
			SetCheckTmu(27,SendData69827_2,sizeof(SendData69827_2),g_bPwdLong);
		}
	}
}


// 告警声音打开
void CFkSetDlg::OnButton28() 
{
	// TODO: Add your control notification handler code here
	if(g_RtuType == TERMINAL_TYPE_FK)
	{
	
	}
	else
	{
		SetCheckTmu(28,SendData69828,sizeof(SendData69828),g_bPwdLong);
	}	
}

// 485  透抄
void CFkSetDlg::OnButton29() 
{

	char straddr[6];
	SendData69829_4851[18] = m_sel4851.GetCurSel() + 1;
	memset(straddr,0,6);
	if (GetMeteraddr(1,straddr) < 0)
		return;

	memcpy(SendData69829_4851+29,straddr,6);
	SetCheckTmu(29,SendData69829_4851,sizeof(SendData69829_4851),g_bPwdLong);

}

void CFkSetDlg::OnButton30() 
{

	SendData69829_4851[18] = m_sel4852.GetCurSel() + 1;

	char straddr[6];
	memset(straddr,0,6);
	if (GetMeteraddr(2,straddr) < 0)
		return;

	memcpy(SendData69829_4851+29,straddr,6);

	SetCheckTmu(30,SendData69829_4851,sizeof(SendData69829_4851),g_bPwdLong);
	
}



int CFkSetDlg::MakeSetMacFrame(BYTE *pReport,CString &strAddr)
{

	DWORD area1;
	DWORD area2;
	DWORD addr1;
	DWORD addr2;
	int num;
	DWORD mac;
	int nReportLen = 18;
	pReport[nReportLen++] = 0x55;
	pReport[nReportLen++] = 0x55;

	if (strAddr.GetLength()!=(int)strlen(m_macdlg.m_para.szStratTermAddr))
	{
		TRACE("地址长度不一致[%s][%s]",(LPCTSTR)strAddr,m_macdlg.m_para.szStratTermAddr);
		return -1;
	}

	switch(strAddr.GetLength())
	{
	case 8:
		addr1 =	atoi((LPCTSTR)strAddr);
		addr2 =	atoi(m_macdlg.m_para.szStratTermAddr);
		num = addr1-addr2;
		break;
	case 9:
		sscanf((LPCTSTR)strAddr,"%4X%5d",&area1,&addr1);
		sscanf(m_macdlg.m_para.szStratTermAddr,"%04X%05d",&area2,&addr2);
		num = (area1 *0xFFFF+addr1)-(area2*0xFFFF+addr2) ;
		break;
	case 12:
		addr1 =	atoi((LPCTSTR)strAddr+4);
		addr2 =	atoi(m_macdlg.m_para.szStratTermAddr+4);
		num = addr1-addr2;
		break;	
	default:
		TRACE("地址长度不合法[%s][%s]",(LPCTSTR)strAddr,m_macdlg.m_para.szStratTermAddr);
		return -1;
	}
	if(num < 0 || num >(int)m_macdlg.m_para.dwTermNumber )
	{
		TRACE("地址不在范围[%s][%s]",(LPCTSTR)strAddr,m_macdlg.m_para.szStratTermAddr);
		return -1;
	}
	if (strlen(m_macdlg.m_para.szStratMAC)!=12)
	{
		return -2;
	}

	sscanf(m_macdlg.m_para.szStratMAC,"%2X%2X%2X%6X",&g_mac[0],&g_mac[1],&g_mac[2],&mac);

	mac+=num;
	g_mac[3]=LOBYTE(mac/0x10000);
	g_mac[4]=LOBYTE(mac/0x100);
	g_mac[5]=LOBYTE(mac);

	pReport[nReportLen++] = g_mac[0];
	pReport[nReportLen++] = g_mac[1];
	pReport[nReportLen++] = g_mac[2];
	pReport[nReportLen++] = g_mac[3];
	pReport[nReportLen++] = g_mac[4];
	pReport[nReportLen++] = g_mac[5];

	//	memcpy(pReport+nReportLen,param1,6);
	//	nReportLen += 6;
	int nCrc16 =CalcCRC16_r(pReport+18,8);
	
	memcpy(pReport+nReportLen,&nCrc16,2);
	nReportLen += 2;
	pReport[nReportLen++] = 0xAA;
	pReport[nReportLen++] = 0xAA;

	return nReportLen;
} 
int CFkSetDlg::MakeSetSERFrame(BYTE *pReport,CString &strAddr)
{

	DWORD area1;
	DWORD area2;
	DWORD addr1;
	DWORD addr2;
	int num;
	DWORD mac;
	int nReportLen = 18;
	pReport[nReportLen++] = 0x55;
	pReport[nReportLen++] = 0x55;

	if (strAddr.GetLength()!=(int)strlen(m_CDMAsERIA.m_szTermID))
	{
		TRACE("地址长度不一致[%s][%s]",(LPCTSTR)strAddr,m_CDMAsERIA.m_szTermID);
		return -1;
	}

	switch(strAddr.GetLength())
	{

	case 9:
		sscanf((LPCTSTR)strAddr,"%4X%5d",&area1,&addr1);
		sscanf(m_CDMAsERIA.m_szTermID,"%04X%05d",&area2,&addr2);
			num = (area1 *0xFFFF+addr1)-(area2*0xFFFF+addr2) ;
		break;
	
	
	default:
		//TRACE("地址长度不合法[%s][%s]",(LPCTSTR)strAddr,m_macdlg.m_para.szStratTermAddr);
		return -1;
	}
	if(num < 0 || num >(int)m_CDMAsERIA.dwTermNumber )
	{
		TRACE("地址不在范围[%s][%s]",(LPCTSTR)strAddr,m_CDMAsERIA.dwTermNumber);
		return -1;
	}
//char tmp [12];

 int tmp ;	
	
	 tmp = atoi(m_CDMAsERIA.m_sz8)+num;
 return tmp;

} 
int CFkSetDlg::MakeSetSERFrametwo(BYTE *pReport,CString &strAddr)
{

	DWORD area1;
	DWORD area2;
	DWORD addr1;
	DWORD addr2;
	int num;
	DWORD mac;
	int nReportLen = 18;
	pReport[nReportLen++] = 0x55;
	pReport[nReportLen++] = 0x55;

	if (strAddr.GetLength()!=(int)strlen(m_macdlg.m_para.szStratTermAddr))
	{
		TRACE("地址长度不一致[%s][%s]",(LPCTSTR)strAddr,m_macdlg.m_para.szStratTermAddr);
		return -1;
	}

	switch(strAddr.GetLength())
	{

	case 9:
		sscanf((LPCTSTR)strAddr,"%4X%5d",&area1,&addr1);
		sscanf(m_CDMAsERIA.m_szTermID,"%04X%05d",&area2,&addr2);
			num = (area1 *0xFFFF+addr1)-(area2*0xFFFF+addr2) ;

	default:
		//TRACE("地址长度不合法[%s][%s]",(LPCTSTR)strAddr,m_macdlg.m_para.szStratTermAddr);
		return -1;
	}
//char tmp [12];

 int tmp ;	return
	
	 tmp = atoi(m_CDMAsERIA.m_szSerisID)+num;
 return tmp;

} 
// 32

void CFkSetDlg::OnButtonSetMac() 
{
	// TODO: Add your control notification handler code here
	
	if(m_macdlg.DoModal() == IDOK)
	{		
		//SetCheckTmu(17,SendData17,sizeof(SendData17),(void*)&dlg.m_data);
		if(g_RtuType == TERMINAL_TYPE_FK)
		{
		//	SetCheckTmu(32,SendData17,sizeof(SendData17),FALSE,(void*)&dlg.m_data);
		}
		else
		{
		//	void* pp;
		//	SetCheckTmu(32,SendData69832_mac,sizeof(SendData69832_mac),g_bPwdLong,(void*)dlg.m_strMac);
			SetCheckTmu(32,SendData69832_mac,sizeof(SendData69832_mac),g_bPwdLong);
		}
	}	
}

// 33
void CFkSetDlg::OnButtonRdMac() 
{

	SetCheckTmu(33,SendData69833_mac,sizeof(SendData69833_mac),g_bPwdLong);
		
}

//34
void CFkSetDlg::OnButtonRdVer() 
{

		SetCheckTmu(34,SendData69834_ver,sizeof(SendData69834_ver),g_bPwdLong);
		
}

int CFkSetDlg::GetMeteraddr(int index,char *pAddr) 
{
	int lenId = 0;
	
	this->UpdateData(TRUE);
	CString strMtno;
	switch(index)
	{
	case 1:	
	default:
		lenId = m_editmt1.GetLength();
		strMtno.Format("%s",m_editmt1);
		break;
	case 2:
		lenId = m_editmt2.GetLength();
		strMtno.Format("%s",m_editmt2);
		break;

	}
	CString strtmp = "";
	if (lenId < 1)
	{
		AfxMessageBox("请输入表地址!");
		return -1;
	}
	if (lenId <= 12)
	{
		strtmp.Format("%012s",strMtno);
	}

	for(int i=0;i<12;i++)		// 数据项编号。占四字节，低位在前
    {
        if(strtmp.GetLength()<2)
            break;
        CString str=strtmp.Right(2);
        strtmp=strtmp.Left(strtmp.GetLength()-2);
        strtmp.TrimRight();
        BYTE nChar=(BYTE)strtol(str,NULL,16);
        pAddr[i]=nChar;		
    }
	return 0;
}






void CFkSetDlg::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
//	AfxMessageBox("");
	*pResult = 0;
}

void CFkSetDlg::OnKillfocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
//	AfxMessageBox("");
	*pResult = 0;
}

void CFkSetDlg::OnReturnList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	AfxMessageBox("");
	*pResult = 0;
}

void CFkSetDlg::OnButtonDoor() 
{

		SetCheckTmu(35,SendData69835_door,sizeof(SendData69835_door),g_bPwdLong);
		
}



void CFkSetDlg::OnButton33() 
{
	// TODO: Add your control notification handler code here
	CDlgCDMAPara dlg;
	if(dlg.DoModal() == IDOK)
	{		
		//SetCheckTmu(17,SendData17,sizeof(SendData17),(void*)&dlg.m_data);

		SetCheckTmu(TYPE_SET_CDMA,SendDataSetCDMA,sizeof(SendDataSetCDMA),FALSE,(void*)&dlg.m_data);
		
	}	
}

void CFkSetDlg::OnButton36() 
{
	// TODO: Add your control notification handler code here
		SetCheckTmu(TYPE_GET_CDMA,SendDataGetCDMA,sizeof(SendDataGetCDMA),FALSE);	
}

void CFkSetDlg::OnButtonClrAC() 
{
	// TODO: Add your control notification handler code here
		SetCheckTmu(TYPE_CLR_AC,SendDataClrAC,sizeof(SendDataClrAC),FALSE);		
}

void CFkSetDlg::OnButton37() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CFkSetDlg::OnButtonSetid() 
{
	// TODO: Add your control notification handler code here
	CSetTermID dlg;
	if(dlg.DoModal() == IDOK)
	{		
		//SetCheckTmu(17,SendData17,sizeof(SendData17),(void*)&dlg.m_data);
		
		SetCheckTmu(TYPE_SET_225,SendDataSetF225,sizeof(SendDataSetF225),FALSE,(void*)&dlg.m_szTermID);
		
	}	

}
void CFkSetDlg::OnButton38() 
{

	if(m_CDMAsERIA.DoModal() == IDOK)
	{		
		//SetCheckTmu(17,SendData17,sizeof(SendData17),(void*)&dlg.m_data);
		//设置资产号
		//compareCHAR
	//	SetCheckTmu(TYPE_SET_225,SendDataSetF225,sizeof(SendDataSetF225),FALSE,(void*)&m_CDMAsERIA.m_sz8);
		SetCheckTmu(38,SendDataSetF225,sizeof(SendDataSetF225),FALSE,(void*)&m_CDMAsERIA.m_sz8,(void*)&m_CDMAsERIA.dwTermNumber);
		//读取
		SetCheckTmu(39,SendData69821,sizeof(SendData69821),g_bPwdLong);
		//设置CDMA
//		SetCheckTmu(TYPE_SET_CDMA,SendDataSetCDMA,sizeof(SendDataSetCDMA),FALSE,(void*)&m_CDMAsERIA.m_szTermCDMA);
	SetCheckTmu(40,SendDataSetCDMA,sizeof(SendDataSetCDMA),FALSE,(void*)&m_CDMAsERIA.m_szTermCDMA);
		//读取CDMA
		SetCheckTmu(41,SendDataGetCDMA,sizeof(SendDataGetCDMA),FALSE);	
		
	}	

}

void CFkSetDlg::OnButtonUpmodem() 
{
	// TODO: Add your control notification handler code here
	
}

void CFkSetDlg::OnEsam() 
{
	// TODO: Add your control notification handler code here
	
}
