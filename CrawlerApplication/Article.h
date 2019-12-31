#ifndef MODEL_ARTICLE_H
#define MODEL_ARTICLE_H

#include <string>

namespace model
{

struct Article
{
	Article() {}

	Article(std::string url)
		: url(url)
	{
	}

	std::string title;
	std::string url;
	std::string imagePath;
	std::string content;
};

}

#endif