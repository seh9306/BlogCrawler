#ifndef OBSERVER_PROGRESS_DEFINE_H
#define OBSERVER_PROGRESS_DEFINE_H

#include <map>

namespace observer
{

enum ProgressComplete {
	kComplete = 1,											// 2

	kCompleteDevMicrosoftBlogMakePageUrls = 1000,			// 6
	kCompleteDevMicrosoftBlogParsePages,					// 6
	kCompleteDevMicrosoftBlogRequestAndGetPages,			// 1
	kCompleteDevMicrosoftBlogMakeArticleUrls,				// 6
	kCompleteDevMicrosoftBlogParseArticles,					// 6
	kCompleteDevMicrosoftBlogRequestAndGetArticles,			// 1
	kCompleteDevMicrosoftBlogMappingArticles,				// 1
	kCompleteDevMicrosoftBlogInsertArticles,				// 1

	kCompleteHerbsutterBlogMakePageUrls = 2000,				// 10
	kCompleteHerbsutterBlogParsePages,						// 10
	kCompleteHerbsutterBlogReqeustAndGetPages,				// 1
	kCompleteHerbsutterBlogMappingArticles,					// 1
	kCompleteHerbsutterBlogInsertArticles,					// 1

};

}

#endif