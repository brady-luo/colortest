
// RJ-ColorTestDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "RJ-ColorTest.h"
#include "RJ-ColorTestDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <fstream>
#include <string>

#include<time.h>  


// CA-SDK
#include "Const.h"       
#include "CaEvent.h"     
#import "C:\Program Files (x86)\KONICAMINOLTA\CA-SDK\SDK\CA200Srvr.dll" no_namespace implementation_only   //编译后会自动生成CA200Srvr.tlh

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

//全局变量
string str_Gray, str_fx, str_fy, str_Lv, str_CCT;
ofstream outUserTestFile;
int steptime;
HANDLE g_hWaitContrast, g_hWaitColor, g_hWaitGamma;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRJColorTestDlg 对话框

CRJColorTestDlg::CRJColorTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_RJCOLORTEST_DIALOG, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
}

void CRJColorTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OpenCA, m_isOpencaOK);
	DDX_Control(pDX, IDC_LIST_TestResult, m_isTestOver);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
	DDX_Control(pDX, IDC_COMBO_steptime, vCombo_steptime);
}

BEGIN_MESSAGE_MAP(CRJColorTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_FindCenter, &CRJColorTestDlg::OnBnClickedBtnFindcenter)
	ON_BN_CLICKED(IDC_BTN_GammaTest, &CRJColorTestDlg::OnBnClickedBtnGammatest)
	ON_BN_CLICKED(IDC_BTN_ColorTest, &CRJColorTestDlg::OnBnClickedBtnColortest)
	ON_BN_CLICKED(IDC_BTN_ContrastTest, &CRJColorTestDlg::OnBnClickedBtnContrasttest)
	ON_BN_CLICKED(IDC_BTN_AllTest, &CRJColorTestDlg::OnBnClickedBtnAlltest)
	ON_BN_CLICKED(IDC_BTN_ConnectCA, &CRJColorTestDlg::OnBnClickedBtnConnectca)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OpenCA, &CRJColorTestDlg::OnNMCustomdrawListOpenca)
	ON_BN_CLICKED(IDOK, &CRJColorTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRJColorTestDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_UserTest, &CRJColorTestDlg::OnBnClickedBtnUsertest)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_TestResult, &CRJColorTestDlg::OnNMCustomdrawListTestresult)
	ON_BN_CLICKED(IDC_CHECK_IsSaveData, &CRJColorTestDlg::OnBnClickedCheckIssavedata)
	ON_CBN_SELCHANGE(IDC_COMBO_steptime, &CRJColorTestDlg::OnCbnSelchangeCombosteptime)
END_MESSAGE_MAP()


// CRJColorTestDlg 消息处理程序

BOOL CRJColorTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	//初始化设置listControl
	CFont m_Font;      // 字体变量 m_Font 的声明。只能声明成局部变量，否则assert failed
	m_Font.CreatePointFont(150, _T("新宋体"));  // 设置字体的大小和格式
	m_isOpencaOK.SetFont(&m_Font, true);
	m_isOpencaOK.InsertColumn(0, _T("第一列"), LVCFMT_CENTER, 125);
	m_isOpencaOK.InsertItem(0, _T("请连接CA310"), LVCFMT_CENTER);
	m_isOpencaOK.SetItemData(0, 3);	

	m_isTestOver.SetFont(&m_Font, true);
	m_isTestOver.InsertColumn(0, _T("第一列"), LVCFMT_CENTER, 140);
	m_isTestOver.InsertItem(0, _T("  未开始测试  "), LVCFMT_CENTER);
	m_isTestOver.SetItemData(0, 3);

	//m_pictureName.SetWindowText(_T("请填写画面名称"));
	SetDlgItemText(IDC_EDIT_PictureName, _T("请填写画面名称"));

	//ChannelNO设置初始值，默认为0 (Konica Minolta calibration)
	SetDlgItemText(IDC_ChannelNO, _T("0"));

	//初始化控件不可用
	HWND hBtn = ::GetDlgItem(m_hWnd, IDC_BTN_GammaTest);
	::EnableWindow(hBtn, FALSE);
	hBtn = ::GetDlgItem(m_hWnd, IDC_BTN_ColorTest);
	::EnableWindow(hBtn, FALSE);
	hBtn = ::GetDlgItem(m_hWnd, IDC_BTN_ContrastTest);
	::EnableWindow(hBtn, FALSE);
	hBtn = ::GetDlgItem(m_hWnd, IDC_BTN_AllTest);
	::EnableWindow(hBtn, FALSE);
	hBtn = ::GetDlgItem(m_hWnd, IDC_BTN_UserTest);
	::EnableWindow(hBtn, FALSE);

	//初始化时间步长
	vCombo_steptime.SetCurSel(3);
	CString curStr_steptime;
	vCombo_steptime.GetLBText(3, curStr_steptime);
	steptime = _ttoi(curStr_steptime);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRJColorTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRJColorTestDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRJColorTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRJColorTestDlg::OnBnClickedBtnFindcenter()
{
	//在对话框上打定位画面

	//获取对话框上图片控件的句柄
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_PICTURE);
	//设置静态控件窗口风格为位图居中显示
	pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	CString cstr_pictureName;
	string str_pictureName;

	str_pictureName = "picture_ColorTest\\Center.bmp";
	cstr_pictureName = CString(str_pictureName.c_str());
	//显示图片
	pWnd->SetBitmap((HBITMAP)::LoadImage(NULL, cstr_pictureName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

	//CString strerr = timeChangeFormat();
	//AfxMessageBox(_T("aaaaa"));
}


DWORD CRJColorTestDlg::ThreadFunc_Gamma(LPVOID lpParam)
{
	DWORD dWait = WaitForSingleObject(g_hWaitGamma, INFINITE);//无限等待
	if (WAIT_OBJECT_0 == dWait)
	{
		CRJColorTestDlg* PDLG = (CRJColorTestDlg*)lpParam;
		//显示正在状态
		PDLG->m_isTestOver.SetItemText(0, 0, _T("GammaTest进行中..."));
		PDLG->m_isTestOver.SetItemData(0, 2);  //设置红底
		//设置对话框为顶层窗口
		PDLG->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		//获取系统时间，作为文件名称	
		CString timer = PDLG->timeChangeFormat();
		string filename = "TestResult\\" + CStringA(timer) + "-GammaTest.csv";
		//打开文件
		ofstream outGammaFile;
		outGammaFile.open(filename, ios::out);
		outGammaFile << "Gray" << ',' << "fx" << ',' << "fy" << ',' << "Lv" << ',' << "CCT" << endl;

		//在对话框上打测试画面
		PDLG->backBrush.DeleteObject();
		PDLG->backBrush.CreateSolidBrush((RGB(0, 0, 0)));
		CRect prect;
		PDLG->m_picture.GetClientRect(&prect);
		FillRect(PDLG->m_picture.GetDC()->GetSafeHdc(), &prect, PDLG->backBrush);

		for (int i = 0; i < 256; )
		{
			//更改测试W画面
			PDLG->backBrush.DeleteObject();
			PDLG->backBrush.CreateSolidBrush((RGB(i, i, i)));
			FillRect(PDLG->m_picture.GetDC()->GetSafeHdc(), &prect, PDLG->backBrush);
			PDLG->CA_Measure_SxSyLv();
			str_Gray = to_string(i);
			outGammaFile << str_Gray << ',' << str_fx << ',' << str_fy << ',' << str_Lv << ',' << str_CCT << endl;
			if (i == 0) {
				i += 3;
			}
			else {
				i += 4;
			}
		}

		for (int i = 0; i < 256; )
		{
			//更改测试B画面
			PDLG->backBrush.DeleteObject();
			PDLG->backBrush.CreateSolidBrush((RGB(0, 0, i)));
			FillRect(PDLG->m_picture.GetDC()->GetSafeHdc(), &prect, PDLG->backBrush);
			PDLG->CA_Measure_SxSyLv();
			str_Gray = to_string(i);
			outGammaFile << str_Gray << ',' << str_fx << ',' << str_fy << ',' << str_Lv << ',' << str_CCT << endl;
			if (i == 0) {
				i += 3;
			}
			else {
				i += 4;
			}
		}

		for (int i = 0; i < 256; )
		{
			//更改测试G画面
			PDLG->backBrush.DeleteObject();
			PDLG->backBrush.CreateSolidBrush((RGB(0, i, 0)));
			FillRect(PDLG->m_picture.GetDC()->GetSafeHdc(), &prect, PDLG->backBrush);
			PDLG->CA_Measure_SxSyLv();
			str_Gray = to_string(i);
			outGammaFile << str_Gray << ',' << str_fx << ',' << str_fy << ',' << str_Lv << ',' << str_CCT << endl;
			if (i == 0) {
				i += 3;
			}
			else {
				i += 4;
			}
		}

		for (int i = 0; i < 256; )
		{
			//更改测试R画面
			PDLG->backBrush.DeleteObject();
			PDLG->backBrush.CreateSolidBrush((RGB(i, 0, 0)));
			FillRect(PDLG->m_picture.GetDC()->GetSafeHdc(), &prect, PDLG->backBrush);
			PDLG->CA_Measure_SxSyLv();
			str_Gray = to_string(i);
			outGammaFile << str_Gray << ',' << str_fx << ',' << str_fy << ',' << str_Lv << ',' << str_CCT << endl;
			if (i == 0) {
				i += 3;
			}
			else {
				i += 4;
			}
		}

		outGammaFile.close();
		//显示完成状态
		PDLG->m_isTestOver.SetItemText(0, 0, _T("GammaTest完成"));
		PDLG->m_isTestOver.SetItemData(0, 1);  //设置绿底
		//取消对话框置顶
		PDLG->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
	return 0;
}

void CRJColorTestDlg::OnBnClickedBtnGammatest()
{
	g_hWaitGamma = CreateEvent(NULL, TRUE, TRUE, NULL);//创建一个初始为有信号的事件量
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc_Gamma, this, 0, 0);
}


DWORD CRJColorTestDlg::ThreadFunc_Color(LPVOID lpParam)
{
	DWORD dWait = WaitForSingleObject(g_hWaitColor, INFINITE);//无限等待
	if (WAIT_OBJECT_0 == dWait)
	{

		CRJColorTestDlg* PDLG = (CRJColorTestDlg*)lpParam;

		//显示正在状态
		PDLG->m_isTestOver.SetItemText(0, 0, _T("ColorTest进行中..."));
		PDLG->m_isTestOver.SetItemData(0, 2);  //设置红底
		//设置对话框为顶层窗口
		PDLG->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		//获取系统时间，作为文件名称
		CString timer = PDLG->timeChangeFormat();
		string filename = "TestResult\\" + CStringA(timer) + "-ColorTest.csv";
		//打开文件
		ofstream outColorFile;
		outColorFile.open(filename, ios::out);
		outColorFile << "Picture" << ',' << "fx" << ',' << "fy" << ',' << "Lv" << ',' << "CCT" << endl;
		//在对话框上打测试画面
		PDLG->backBrush.DeleteObject();
		PDLG->backBrush.CreateSolidBrush((RGB(255, 255, 255)));
		CRect prect;
		PDLG->m_picture.GetClientRect(&prect);
		FillRect(PDLG->m_picture.GetDC()->GetSafeHdc(), &prect, PDLG->backBrush);


		//测试 R 画面	
		PDLG->backBrush.DeleteObject();
		PDLG->backBrush.CreateSolidBrush((RGB(255, 0, 0)));
		FillRect(PDLG->m_picture.GetDC()->GetSafeHdc(), &prect, PDLG->backBrush);
		PDLG->CA_Measure_SxSyLv();
		str_Gray = "R";
		outColorFile << str_Gray << ',' << str_fx << ',' << str_fy << ',' << str_Lv << ',' << str_CCT << endl;


		//测试 G 画面	
		PDLG->backBrush.DeleteObject();
		PDLG->backBrush.CreateSolidBrush((RGB(0, 255, 0)));
		FillRect(PDLG->m_picture.GetDC()->GetSafeHdc(), &prect, PDLG->backBrush);
		PDLG->CA_Measure_SxSyLv();
		str_Gray = "G";
		outColorFile << str_Gray << ',' << str_fx << ',' << str_fy << ',' << str_Lv << ',' << str_CCT << endl;


		//测试 B 画面
		PDLG->backBrush.DeleteObject();
		PDLG->backBrush.CreateSolidBrush((RGB(0, 0, 255)));
		FillRect(PDLG->m_picture.GetDC()->GetSafeHdc(), &prect, PDLG->backBrush);
		PDLG->CA_Measure_SxSyLv();
		str_Gray = "B";
		outColorFile << str_Gray << ',' << str_fx << ',' << str_fy << ',' << str_Lv << ',' << str_CCT << endl;


		//测试 C 画面
		PDLG->backBrush.DeleteObject();
		PDLG->backBrush.CreateSolidBrush((RGB(0, 255, 255)));
		FillRect(PDLG->m_picture.GetDC()->GetSafeHdc(), &prect, PDLG->backBrush);
		PDLG->CA_Measure_SxSyLv();
		str_Gray = "C";
		outColorFile << str_Gray << ',' << str_fx << ',' << str_fy << ',' << str_Lv << ',' << str_CCT << endl;


		//测试 M 画面
		PDLG->backBrush.DeleteObject();
		PDLG->backBrush.CreateSolidBrush((RGB(255, 0, 255)));
		FillRect(PDLG->m_picture.GetDC()->GetSafeHdc(), &prect, PDLG->backBrush);
		PDLG->CA_Measure_SxSyLv();
		str_Gray = "M";
		outColorFile << str_Gray << ',' << str_fx << ',' << str_fy << ',' << str_Lv << ',' << str_CCT << endl;


		//测试 Y 画面
		PDLG->backBrush.DeleteObject();
		PDLG->backBrush.CreateSolidBrush((RGB(255, 255, 0)));
		FillRect(PDLG->m_picture.GetDC()->GetSafeHdc(), &prect, PDLG->backBrush);
		PDLG->CA_Measure_SxSyLv();
		str_Gray = "Y";
		outColorFile << str_Gray << ',' << str_fx << ',' << str_fy << ',' << str_Lv << ',' << str_CCT << endl;


		//获取对话框上图片控件的句柄
		CStatic* pWnd = (CStatic*)PDLG->GetDlgItem(IDC_PICTURE);
		//设置静态控件窗口风格为位图居中显示
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		CString cstr_pictureName;
		string str_pictureName;
		for (int i = 1; i <= 24; i++)
		{
			str_Gray = "X-rite_" + to_string(i);
			str_pictureName = "picture_ColorTest\\" + to_string(i) + ".bmp";
			cstr_pictureName = CString(str_pictureName.c_str());
			//显示图片
			pWnd->SetBitmap((HBITMAP)::LoadImage(NULL, cstr_pictureName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));
			PDLG->CA_Measure_SxSyLv();
			outColorFile << str_Gray << ',' << str_fx << ',' << str_fy << ',' << str_Lv << ',' << str_CCT << endl;
		}

		outColorFile.close();
		//显示完成状态
		PDLG->m_isTestOver.SetItemText(0, 0, _T("ColorTest完成"));
		PDLG->m_isTestOver.SetItemData(0, 1);  //设置绿底 
		//取消对话框置顶
		PDLG->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

		SetEvent(g_hWaitGamma);//转为有信号状态，其他线程的WaitForSingleObject会返回WAIT_OBJECT_0
	}
	return 0;
}

void CRJColorTestDlg::OnBnClickedBtnColortest()
{
	g_hWaitColor = CreateEvent(NULL, TRUE, TRUE, NULL);//创建一个初始为有信号的事件量
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc_Color, this, 0, 0);
}


DWORD CRJColorTestDlg::ThreadFunc_Contrast(LPVOID lpParam)
{
	SetEvent(g_hWaitContrast);
	DWORD dWait = WaitForSingleObject(g_hWaitContrast, INFINITE);//无限等待
	if (WAIT_OBJECT_0 == dWait)
	{

		CRJColorTestDlg* PDLG = (CRJColorTestDlg*)lpParam;

		//显示正在状态
		PDLG->m_isTestOver.SetItemText(0, 0, _T("ContrastTest进行中..."));
		PDLG->m_isTestOver.SetItemData(0, 2);  //设置红底
		//设置对话框为顶层窗口
		PDLG->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		//获取系统时间，作为文件名称	
		CString timer = PDLG->timeChangeFormat();
		string filename = "TestResult\\" + CStringA(timer) + "-ContrastTest.csv";
		//打开文件
		ofstream outContrastFile;
		outContrastFile.open(filename, ios::out);
		outContrastFile << "Gray" << ',' << "fx" << ',' << "fy" << ',' << "Lv" << ',' << "CCT" << endl;

		PDLG->backBrush.DeleteObject();
		PDLG->backBrush.CreateSolidBrush((RGB(255, 255, 255)));
		CRect prect;
		PDLG->m_picture.GetClientRect(&prect);
		FillRect(PDLG->m_picture.GetDC()->GetSafeHdc(), &prect, PDLG->backBrush);
		PDLG->CA_Measure_SxSyLv();
		str_Gray = "White";
		outContrastFile << str_Gray << ',' << str_fx << ',' << str_fy << ',' << str_Lv << ',' << str_CCT << endl;


		PDLG->backBrush.DeleteObject();
		PDLG->backBrush.CreateSolidBrush((RGB(0, 0, 0)));
		FillRect(PDLG->m_picture.GetDC()->GetSafeHdc(), &prect, PDLG->backBrush);
		PDLG->CA_Measure_SxSyLv();
		str_Gray = "Black";
		outContrastFile << str_Gray << ',' << str_fx << ',' << str_fy << ',' << str_Lv << ',' << str_CCT << endl;

		outContrastFile.close();
		//显示完成状态
		PDLG->m_isTestOver.SetItemText(0, 0, _T("ContrastTest完成"));
		PDLG->m_isTestOver.SetItemData(0, 1);  //设置绿底
		//取消对话框置顶
		PDLG->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

		SetEvent(g_hWaitColor);//转为有信号状态，其他线程的WaitForSingleObject会返回WAIT_OBJECT_0
	}
	return 0;
}

void CRJColorTestDlg::OnBnClickedBtnContrasttest()
{
	g_hWaitContrast = CreateEvent(NULL, TRUE, TRUE, NULL);//创建一个初始为有信号的事件量
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc_Contrast, this, 0, 0);
}


void CRJColorTestDlg::OnBnClickedBtnAlltest()
{
	g_hWaitContrast = CreateEvent(NULL, TRUE, FALSE, NULL);//创建一个手动复位、初始为无信号的事件
	g_hWaitColor = CreateEvent(NULL, TRUE, FALSE, NULL);//创建一个手动复位、初始为无信号的事件
	g_hWaitGamma = CreateEvent(NULL, TRUE, FALSE, NULL);//创建一个手动复位、初始为无信号的事件
	DWORD ThreadID_Contrast, ThreadID_Color, ThreadID_Gamma;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc_Contrast, this, 0, &ThreadID_Contrast);
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc_Color, this, 0, &ThreadID_Color);
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc_Gamma, this, 0, &ThreadID_Gamma);
}


void CRJColorTestDlg::OnBnClickedBtnUsertest()
{
	CString CStr_pictureName;
	GetDlgItem(IDC_EDIT_PictureName)->GetWindowText(CStr_pictureName);
	str_Gray = CStringA(CStr_pictureName);	
	CA_Measure_SxSyLv();

	if (((CButton*)GetDlgItem(IDC_CHECK_IsSaveData))->GetCheck())
	{
		outUserTestFile << str_Gray << ',' << str_fx << ',' << str_fy << ',' << str_Lv << ',' << str_CCT << endl;
	}
	
	Sleep(500);//等待500ms
	SetDlgItemText(IDC_EDIT_PictureName, _T("请填写画面名称"));	
}


void CRJColorTestDlg::OnBnClickedCheckIssavedata()
{
	int state = ((CButton*)GetDlgItem(IDC_CHECK_IsSaveData))->GetCheck();
	if (state == 1)
	{
		//获取系统时间，作为文件名称	
		CString timer = timeChangeFormat();
		string filename = "TestResult\\" + CStringA(timer) + "-UserTest.csv";
		//打开自定义测试的保存文件	
		outUserTestFile.open(filename, ios::app);
		outUserTestFile << "Picture" << ',' << "fx" << ',' << "fy" << ',' << "Lv" << ',' << "CCT" << endl;
	}
	else if (state == 0)
	{
		outUserTestFile.close();
	}
}


void CRJColorTestDlg::OnBnClickedBtnConnectca()
{
	// TODO: 在此添加控件通知处理程序代码
	// CA-SDK  创建和初始化 CA-SDK对象。使用SetConfiguration method
	long lcan = 1;
	_bstr_t strcnfig(_T("1"));
	long lprt = PORT_USB;
	long lbr = 38400;        //读取波特率设置
	_bstr_t strprbid(_T("P1"));
	_variant_t vprbid(_T("P1"));

	try {

		m_pCa200Obj = ICa200Ptr(__uuidof(Ca200));   // 1、创建CA200对象
		m_pCa200Obj->SetConfiguration(lcan, strcnfig, lprt, lbr);

		/*2、使用SetConfiguration()方法，通过以下设置将程序设置为使用1个CA单元
			CA unit CA number = 1
			Probe: 1 connected, probe number = 1
			USB connection
		*/
	}
	catch (_com_error e) {
		CString strerr;
		strerr.Format(_T("打开CA310失败"));
		AfxMessageBox(strerr);
		//return TRUE;
	}

	//在CA- sdk对象级别上执行用于控制CA单元的对象的设置
	m_pCasObj = m_pCa200Obj->Cas;      //从Ca200对象获取Cas集合
	m_pCaObj = m_pCasObj->ItemOfNumber[lcan];  //
	m_pOutputProbesObj = m_pCaObj->OutputProbes; //
	m_pOutputProbesObj->RemoveAll();          //
	m_pOutputProbesObj->Add(strprbid);     //
	m_pProbeObj = m_pOutputProbesObj->Item[vprbid]; //
	m_pMemoryObj = m_pCaObj->Memory;        //
	//各种对象用于执行以下CA单元初始化和内存通道设置
	m_pCaObj->SyncMode = SYNC_NTSC;   //将同步模式设置为NTSC
	m_pCaObj->AveragingMode = AVRG_FAST;  //设置快/慢模式为快
	m_pCaObj->SetAnalogRange(2.5, 2.5);  //设置模拟显示范围为2.5%，2.5%
	m_pCaObj->DisplayMode = DSP_LXY;     //设置显示模式
	m_pCaObj->DisplayDigits = DIGT_4;    //设置显示数字的数目为4

	//Set memory channel
	CString CStr_ChannelNO;
	GetDlgItem(IDC_ChannelNO)->GetWindowText(CStr_ChannelNO);
	m_pMemoryObj->ChannelNO = _ttol(CStr_ChannelNO);


	// CA-SDK 0 Cal校准
	CFont m_Font;      // 字体变量 m_Font 的声明。只能声明成局部变量，否则assert failed
	m_Font.CreatePointFont(100, _T("新宋体"));  // 设置字体的大小和格式
	//vResult_OpenCOM.SetFont(&m_Font, true);   // 设置OpenCOM列表框中的字体的格式
	m_isOpencaOK.SetFont(&m_Font, true);

	try {
		m_pCaObj->CalZero();

		m_isOpencaOK.SetItemText(0, 0, _T("CA310校准成功！"));
		m_isOpencaOK.SetItemData(0, 1);  //设置绿底

	}
	catch (_com_error e) {

		m_isOpencaOK.SetItemText(0, 0, _T("CA310校准失败！"));
		m_isOpencaOK.SetItemData(0, 2);  //设置红底

		CString strerr;
		strerr.Format(_T("CA310校准失败！"));
		AfxMessageBox(strerr);
		return;
	}
	//CButton* pb;
	//pb = (CButton*)GetDlgItem(IDC_BTN_ConnectCA); //测试按钮
	//pb->EnableWindow(FALSE);           //使测试按钮不可以操作

	//测试控件设置为可用
	HWND hBtn = ::GetDlgItem(m_hWnd, IDC_BTN_GammaTest);
	::EnableWindow(hBtn, TRUE);
	hBtn = ::GetDlgItem(m_hWnd, IDC_BTN_ColorTest);
	::EnableWindow(hBtn, TRUE);
	hBtn = ::GetDlgItem(m_hWnd, IDC_BTN_ContrastTest);
	::EnableWindow(hBtn, TRUE);
	hBtn = ::GetDlgItem(m_hWnd, IDC_BTN_AllTest);
	::EnableWindow(hBtn, TRUE);
	hBtn = ::GetDlgItem(m_hWnd, IDC_BTN_UserTest);
	::EnableWindow(hBtn, TRUE);
}


void CRJColorTestDlg::OnNMCustomdrawListOpenca(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMTVCUSTOMDRAW>(pNMHDR);
	NMCUSTOMDRAW nmCustomDraw = pNMCD->nmcd;
	switch (nmCustomDraw.dwDrawStage)
	{
	case CDDS_ITEMPREPAINT:
	{
		if (1 == nmCustomDraw.lItemlParam)
		{
			pNMCD->clrTextBk = RGB(0, 255, 0);
			pNMCD->clrText = RGB(0, 0, 0);
		}
		else if (2 == nmCustomDraw.lItemlParam)
		{
			pNMCD->clrTextBk = RGB(255, 0, 0);		//背景颜色
			pNMCD->clrText = RGB(0, 0, 0);		//文字颜色
		}
		else if (3 == nmCustomDraw.lItemlParam)
		{
			pNMCD->clrTextBk = RGB(200, 200, 200);
			pNMCD->clrText = RGB(128, 128, 128);
		}
		break;
	}
	default:
	{
		break;
	}
	}

	*pResult = 0;
	*pResult |= CDRF_NOTIFYPOSTPAINT;		//必须有，不然就没有效果
	*pResult |= CDRF_NOTIFYITEMDRAW;		//必须有，不然就没有效果
	return;
}


void CRJColorTestDlg::OnNMCustomdrawListTestresult(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMTVCUSTOMDRAW>(pNMHDR);
	NMCUSTOMDRAW nmCustomDraw = pNMCD->nmcd;
	switch (nmCustomDraw.dwDrawStage)
	{
	case CDDS_ITEMPREPAINT:
	{
		if (1 == nmCustomDraw.lItemlParam)
		{
			pNMCD->clrTextBk = RGB(0, 255, 0);
			pNMCD->clrText = RGB(0, 0, 0);
		}
		else if (2 == nmCustomDraw.lItemlParam)
		{
			pNMCD->clrTextBk = RGB(255, 50, 50);		//背景颜色
			pNMCD->clrText = RGB(0, 0, 0);		//文字颜色
		}
		else if (3 == nmCustomDraw.lItemlParam)
		{
			pNMCD->clrTextBk = RGB(200, 200, 200);
			pNMCD->clrText = RGB(128, 128, 128);
		}
		break;
	}
	default:
	{
		break;
	}
	}

	*pResult = 0;
	*pResult |= CDRF_NOTIFYPOSTPAINT;		//必须有，不然就没有效果
	*pResult |= CDRF_NOTIFYITEMDRAW;		//必须有，不然就没有效果
	return;
}


void CRJColorTestDlg::CA_Measure_SxSyLv()
{
	Sleep(steptime); //等待时间
	float fLv, fx, fy;
	long lT;
	//float fduv;
	for (int i = 0; i < 4; i++) {
		m_pCaObj->Measure(0);
		fLv = m_pProbeObj->Lv;
		fx = m_pProbeObj->sx;
		fy = m_pProbeObj->sy;
		lT = m_pProbeObj->T;
		//fduv = m_pProbeObj->duv;
		
		m_fx.Format(_T("%1.3f"), fx);
		m_fy.Format(_T("%1.3f"), fy);
		m_Lv.Format(_T("%.2f"), fLv);
		m_CCT.Format(_T("%5ld"), lT);

		SetDlgItemText(IDC_EDIT_fx, m_fx);
		SetDlgItemText(IDC_EDIT_fy, m_fy);
		SetDlgItemText(IDC_EDIT_Lv, m_Lv);
		SetDlgItemText(IDC_EDIT_CCT, m_CCT);

		str_fx = CStringA(m_fx);
		str_fy = CStringA(m_fy);
		str_Lv = CStringA(m_Lv);
		str_CCT = CStringA(m_CCT);		
		//UpdateData(FALSE);
	}
}


CString CRJColorTestDlg::timeChangeFormat()
{
	time_t rawtime;
	struct tm timeinfo;
	char s[100];
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	strftime(s, sizeof(s), "%Y%m%d-%H-%M-%S", &timeinfo);

	string strTime = s;
	CString strerr(strTime.c_str());
	return strerr;
}

void CRJColorTestDlg::OnCbnSelchangeCombosteptime()
{
	int curSel_steptime = vCombo_steptime.GetCurSel();
	CString curStr_steptime;
	vCombo_steptime.GetLBText(curSel_steptime, curStr_steptime);
	steptime = _ttoi(curStr_steptime);
}


void CRJColorTestDlg::OnBnClickedOk()
{
	if (m_pCaObj != NULL)
	{
		m_pCaObj->RemoteMode = 0;
	}

	outUserTestFile.close();

	
	CDialog::OnOK();
}

void CRJColorTestDlg::OnBnClickedCancel()
{
	if (m_pCaObj != NULL)
	{
		m_pCaObj->RemoteMode = 0;
	}

	outUserTestFile.close();

	CDialog::OnCancel();
}


BOOL CRJColorTestDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN://屏蔽回车
			return TRUE;
		case VK_ESCAPE://屏蔽Esc
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}