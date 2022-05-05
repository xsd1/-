
// POCDlg.cpp: 实现文件
//
#pragma once
#include "pch.h"
#include "framework.h"
#include "POC.h"
#include "POCDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPOCDlg 对话框



CPOCDlg::CPOCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_POC_DIALOG, pParent)
	, sourceCode(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPOCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_scode);
	DDX_Control(pDX, IDC_EDIT2, m_process);
	DDX_Control(pDX, IDC_EDIT3, m_res);
}

BEGIN_MESSAGE_MAP(CPOCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPOCDlg::OnLoad)
	ON_BN_CLICKED(IDC_BUTTON2, &CPOCDlg::OnLA)
	ON_BN_CLICKED(IDC_BUTTON4, &CPOCDlg::OnFinish)
	ON_BN_CLICKED(IDC_BUTTON3, &CPOCDlg::OnPA)
	ON_BN_CLICKED(IDC_BUTTON5, &CPOCDlg::OnClear)
END_MESSAGE_MAP()


// CPOCDlg 消息处理程序

BOOL CPOCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	AfxGetMainWnd()->SetWindowText("简易c++编译器");
	GetDlgItem(IDC_BUTTON2)->EnableWindow(0);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(0);
	la = new LA();
	la->main();// 初始化
	pa = new PA();
	pa->LR1();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPOCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPOCDlg::OnPaint()
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
HCURSOR CPOCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPOCDlg::OnLoad()
{
	//createNFA();
	// TODO: 在此添加控件通知处理程序代码
	m_scode.SetWindowTextA("");
	FILE* fp;
	fopen_s(&fp, "my_res\\词法分析_源程序.txt", "r");
	if (!fp) {
		MessageBox("file open error");
		return;
	}
	char str[100];
	memset(str, 0, sizeof(str));
	char ch;
	int len = 0;
	while (TRUE) {
		ch = fgetc(fp);
		if (ch == '\n'|| ch == EOF) {
			str[len] = '\0';
			CString s;
			s.Format("%s\r\n", str);
			int nLen = m_scode.GetWindowTextLengthA();
			m_scode.SetSel(nLen, nLen);
			m_scode.ReplaceSel(s);
			len = 0;
			if (ch == EOF)break;
		}
		else {
			str[len++] = ch;
		}
	}
	fclose(fp);
	//MessageBox("load success!");
	GetDlgItem(IDC_BUTTON1)->EnableWindow(0);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(1);
	m_scode.SetReadOnly(1);
}

void CPOCDlg::OnLA()
{
	// TODO: 在此添加控件通知处理程序代码
	char tokens[200][200];// token
	char dfa[200][200];
	memset(tokens, 0, sizeof(tokens));
	memset(dfa, 0, sizeof(dfa));
	int flag = 0;
	la->printDFA(dfa, 200, flag);
	if (flag == 1){
		MessageBox("too long!");
		return;
	}
	m_process.SetWindowTextA("");
	for (int i = 0; i < flag; i++) {
		CString s;
		s.Format("%s\r\n", dfa[i]);
		int nLen = m_process.GetWindowTextLengthA();
		m_process.SetSel(nLen, nLen);
		m_process.ReplaceSel(s);
	}
	Sleep(500);
	la->scanSourceFile();
	flag = 0;
	la->showTokens(tokens, 200, flag);
	if (flag==-1) {
		MessageBox("too long!");
		return;
	}
	m_res.SetWindowTextA("");
	for (int i = 0; i < flag; i++) {
		CString s;
		s.Format("%s\r\n", tokens[i]);
		int nLen = m_res.GetWindowTextLengthA();
		m_res.SetSel(nLen, nLen);
		m_res.ReplaceSel(s);
		Sleep(50);
	}
	MessageBox("词法分析成功!");
	GetDlgItem(IDC_BUTTON1)->EnableWindow(0);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(0);
	
	m_process.SetReadOnly(1);
	m_res.SetReadOnly(1);
}

void CPOCDlg::OnFinish()
{
	// TODO: 在此添加控件通知处理程序代码
	this->DestroyWindow();
}

void CPOCDlg::OnPA()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_STATIC1)->SetWindowTextA("token序列");
	char nToken[200];
	char nProcess[500][500];
	memset(nToken, 0, sizeof(nToken));
	memset(nProcess, 0, sizeof(nProcess));
	int flag = 0;
	char error;
	int Size = 0;
	pa->process(nToken, nProcess, flag, error, Size);
	CString nTokens;
	nTokens.Format("%s", nToken);
	m_scode.ReplaceSel(nTokens);
	Sleep(500);
	//::SendMessageW(m_scode, EM_SETREADONLY, 1, 0);// 设置不可更改
	if (flag == -1) {
		MessageBox("too long!");
		return;
	}
	m_process.SetLimitText(UINT_MAX);
	CString out;
	if (flag == -2) {
		out.Format("%s", "No");
		CString ss;
		if (error == '2') {
			ss = "标识符所在位置出错";
		}
		else if (error == '3') {
			ss = "常量所在位置出错";
		}
		else {
			ss = "关键词所在位置出错";
		}
		MessageBox(ss);
	}
	else {
		out.Format("%s", "Yes");
		MessageBox("语法分析成功！");
	}
	for (int i = 0; i <= Size; i++) {
		CString nPro;
		nPro.Format("%s\r\n", nProcess[i]);
		int nLen = m_process.GetWindowTextLengthA();
		m_process.SetSel(nLen, nLen);
		m_process.ReplaceSel(nPro);
	}
	
	m_res.ReplaceSel(out);
	m_scode.SetReadOnly(1);
	m_process.SetReadOnly(1);
	m_res.SetReadOnly(1);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(0);
}

void CPOCDlg::OnClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_scode.SetReadOnly(0);
	m_process.SetReadOnly(0);
	m_res.SetReadOnly(0);
	m_scode.SetWindowTextA("");
	m_process.SetWindowTextA("");
	m_res.SetWindowTextA("");
	GetDlgItem(IDC_BUTTON3)->EnableWindow(1);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(0);
}
