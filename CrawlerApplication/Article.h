#ifndef MODEL_ARTICLE_H
#define MODEL_ARTICLE_H

#include <string>

namespace model
{

struct Article;

using ArticleList = std::vector<model::Article>;

struct Article
{
	Article() {}

	Article(std::string url)
		: url_(url)
	{
		id_ = -1;
	}

	Article(std::string title,
			std::string url,
			std::string imagePath,
			std::string content)
		: title_(title),
		url_(url),
		imagePath_(imagePath),
		content_(content)
	{
		id_ = -1;
	}

	Article(int id,
		std::string title,
		std::string url,
		std::string imagePath,
		std::string content)
		: Article(title, url, imagePath, content)
	{
		id_ = id;
	}

	Article(const Article& article)
	{
		id_ = article.id_;
		title_ = article.title_;
		url_ = article.url_;
		imagePath_ = article.imagePath_;
		content_ = article.content_;
	}

	Article(const Article&& article)
	{
		id_ = article.id_;
		title_ = std::move(article.title_);
		url_ = std::move(article.url_);
		imagePath_ = std::move(article.imagePath_);
		content_ = std::move(article.content_);
	}

	int id_;
	std::string title_;
	std::string url_;
	std::string imagePath_;
	std::string content_;
};



}

#endif