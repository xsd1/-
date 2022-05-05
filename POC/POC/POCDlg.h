
// POCDlg.h: 头文件
//

#pragma once
#include "LA.h"
#include "PA.h"
using namespace std;

// CPOCDlg 对话框
class CPOCDlg : public CDialogEx
{
// 构造
public:
	CPOCDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	LA* la;
	PA* pa;


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLoad();
	CString sourceCode;
	CEdit m_scode;
	afx_msg void OnLA();
	CEdit m_process;
	CEdit m_res;
	afx_msg void OnFinish();
	afx_msg void OnPA();
	afx_msg void OnClear();
};
