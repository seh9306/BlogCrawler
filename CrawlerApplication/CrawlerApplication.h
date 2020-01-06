
#ifndef CRAWLER_APPLICATION_H
#define CRAWLER_APPLICATION_H

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include <vector>
#include <memory>

#include "CrawlService.h"
#include "resource.h"		// 주 기호입니다.

// CBlogCrawlerApplicationApp:
// 이 클래스의 구현에 대해서는 BlogCrawlerApplication.cpp을(를) 참조하세요.
//

namespace dao
{
	class BlogArticleDao;
}

class CCrawlerApplicationApp : public CWinApp
{
public:
	CCrawlerApplicationApp();

	virtual BOOL InitInstance();
	void RunCrawlService() const;

private:
	void Initialize(observer::ObserverList& observers);
	void CreateDirectoryIfNotExist(std::wstring path);

	std::vector<std::unique_ptr<service::CrawlService>> crawlSerivces_;
	std::shared_ptr<dao::BlogArticleDao> blogArticleDao_;

	DECLARE_MESSAGE_MAP()
};

extern CCrawlerApplicationApp theApp;

#endif