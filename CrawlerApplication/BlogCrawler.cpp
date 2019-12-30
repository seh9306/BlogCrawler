#include "stdafx.h"
#include "BlogCrawler.h"

namespace crawler
{

namespace blog
{

BlogCrawler::BlogCrawler()
	: ctx_(boost::asio::ssl::context::sslv23),
	resolver_(ioContext_)
{
	ctx_.set_verify_mode(boost::asio::ssl::verify_none);
}

BlogCrawler::~BlogCrawler()
{
}

bool BlogCrawler::Crawl()
{
	if (!RequestForGettingArchives())
	{
		return false;
	}

	if (!RequestForGettingArticles())
	{
		return false;
	}
	
	if (!InsertArticles())
	{
		return false;
	}

	return true;
}

}

}