#ifndef OBSERVER_OBSERVER_H
#define OBSERVER_OBSERVER_H

#include "ProgressDefine.h"

namespace observer
{

class ProgressObserver
{
public:
	ProgressObserver();
	virtual ~ProgressObserver();

	virtual void Update(ProgressComplete progress) = 0;
};

}

#endif