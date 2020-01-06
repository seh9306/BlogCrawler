
// BlogCrawlerApplicationDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "CrawlerApplication.h"
#include "CrawlerApplicationDlg.h"
#include "afxdialogex.h"

#include <algorithm>
#include <thread>

#include "BlogArticleDao.h"
#include "ProgressDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{

static int GetWorkScore(observer::ProgressComplete progress)
{
	if (progress == observer::kComplete)
	{
		static int count = 0;
		if (++count == 2)
		{
			return 100;
		}
		else
		{
			return 0;
		}
	}

	static std::map<observer::ProgressComplete, int> progressScoreMap
		= { 
		{ observer::kCompleteDevMicrosoftBlogMakePageUrls           , 3 },
		{ observer::kCompleteDevMicrosoftBlogParsePages             , 3 },
		{ observer::kCompleteDevMicrosoftBlogRequestAndGetPages     , 3 },
		{ observer::kCompleteDevMicrosoftBlogMakeArticleUrls        , 3 },
		{ observer::kCompleteDevMicrosoftBlogParseArticles          , 3 },
		{ observer::kCompleteDevMicrosoftBlogRequestAndGetArticles  , 3 },
		{ observer::kCompleteDevMicrosoftBlogMappingArticles        , 1 },
		{ observer::kCompleteDevMicrosoftBlogInsertArticles         , 1 },

		{ observer::kCompleteHerbsutterBlogMakePageUrls             , 1 },
		{ observer::kCompleteHerbsutterBlogParsePages               , 1 },
		{ observer::kCompleteHerbsutterBlogReqeustAndGetPages       , 1 },
		{ observer::kCompleteHerbsutterBlogMappingArticles          , 1 },
		{ observer::kCompleteHerbsutterBlogInsertArticles           , 1 } 
	};

	return progressScoreMap.find(progress)->second;
}

}

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CCrawlerApplicationDlg 대화 상자



CCrawlerApplicationDlg::CCrawlerApplicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BLOGCRAWLERAPPLICATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCrawlerApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, crawlingButton_);
	DDX_Control(pDX, IDC_PROGRESS1, progressCtrl_);
	DDX_Control(pDX, IDC_EDIT1, searchEdit_);
	DDX_Control(pDX, IDC_BUTTON2, searchButton_);
	DDX_Control(pDX, IDC_LIST1, searchList_);
}

BEGIN_MESSAGE_MAP(CCrawlerApplicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CCrawlerApplicationDlg::OnBnClickedCrawlingButton)
	ON_BN_CLICKED(IDC_BUTTON2, &CCrawlerApplicationDlg::OnClickedSearchButton)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, &CCrawlerApplicationDlg::OnGetdispinfoArticleList)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CCrawlerApplicationDlg::OnArticleItemClickList)
END_MESSAGE_MAP()


// CCrawlerApplicationDlg 메시지 처리기

BOOL CCrawlerApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	CRect rect;
	searchList_.GetWindowRect(rect);
	searchList_.InsertColumn(0, TEXT("thumnail"), LVCFMT_LEFT, rect.Width()*0.4);
	searchList_.InsertColumn(1, TEXT("title"), LVCFMT_LEFT, rect.Width()*0.3);
	searchList_.InsertColumn(2, TEXT("url"), LVCFMT_LEFT, rect.Width()*0.3);

	searchList_.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	progressCtrl_.SetRange(0, 100);
	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CCrawlerApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCrawlerApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCrawlerApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCrawlerApplicationDlg::OnBnClickedCrawlingButton()
{
	crawlingButton_.EnableWindow(FALSE);
	std::thread([&] {
		auto app = reinterpret_cast<CCrawlerApplicationApp*>(AfxGetApp());

		if (app != nullptr)
		{
			app->RunCrawlService();
		}
		progressCtrl_.SetPos(100);
	}).detach();
}

void CCrawlerApplicationDlg::OnClickedSearchButton()
{
	searchButton_.EnableWindow(FALSE);
	std::thread([&] {
		CString searchCstring;
		searchEdit_.GetWindowTextW(searchCstring);

		std::string search = converter_.to_bytes(searchCstring.GetBuffer());
		articles_ = blogArticleDao_->SelectArticles(search);

		imageList_.Detach();
		imageList_.Create(300, 300, ILC_COLOR32, 0, 1000);

		CImage image, temp;
		CRect rect;
		
		searchList_.GetWindowRect(rect);
		auto width = rect.Width() * 0.4;
		auto imageCount = 0;

		for (auto& article : articles_)
		{
			if (article.imagePath_.empty())
			{
				continue;
			}
			auto imagePathUTF16 = converter_.from_bytes(article.imagePath_);

			if (image.Load(imagePathUTF16.c_str()) != S_OK)
			{
				continue;
			}

			temp.Create(width, 300, image.GetBPP());

			HDC hdc = temp.GetDC();

			SetStretchBltMode(hdc, HALFTONE);
			image.StretchBlt(hdc, 0, 0, width, 300, 0, 0, image.GetWidth(), image.GetHeight(), SRCCOPY);
			temp.ReleaseDC();

			CBitmap bitmap;
			bitmap.Attach(temp.Detach());

			imageList_.Add(&bitmap, ILC_COLOR32);
			image.Detach();

			imageInfoMap_[article.id_] = imageCount++;
		}

		searchList_.SetImageList(&imageList_, LVSIL_SMALL);
		searchList_.SetItemCountEx(static_cast<int>(articles_.size()), LVSICF_NOINVALIDATEALL | LVSICF_NOSCROLL);
		
		searchButton_.EnableWindow(TRUE);
	}).detach();
}

void CCrawlerApplicationDlg::SetArticleBlogDao(std::shared_ptr<dao::BlogArticleDao>& blogArticleDao)
{
	blogArticleDao_ = blogArticleDao;
}

void CCrawlerApplicationDlg::Update(observer::ProgressComplete progress)
{
	std::lock_guard<std::mutex> guard(progressMutex_);
	
	auto score = GetWorkScore(progress);
	
	if (score == 100)
	{
		blogArticleDao_->Uninitialize();
		MessageBox(_T("크롤링이 완료되었습니다."), _T("Crawler"), 0);
	}
	progressCtrl_.SetPos(progressCtrl_.GetPos() + score);
}

void CCrawlerApplicationDlg::OnGetdispinfoArticleList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LV_ITEM* pItem = reinterpret_cast<LV_ITEM*>(&(pDispInfo)->item);

	if (pItem == NULL) return;

	int nRow = pItem->iItem;
	int nCol = pItem->iSubItem;

	if (nRow < 0 || nRow >= articles_.size())
	{
		return;
	}

	auto& article = articles_.at(nRow);
	
	if (pItem->pszText)
	{
		std::wstring content;
		switch (nCol)
		{
		case 0:
			break;
		case 1:
			content = converter_.from_bytes(article.title_);
			std::wcsncpy(pItem->pszText, content.c_str(), content.size() + 1);
			break;
		case 2:
			content = converter_.from_bytes(article.url_);
			std::wcsncpy(pItem->pszText, content.c_str(), content.size() + 1);
			break;
		}
	}

	if (pItem->mask & LVIF_IMAGE)
	{
		auto imageIndex = imageInfoMap_.find(article.id_);
		if (imageIndex != imageInfoMap_.end())
		{
			pItem->iImage = imageIndex->second;
		}
		
	}

	*pResult = 0;
}

void CCrawlerApplicationDlg::OnArticleItemClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (pNMItemActivate->iItem < 0 || pNMItemActivate->iItem > articles_.size())
	{
		return;
	}

	auto& article = articles_.at(pNMItemActivate->iItem);
	auto url = converter_.from_bytes(article.url_);
	
	ShellExecute(NULL, L"open", url.c_str(), L"", L"", SW_SHOW);

	*pResult = 0;
}
