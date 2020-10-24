
// VideoDlg.h : ͷ�ļ�
//

#pragma once
#include <list>
#include <string.h>

// CVideoDlg �Ի���
class CVideoDlg : public CDialogEx
{
// ����
public:
	CVideoDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CVideoDlg();
// �Ի�������
	enum { IDD = IDD_VIDEO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	// ��ʼ��Ӳ��¼���
	bool m_bInitDVR;
	// ��¼�����б�
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
