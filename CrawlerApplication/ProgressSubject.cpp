#include "stdafx.h"
#include "ProgressSubject.h"

#include "ProgressObserver.h"

namespace observer
{

ProgressSubject::ProgressSubject()
{
}

ProgressSubject::~ProgressSubject()
{
}

void ProgressSubject::Notify(int progress)
{
	for (auto observer : observers_)
	{
		observer->Update(progress);
	}
}

void ProgressSubject::Attach(ProgressObserver* observer)
{
	observers_.emplace_back(observer);
}

void ProgressSubject::Detach(ProgressObserver* observer)
{
	for (auto iter = observers_.begin(); iter != observers_.end(); ++iter) {
		if ((*iter) == observer)
		{
			iter = observers_.erase(iter);
			break;
		}
	}	
}

}