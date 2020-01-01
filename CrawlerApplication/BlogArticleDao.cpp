#include "stdafx.h"
#include "BlogArticleDao.h"

#include "Query.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using std::placeholders::_4;

namespace 
{
	constexpr char* const kDbFilePath = u8"blog_article.db";

	constexpr char* const articleTableName = "ARTICLE";

	constexpr int kBadAction = -1;
	constexpr int kSqliteOK = SQLITE_OK;

	int ArticleTableCheck(void *data, int argc, char **argv, char **azColName) 
	{
		auto existTable = static_cast<bool*>(data);

		if (!(*existTable))
		{
			if (std::strcmp(articleTableName, argv[0]) == 0)
			{
				*existTable = true;
			}
			
		}

		return kSqliteOK;
	}

	int Select(void *data, int argc, char **argv, char **azColName) 
	{
		auto articleActionData = static_cast<dao::ArticleActionData*>(data);
		auto actionData = articleActionData->actionData_;
		auto& actions = actionData.actions_;
		auto action = actions.find(actionData.actionIndex_);

		if (action == actions.end())
		{
			return kBadAction;
		}

		return action->second(argc, argv, azColName, &(articleActionData->articles_));
	}

	int DoNothing(void *null, int argc, char **argv, char **azColName) 
	{
		return kSqliteOK;
	}

	constexpr int kSelectAllAritcle = 0;

	constexpr int kId = 0;
	constexpr int kTitle = 1;
	constexpr int kUrl = 2;
	constexpr int kImagePath = 3;
	constexpr int kContent = 4;

}


namespace dao
{

BlogArticleDao::BlogArticleDao()
{
}

BlogArticleDao::~BlogArticleDao()
{
}

bool BlogArticleDao::Initialize()
{
	actions_[kSelectAllAritcle] = std::bind(&BlogArticleDao::SelectAllAritcle, this, _1, _2, _3, _4);

	if (DBopen() != kSqliteOK)
	{
		return false;
	}

	bool existTable = false;
	Sqlite3Exec(CHECK_TABLE_QUERY, ArticleTableCheck, static_cast<void*>(&existTable));

	if (!existTable)
	{
		if (Sqlite3Exec(CREATE_ARTICLE_TABLE_QUERY) != kSqliteOK)
		{
			return false;
		}
	}

	return true;
}

bool BlogArticleDao::Uninitialize()
{
	sqlite3_close(db_);

	return true;
}

bool BlogArticleDao::InsertArticles(ArticleList& articles)
{
	return Sqlite3Exec(INSERT_ARTICLES_QUERY) == kSqliteOK;
}

ArticleList BlogArticleDao::SelectArticles()
{
	ArticleList articles;

	Sqlite3Exec(SELECT_ALL_ARTICLES_QUERY, kSelectAllAritcle, articles);

	return articles;
}

bool BlogArticleDao::DBopen()
{
	return sqlite3_open(kDbFilePath, &db_);
}

int BlogArticleDao::SelectAllAritcle(int argc, char** argv, char** azColName, ArticleList* ptrArticles)
{
	if (argc == 0)
	{
		return kBadAction;
	}
	ptrArticles->emplace_back(argv[kTitle], argv[kUrl], argv[kImagePath], argv[kContent]);

	return kSqliteOK;
}

int BlogArticleDao::Sqlite3Exec(const char* const query)
{
	return Sqlite3Exec(query, DoNothing, nullptr);
}

int BlogArticleDao::Sqlite3Exec(const char* const query, sqlite3_callback callback, void* data)
{
	char *zErrMsg = 0;

	int rc = sqlite3_exec(db_, query, callback, data, &zErrMsg);

	if (rc != kSqliteOK) {
		sqlite3_free(zErrMsg);
	}

	return rc;
}

int BlogArticleDao::Sqlite3Exec(const char* const query, int actionIndex, ArticleList& articles)
{
	ArticleActionData actionData(articles, actions_, actionIndex);

	return Sqlite3Exec(query, Select, static_cast<void*>(&actionData));
}

}
