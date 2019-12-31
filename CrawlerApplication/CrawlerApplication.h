
// BlogCrawlerApplication.h: PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

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

class CCrawlerApplicationApp : public CWinApp
{
public:
	CCrawlerApplicationApp();

	virtual BOOL InitInstance();
	void RunCrawlService() const;

private:
	void InitServices();

	std::vector<std::unique_ptr<service::CrawlService>> crawlSerivces;

	DECLARE_MESSAGE_MAP()
};

extern CCrawlerApplicationApp theApp;
