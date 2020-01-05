
#ifndef VIEW_CRAWLER_APPLICATION_H
#define VIEW_CRAWLER_APPLICATION_H

#include <codecvt>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "Article.h"
#include "ProgressObserver.h"

namespace dao
{
class BlogArticleDao;
}

// CBlogCrawlerApplicationDlg 대화 상자
class CCrawlerApplicationDlg 
	: public CDialogEx,
	public observer::ProgressObserver

{
// 생성입니다.
public:
	CCrawlerApplicationDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	void SetArticleBlogDao(std::shared_ptr<dao::BlogArticleDao>& blogArticleDao);
	void Update(observer::ProgressComplete progress) override;
	
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BLOGCRAWLERAPPLICATION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedCrawlingButton();
	afx_msg void OnClickedSearchButton();
	afx_msg void OnGetdispinfoArticleList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnArticleItemClickList(NMHDR *pNMHDR, LRESULT *pResult);

private:
	CButton crawlingButton_;
	CProgressCtrl progressCtrl_;
	CEdit searchEdit_;
	CButton searchButton_;
	CListCtrl searchList_;
	CImageList imageList_;

	std::shared_ptr<dao::BlogArticleDao> blogArticleDao_;
	model::ArticleList articles_;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter_;
	std::unordered_map<int, int> imageInfoMap_;
	
	std::mutex progressMutex_;

	DECLARE_MESSAGE_MAP()

};

#endif