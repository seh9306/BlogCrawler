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
	}

	std::string title_;
	std::string url_;
	std::string imagePath_;
	std::string content_;
};



}

#endif