
// MyMemSearch.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMyMemSearchApp: 
// �йش����ʵ�֣������ MyMemSearch.cpp
//

class CMyMemSearchApp : public CWinApp
{
public:
	CMyMemSearchApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMyMemSearchApp theApp;