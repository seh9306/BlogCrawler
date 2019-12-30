#ifndef MODEL_ARTICLE_H
#define MODEL_ARTICLE_H

#include <string>

namespace model
{

struct Article
{
	Article(std::string title,
		std::string url,
		std::string content,
		std::string imagePath = "")
		: title(title),
		url(url),
		content(content),
		imagePath(imagePath)
	{
	}

	std::string title;
	std::string url;
	std::string imagePath;
	std::string content;
};

}

#endif