
// VideoDlg.h : 头文件
//

#pragma once
#include <list>
#include <string.h>

// CVideoDlg 对话框
class CVideoDlg : public CDialogEx
{
// 构造
public:
	CVideoDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CVideoDlg();
// 对话框数据
	enum { IDD = IDD_VIDEO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	// 初始化硬盘录像机
	bool m_bInitDVR;
	// 登录参数列表
	struct SDLonIn
	{
		CString strIP;
		CString strUserName;
		CString strPassword;
		int nPort;
		int nChannel;
	};
	std::list<SDLonIn> m_lsLonIn;

	void StartPlay(const int& nWnd, const int& nChannel, const LONG& lLonIn);
	LONG LonIn(SDLonIn sDLonIn);
};
