#ifndef CRAWLER_CRAWLER_H
#define CRAWLER_CRAWLER_H

namespace {
	constexpr char* const kHttpsProtocolName = "https";
}

namespace crawler
{

class Crawler
{
public:
	Crawler() {};
	virtual ~Crawler() {};

	virtual bool Crawl() = 0;
	virtual void SetDao(void* dao) = 0;
};

}

#endif 
