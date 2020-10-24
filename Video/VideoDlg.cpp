
// VideoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Video.h"
#include "VideoDlg.h"
#include "afxdialogex.h"

#include "..\GAVideoSDK\HCNetSDK\Include\HCNetSDK.h"
#pragma comment( lib, "..\\GAVideoSDK\\HCNetSDK\\Lib\\HCNetSDK.lib" )

#include "..\NHWin32Lib\NHWin32Lib.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\Debug\\NHWin32Lib_D.lib")
#else
#pragma comment(lib, "..\\Release\\NHWin32Lib.lib")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVideoDlg 对话框




CVideoDlg::CVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoDlg::IDD, pParent)
	, m_bInitDVR(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// 初始化硬盘录像机
	if (NET_DVR_Init())
	{
		m_bInitDVR = true;
	}

	m_lsLonIn.clear();
}

CVideoDlg::~CVideoDlg()
{
	// 卸载硬盘录像机
	if (m_bInitDVR)
	{
		NET_DVR_Cleanup();
	}
}

void CVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVideoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CVideoDlg 消息处理程序

BOOL CVideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	SDLonIn sDLonIn;

	sDLonIn.strIP = L"172.18.38.51";
	sDLonIn.nPort = 8000;
	sDLonIn.nChannel = 33;
	sDLonIn.strUserName = L"admin";
	sDLonIn.strPassword = L"12345";

	m_lsLonIn.push_back(sDLonIn);

	if (m_lsLonIn.size() > 0)
	{
		CString strIP;
		int nWnd(IDC_STATIC_VIDEO1);
		std::list<SDLonIn>::iterator iter = m_lsLonIn.begin();
		for ( ; iter != m_lsLonIn.end(); iter++)
		{
			LONG LlonIn;
			if (strIP != iter->strIP)
			{
				// 登录
				LlonIn = LonIn(*iter);
			}
			// 显示
			if (LlonIn == 0)
			{
				StartPlay(nWnd, iter->nChannel, LlonIn);
			}
			// 记录IP不重复登录
			// strIP = iter->strIP;
			// 控件框+1
			nWnd +=1;
		}
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVideoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVideoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LONG CVideoDlg::LonIn(SDLonIn sDLonIn)
{
	// 获取视频信息
	char *pch(NULL);
	char chIP[MAX_PATH],chUserName[MAX_PATH],chPassword[MAX_PATH];

	pch = CNHCommonAPI::UnicodeToANSI(sDLonIn.strIP);
	strncpy_s(chIP, sizeof(chIP), pch, _TRUNCATE);
	if (NULL != pch)
	{
		free(pch);
		pch = NULL;
	}

	pch = CNHCommonAPI::UnicodeToANSI(sDLonIn.strUserName);
	strncpy_s(chUserName, sizeof(chUserName), pch, _TRUNCATE);
	if (NULL != pch)
	{
		free(pch);
		pch = NULL;
	}

	pch = CNHCommonAPI::UnicodeToANSI(sDLonIn.strPassword);
	strncpy_s(chPassword, sizeof(chPassword), pch, _TRUNCATE);
	if (NULL != pch)
	{
		free(pch);
		pch = NULL;
	}

	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
	memset(&DeviceInfoTmp,0,sizeof(NET_DVR_DEVICEINFO_V30));
	LONG lUserID = NET_DVR_Login_V30(chIP, sDLonIn.nPort, chUserName, chPassword, &DeviceInfoTmp);
	if(lUserID == -1)
	{
		MessageBox(L"Login to Device failed!\n");
		//return FALSE;
	}
	return lUserID;
}

void CVideoDlg::StartPlay(const int& nWnd, const int& nChannel, const LONG& lLonIn)
{
	NET_DVR_CLIENTINFO ClientInfo;
	ClientInfo.hPlayWnd     = GetDlgItem(nWnd)->m_hWnd;
	ClientInfo.lChannel     = nChannel;
	ClientInfo.lLinkMode    = 0;
    ClientInfo.sMultiCastIP = NULL;

	LONG lPlayHandle = NET_DVR_RealPlay_V30(lLonIn,&ClientInfo,NULL,NULL,TRUE);
	if(-1 == lPlayHandle)
	{
		DWORD err=NET_DVR_GetLastError();
		CString m_csErr;
		m_csErr.Format(L"播放出错，错误代码%d",err);
		MessageBox(m_csErr);
	}
}
