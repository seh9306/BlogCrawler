
// BlogCrawlerApplication.cpp: 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "BlogCrawlService.h"
#include "BlogArticleDao.h"
#include "CrawlerApplication.h"
#include "CrawlerApplicationDlg.h"
#include "HttpKeepAliveClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCrawlerApplicationApp

BEGIN_MESSAGE_MAP(CCrawlerApplicationApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCrawlerApplicationApp 생성

CCrawlerApplicationApp::CCrawlerApplicationApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CCrawlerApplicationApp 개체입니다.

CCrawlerApplicationApp theApp;


// CCrawlerApplicationApp 초기화

BOOL CCrawlerApplicationApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;
	
	// MFC 컨트롤의 테마를 사용하기 위해 "Windows 원형" 비주얼 관리자 활성화
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	SetRegistryKey(_T("Crawler"));

	CCrawlerApplicationDlg dlg;

	observer::ObserverList observers;
	observers.emplace_back(&dlg);

	Initialize(observers);
	
	dlg.SetArticleBlogDao(blogArticleDao_);
	m_pMainWnd = &dlg;

	INT_PTR nResponse = dlg.DoModal();

	blogArticleDao_->Uninitialize();

	if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 응용 프로그램이 예기치 않게 종료됩니다.\n");
		TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
	}

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	return FALSE;
}

void CCrawlerApplicationApp::RunCrawlService() const
{
	for (auto& crawlService : crawlSerivces_)
	{
		crawlService->Execute();
	}
}

void CCrawlerApplicationApp::Initialize(observer::ObserverList& observers)
{
	blogArticleDao_ = std::make_shared<dao::BlogArticleDao>();

	blogArticleDao_->Initialize();

	crawlSerivces_.push_back(std::make_unique<service::BlogCrawlService>());

	for (auto& crawlService : crawlSerivces_)
	{
		crawlService->CreateCrawlers();
		crawlService->SetProgressObserver(observers);
		crawlService->SetDao(static_cast<void*>(&blogArticleDao_));
	}

	CreateDirectoryIfNotExist(L"images");
}

void CCrawlerApplicationApp::CreateDirectoryIfNotExist(std::wstring path)
{
	CFileFind file;

	std::wstring search = path;
	search.append(L"*.*");

	if (file.FindFile(search.c_str()))
	{
		return;
	}

	path.append(L"\\");

	if (!CreateDirectory(path.c_str(), NULL))
	{
		return;
	}

	return;
}
