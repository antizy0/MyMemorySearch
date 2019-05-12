
// MyMemSearchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyMemSearch.h"
#include "MyMemSearchDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include <windows.h>
#include<TlHelp32.h>
#include <process.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue);  //ƥ���Ӧ��ֵ
BOOL FindFirst(DWORD dwValue);	//Ŀ����̿ռ���е�һ�β���
BOOL FindNext(DWORD dwValue);	//��Ŀ����̵�ַ�ռ��2��3��4 ...�β���

DWORD GetProcessID(char* FileName);

DWORD g_addrList[10240];			//��ַ�б�
int g_nListCnt;					//��Ч��ַ�ĸ���
HANDLE g_hProcess;				//Ŀ����̾��



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyMemSearchDlg �Ի���


IMPLEMENT_DYNAMIC(CMyMemSearchDlg, CDialogEx);

CMyMemSearchDlg::CMyMemSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MYMEMSEARCH_DIALOG, pParent)
	, m_ProcessName(_T(""))
	, SearchValue(0)
	, m_addrvalue(_T(""))
	, m_writeval(0)
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CMyMemSearchDlg::~CMyMemSearchDlg()
{
	// ����öԻ������Զ���������
	//  ���˴���ָ��öԻ���ĺ���ָ������Ϊ NULL���Ա�
	//  �˴���֪���öԻ����ѱ�ɾ����
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CMyMemSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PROCESS_LIST, m_ProcsList);
	DDX_CBString(pDX, IDC_COMBO_PROCESS_LIST, m_ProcessName);
	DDX_Control(pDX, IDC_LIST4, m_CtrAddrList);
	DDX_Text(pDX, IDC_EDIT_Value, SearchValue);
	DDX_Text(pDX, IDC_EDIT2, m_addrvalue);
	DDX_Control(pDX, IDC_EDIT2, m_ctraddrval);
	DDX_Text(pDX, IDC_EDIT3, m_writeval);
}

BEGIN_MESSAGE_MAP(CMyMemSearchDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_FLASH, &CMyMemSearchDlg::OnBnClickedButtonFlash)
	ON_BN_CLICKED(IDC_BUTTON_FINDFIRST, &CMyMemSearchDlg::OnBnClickedButtonFindfirst)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CMyMemSearchDlg::OnHdnItemclickList4)
	ON_NOTIFY(NM_CLICK, IDC_LIST4, &CMyMemSearchDlg::OnNMClickList4)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST4, &CMyMemSearchDlg::OnLvnItemActivateList4)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyMemSearchDlg::OnBnClickedFindNext)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyMemSearchDlg::OnBnClickedChange)
END_MESSAGE_MAP()


// CMyMemSearchDlg ��Ϣ�������

BOOL CMyMemSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	
	PROCESSENTRY32 pe32 = { 0 };//���̿�����Ϣ

	pe32.dwSize = sizeof(pe32);//���û������С

	HANDLE hprocess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//��������

	BOOL  bmore = Process32First(hprocess, &pe32);//Ѱ��,ͨ��ָ��ı��ⲿ����
	while (bmore)
	{
		m_ProcsList.AddString(pe32.szExeFile);

		bmore = Process32Next(hprocess, &pe32);
	}


	DWORD dwStyle = m_CtrAddrList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;// ѡ��ĳ��ʹ���и�����ֻ������report ����listctrl ��   
	dwStyle |= LVS_EX_GRIDLINES;// �����ߣ�ֻ������report ����listctrl ��   
								//dwStyle |= LVS_EX_CHECKBOXES;//item ǰ����checkbox �ؼ�   
	m_CtrAddrList.SetExtendedStyle(dwStyle); // ������չ���   
	dwStyle |= LVS_EX_FULLROWSELECT;// ѡ��ĳ��ʹ���и�����ֻ������report ����listctrl ��  

	m_CtrAddrList.ModifyStyle(LVS_ICON | LVS_SMALLICON | LVS_LIST, LVS_REPORT);  //�����б�ؼ��ı�����ʾ��ʽ  							
	m_CtrAddrList.SetExtendedStyle(LVS_EX_GRIDLINES);  //�����б�ؼ���ʾ������  
									
	//m_CtrAddrList.InsertColumn(0, _T("���"), LVCFMT_LEFT, 50);//���б�ؼ��в�����  
	m_CtrAddrList.InsertColumn(0, _T("˫����ѡ��"), LVCFMT_LEFT, 200);





	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMyMemSearchDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyMemSearchDlg::OnPaint()
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
HCURSOR CMyMemSearchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ���û��ر� UI ʱ������������Ա���������ĳ��
//  �������Զ�����������Ӧ�˳���  ��Щ
//  ��Ϣ�������ȷ����������: �����������ʹ�ã�
//  ������ UI�������ڹرնԻ���ʱ��
//  �Ի�����Ȼ�ᱣ�������

void CMyMemSearchDlg::OnClose()
{
	if (CanExit())
		CDialogEx::OnClose();
}

void CMyMemSearchDlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void CMyMemSearchDlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

BOOL CMyMemSearchDlg::CanExit()
{
	// �����������Ա�����������Զ���
	//  �������Իᱣ�ִ�Ӧ�ó���
	//  ʹ�Ի���������������� UI ����������

	::CloseHandle(g_hProcess);
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}



void CMyMemSearchDlg::OnBnClickedButtonFlash()//ˢ�½����б�
{
	int i;
	for (i = m_ProcsList.GetCount() - 1; i >= 0;i--)
	{
		m_ProcsList.DeleteString(i);
	}

	PROCESSENTRY32 pe32 = { 0 };//���̿�����Ϣ

	pe32.dwSize = sizeof(pe32);//���û������С

	HANDLE hprocess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//��������

	BOOL  bmore = Process32First(hprocess, &pe32);//Ѱ��,ͨ��ָ��ı��ⲿ����
	while (bmore)
	{
		m_ProcsList.AddString(pe32.szExeFile);

		bmore = Process32Next(hprocess, &pe32);
	}

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}



BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue)
{
	BYTE arBytes[4096];
	if (!::ReadProcessMemory(g_hProcess, (LPVOID)dwBaseAddr, arBytes, 4096, NULL))
	{
		return false;
	}
	DWORD * pdw;
	for (int i = 0; i < (int)4 * 1024 - 3; i++)
	{
		pdw = (DWORD*)&arBytes[i];
		if (pdw[0] == dwValue)//����Ҫ���ҵ�ֵ
		{
			if (g_nListCnt >= 10240)
			{
				return FALSE;
			}
			else
			{
				g_addrList[g_nListCnt++] = dwBaseAddr + i;
			}
		}
	}



	return true;
}
BOOL FindFirst(DWORD dwVal)
{
	const DWORD dwOneGB = 1024 * 1024 * 1024;
	const DWORD dwOnePage = 4 * 1024;
	if (g_hProcess == NULL)
	{
		//return FALSE;
	}
	//�鿴����ϵͳ����,�Ծ�����ʼ��ַ
	DWORD dwBase;
	OSVERSIONINFO vi = { sizeof(vi)
	};
	::GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		dwBase = 4 * 1024 * 1024;//windows98ϵ��,4mb
	}
	else
	{
		dwBase = 640 * 1024;//windowsNTϵ��,64kb
	}
	//�ڿ�ʼ��ַ��2GB�ĵ�ַ�ռ���в���
	for (; dwBase < 2 * dwOneGB; dwBase += dwOnePage)
	{
		CompareAPage(dwBase, dwVal);
	}
	return TRUE;
}

BOOL FindNext(DWORD dwValue)
{
	//����m_addrList��������Ч��ַ�ĸ���,��ʼ���µ�m_nListCntֵ
	int nOrgCnt = g_nListCnt;
	g_nListCnt = 0;

	//��m_addrList�����¼�ĵ�ַ������
	BOOL bRet = FALSE;//����ʧ��
	DWORD dwReadValue;
	for (int i = 0; i < nOrgCnt; i++)
	{
		if (::ReadProcessMemory(g_hProcess, (LPVOID)g_addrList[i], &dwReadValue, sizeof(DWORD), NULL))
		{
			//AfxMessageBox("lll");
			if (dwValue == dwReadValue)
			{
				//AfxMessageBox("www");
				g_addrList[g_nListCnt++] = g_addrList[i];
				bRet = TRUE;
			}
		}
	}
	return bRet;
}
BOOL WriteMemory(DWORD dwAddr, DWORD dwValue)
{
	return ::WriteProcessMemory(g_hProcess, (LPVOID)dwAddr, &dwValue, sizeof(DWORD), NULL);
}

void CMyMemSearchDlg::OnBnClickedButtonFindfirst()
{

	m_CtrAddrList.SetExtendedStyle(m_CtrAddrList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	DWORD proID;
	UpdateData(TRUE); 
	char *exename = m_ProcessName.GetBuffer();
	proID = GetProcessID(exename);
	g_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proID);

	FindFirst(SearchValue);
	

	m_CtrAddrList.DeleteAllItems();
	CString addr;


	for (int i = 0; i < g_nListCnt; i++)
	{
		addr.Format("0x%p", g_addrList[i]);

		m_CtrAddrList.InsertItem(i, addr);
	}

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


DWORD GetProcessID(char *FileName)
{
	HANDLE myhProcess;
	PROCESSENTRY32 mype;
	mype.dwSize = sizeof(PROCESSENTRY32);
	BOOL mybRet;
	//���н��̿���
	myhProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //TH32CS_SNAPPROCESS�������н���
																  //��ʼ���̲���
	mybRet = Process32First(myhProcess, &mype);
	//ѭ���Ƚϣ��ó�ProcessID
	while (mybRet)
	{
		if (strcmp(FileName, mype.szExeFile) == 0)
			return mype.th32ProcessID;
		else
			mybRet = Process32Next(myhProcess, &mype);
	}
	return 0;
}

void CMyMemSearchDlg::OnHdnItemclickList4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//AfxMessageBox("www");

	*pResult = 0;
}


void CMyMemSearchDlg::OnNMClickList4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//AfxMessageBox("www");
	*pResult = 0;
}


void CMyMemSearchDlg::OnLvnItemActivateList4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	CString str;
	int nId;
	//���ȵõ������λ��
	POSITION pos = m_CtrAddrList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("������ѡ��һ��", "��ʾ", MB_ICONEXCLAMATION);
		return;
	}
	//�õ��кţ�ͨ��POSITIONת��
	nId = (int)m_CtrAddrList.GetNextSelectedItem(pos);
	str = m_CtrAddrList.GetItemText(nId, 0);
	m_ctraddrval.SetWindowText(str);

	*pResult = 0;
}


void CMyMemSearchDlg::OnBnClickedFindNext()
{
	UpdateData(TRUE);

	FindNext(SearchValue);
	m_CtrAddrList.DeleteAllItems();
	CString addr;


	for (int i = 0; i < g_nListCnt; i++)
	{
		addr.Format("0x%p", g_addrList[i]);

		m_CtrAddrList.InsertItem(i, addr);
	}

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CMyMemSearchDlg::OnBnClickedChange()
{
	UpdateData(TRUE);
	if (WriteMemory(g_addrList[0], m_writeval))
	{
		AfxMessageBox("�޸��ڴ�ֵ�ɹ���");
	}


	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
