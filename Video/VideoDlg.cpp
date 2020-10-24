
// VideoDlg.cpp : ʵ���ļ�
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


// CVideoDlg �Ի���




CVideoDlg::CVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoDlg::IDD, pParent)
	, m_bInitDVR(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// ��ʼ��Ӳ��¼���
	if (NET_DVR_Init())
	{
		m_bInitDVR = true;
	}

	m_lsLonIn.clear();
}

CVideoDlg::~CVideoDlg()
{
	// ж��Ӳ��¼���
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


// CVideoDlg ��Ϣ�������

BOOL CVideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

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
				// ��¼
				LlonIn = LonIn(*iter);
			}
			// ��ʾ
			if (LlonIn == 0)
			{
				StartPlay(nWnd, iter->nChannel, LlonIn);
			}
			// ��¼IP���ظ���¼
			// strIP = iter->strIP;
			// �ؼ���+1
			nWnd +=1;
		}
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVideoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVideoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LONG CVideoDlg::LonIn(SDLonIn sDLonIn)
{
	// ��ȡ��Ƶ��Ϣ
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
		m_csErr.Format(L"���ų����������%d",err);
		MessageBox(m_csErr);
	}
}
