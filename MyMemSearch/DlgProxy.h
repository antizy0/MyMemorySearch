
// DlgProxy.h: ͷ�ļ�
//

#pragma once

class CMyMemSearchDlg;


// CMyMemSearchDlgAutoProxy ����Ŀ��

class CMyMemSearchDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CMyMemSearchDlgAutoProxy)

	CMyMemSearchDlgAutoProxy();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��

// ����
public:
	CMyMemSearchDlg* m_pDialog;

// ����
public:

// ��д
	public:
	virtual void OnFinalRelease();

// ʵ��
protected:
	virtual ~CMyMemSearchDlgAutoProxy();

	// ���ɵ���Ϣӳ�亯��

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CMyMemSearchDlgAutoProxy)

	// ���ɵ� OLE ����ӳ�亯��

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

