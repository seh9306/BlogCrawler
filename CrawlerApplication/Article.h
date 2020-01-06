#ifndef MODEL_ARTICLE_H
#define MODEL_ARTICLE_H

#include <string>
#include <ctime>

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
		content_(content),
		date_(0)
	{
		id_ = -1;
	}

	Article(std::string title,
		std::string url,
		std::string imagePath,
		std::string content,
		std::time_t date)
		: title_(title),
		url_(url),
		imagePath_(imagePath),
		content_(content),
		date_(date)
	{
		id_ = -1;
	}

	Article(int id,
		std::string title,
		std::string url,
		std::string imagePath
	)
		: Article(title, url, imagePath, "")
	{
		id_ = id;
		date_ = 0;
	}

	Article(const Article& article)
	{
		id_ = article.id_;
		title_ = article.title_;
		url_ = article.url_;
		imagePath_ = article.imagePath_;
		content_ = article.content_;
		date_ = article.date_;
	}

	int id_;
	std::string title_;
	std::string url_;
	std::string imagePath_;
	std::string content_;
	std::time_t date_;

};



}

#endif