
// EPVideoAndJudge.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEPVideoAndJudgeApp:
// �йش����ʵ�֣������ EPVideoAndJudge.cpp
//

class CEPVideoAndJudgeApp : public CWinApp
{
public:
	CEPVideoAndJudgeApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CEPVideoAndJudgeApp theApp;