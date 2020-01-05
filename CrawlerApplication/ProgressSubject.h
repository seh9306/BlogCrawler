#ifndef OBSERVER_SUBJECT_H
#define OBSERVER_SUBJECT_H

#include <vector>

namespace observer
{

class ProgressObserver;
using ObserverList = std::vector<ProgressObserver*>;

class ProgressSubject
{
public:
	ProgressSubject();
	virtual ~ProgressSubject();

	void Notify(int progress);
	void Attach(ProgressObserver* observer);
	void Detach(ProgressObserver* observer);

private:
	ObserverList observers_;
};

}

#endif