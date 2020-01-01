#ifndef DAO_BLOG_ARTICLE_DAO_H
#define DAO_BLOG_ARTICLE_DAO_H

#include "sqlite3.h"

#include <functional>
#include <unordered_map>

#include "Article.h"


namespace dao
{

typedef std::vector<model::Article> ArticleList;
typedef std::unordered_map<int, std::function<int(int, char**, char**, ArticleList*)>> ActionMap;


struct ActionData
{
	ActionData(ActionMap& actions, int actionIndex)
		: actions_(actions),
		actionIndex_(actionIndex)
	{}

	ActionMap& actions_;
	int actionIndex_;
};

struct ArticleActionData 
{
	ArticleActionData(ArticleList& articles, ActionMap& actions, int actionIndex)
		: articles_(articles),
		actionData_(actions, actionIndex)
	{}

	ArticleList& articles_;
	ActionData actionData_;
};

class BlogArticleDao
{
public:
	BlogArticleDao();
	~BlogArticleDao();
	bool Initialize();
	bool Uninitialize();

	bool InsertArticles(ArticleList& articles);
	ArticleList SelectArticles();

	bool DBopen();

private:
	int SelectAllAritcle(int argc, char **argv, char **azColName, ArticleList* ptrArticles);

	int Sqlite3Exec(const char* const query);
	int Sqlite3Exec(const char* const query, sqlite3_callback callback, void* data);
	int Sqlite3Exec(const char* const query, int actionIndex, ArticleList& articles);

	sqlite3* db_;
	ActionMap actions_;

};

}

#endif