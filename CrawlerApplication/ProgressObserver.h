#ifndef OBSERVER_OBSERVER_H
#define OBSERVER_OBSERVER_H

namespace observer
{

class ProgressObserver
{
public:
	ProgressObserver();
	virtual ~ProgressObserver();

	virtual void Update(int progress) = 0;
};

}

#endif