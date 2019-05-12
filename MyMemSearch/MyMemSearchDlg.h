
// MyMemSearchDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

class CMyMemSearchDlgAutoProxy;


// CMyMemSearchDlg �Ի���
class CMyMemSearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyMemSearchDlg);
	friend class CMyMemSearchDlgAutoProxy;

// ����
public:
	CMyMemSearchDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CMyMemSearchDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYMEMSEARCH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	CMyMemSearchDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	// �г����н���
	CComboBox m_ProcsList;
	afx_msg void OnBnClickedButtonFlash();
	// ��������
	CString m_ProcessName;
	// Ŀ���ַ�б�
	CListCtrl m_CtrAddrList;
	afx_msg void OnBnClickedButtonFindfirst();
	int SearchValue;
	afx_msg void OnHdnItemclickList4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemActivateList4(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_addrvalue;
	CEdit m_ctraddrval;
	afx_msg void OnBnClickedFindNext();
	afx_msg void OnBnClickedChange();
	int m_writeval;
};
