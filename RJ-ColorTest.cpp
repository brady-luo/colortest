
// RJ-ColorTest.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "RJ-ColorTest.h"
#include "RJ-ColorTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRJColorTestApp

BEGIN_MESSAGE_MAP(CRJColorTestApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CRJColorTestApp 构造

CRJColorTestApp::CRJColorTestApp()
{
	// 支持重新启动管理器
	//m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CRJColorTestApp 对象

CRJColorTestApp theApp;


// CRJColorTestApp 初始化

BOOL CRJColorTestApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	//AfxEnableControlContainer();


	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//必须进行OLE初始化，CA退出时才不会报错
	if (!AfxOleInit())           
	{
		//AfxMessageBox(_T("AfxOleInit"));
		return FALSE;
	}

	AfxEnableControlContainer();



#ifdef _AFXDLL

#else
	//Enable3dControlsStatic();
#endif


	if (RunEmbedded() || RunAutomated())
	{


		COleTemplateServer::RegisterAll();
	}
	else
	{
		COleObjectFactory::UpdateRegistryAll();
	}

	CRJColorTestDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{

	}
	else if (nResponse == IDCANCEL)
	{

	}



	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

