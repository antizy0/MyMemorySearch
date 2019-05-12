
// DlgProxy.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyMemSearch.h"
#include "DlgProxy.h"
#include "MyMemSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyMemSearchDlgAutoProxy

IMPLEMENT_DYNCREATE(CMyMemSearchDlgAutoProxy, CCmdTarget)

CMyMemSearchDlgAutoProxy::CMyMemSearchDlgAutoProxy()
{
	EnableAutomation();
	
	// ΪʹӦ�ó������Զ��������ڻ״̬ʱһֱ���� 
	//	���У����캯������ AfxOleLockApp��
	AfxOleLockApp();

	// ͨ��Ӧ�ó����������ָ��
	//  �����ʶԻ���  ���ô�����ڲ�ָ��
	//  ָ��Ի��򣬲����öԻ���ĺ���ָ��ָ��
	//  �ô���
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CMyMemSearchDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CMyMemSearchDlg)))
		{
			m_pDialog = reinterpret_cast<CMyMemSearchDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CMyMemSearchDlgAutoProxy::~CMyMemSearchDlgAutoProxy()
{
	// Ϊ������ OLE �Զ����������ж������ֹӦ�ó���
	//	������������ AfxOleUnlockApp��
	//  ���������������⣬�⻹���������Ի���
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CMyMemSearchDlgAutoProxy::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease��  ���ཫ�Զ�
	// ɾ���ö���  �ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CMyMemSearchDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CMyMemSearchDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// ע��: ��������˶� IID_IMyMemSearch ��֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡�  �� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {C3DC7F87-C104-4D90-9B0F-4E5C18476527}
static const IID IID_IMyMemSearch =
{ 0xC3DC7F87, 0xC104, 0x4D90, { 0x9B, 0xF, 0x4E, 0x5C, 0x18, 0x47, 0x65, 0x27 } };

BEGIN_INTERFACE_MAP(CMyMemSearchDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CMyMemSearchDlgAutoProxy, IID_IMyMemSearch, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 ���ڴ���Ŀ�� StdAfx.h �ж���
// {0449ED02-8B9D-4AF9-A3C5-689786FC7AA3}
IMPLEMENT_OLECREATE2(CMyMemSearchDlgAutoProxy, "MyMemSearch.Application", 0x449ed02, 0x8b9d, 0x4af9, 0xa3, 0xc5, 0x68, 0x97, 0x86, 0xfc, 0x7a, 0xa3)


// CMyMemSearchDlgAutoProxy ��Ϣ�������
