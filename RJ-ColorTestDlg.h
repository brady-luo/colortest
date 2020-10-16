
// RJ-ColorTestDlg.h: 头文件
//

#pragma once


// CA-SDK
#import "C:\Program Files (x86)\KONICAMINOLTA\CA-SDK\SDK\CA200Srvr.dll" no_namespace no_implementation 


// CRJColorTestDlg 对话框
class CRJColorTestDlg : public CDialog
{
// 构造
public:
	CRJColorTestDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RJCOLORTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CBrush backBrush;
	//RECT Rect1;

	//CA-SDK 智能指针类型
	IOutputProbesPtr m_pOutputProbesObj;
	IProbesPtr m_pProbesObj;
	ICasPtr m_pCasObj;
	IMemoryPtr m_pMemoryObj;
	IProbePtr m_pProbeObj;
	ICaPtr m_pCaObj;
	ICa200Ptr m_pCa200Obj;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedBtnFindcenter();
	afx_msg void OnBnClickedBtnGammatest();
	afx_msg void OnBnClickedBtnColortest();
	afx_msg void OnBnClickedBtnContrasttest();
	afx_msg void OnBnClickedBtnAlltest();
	afx_msg void OnBnClickedBtnConnectca();
	CListCtrl m_isOpencaOK;
	CListCtrl m_isTestOver;
	afx_msg void OnNMCustomdrawListOpenca(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawListTestresult(NMHDR* pNMHDR, LRESULT* pResult);
	CString m_fx;
	CString m_fy;
	CString m_Lv;
	CString m_CCT;
	void CA_Measure_SxSyLv();    //CA测试函数
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnUsertest();
	afx_msg void OnBnClickedCheckIssavedata();
	CStatic m_picture;

	//void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	static DWORD __stdcall ThreadFunc_Gamma(LPVOID lpParam);
	static DWORD __stdcall ThreadFunc_Color(LPVOID lpParam);
	static DWORD __stdcall ThreadFunc_Contrast(LPVOID lpParam);
	static DWORD __stdcall ThreadFunc_AllTest(LPVOID lpParam);
};


