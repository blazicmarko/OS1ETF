#ifndef KERNELS_H_
#define KERNELS_H_

#include "semaphor.h"
#include "init.h"
#include "list.h"

class List;

class KernelSem {
public:
	KernelSem (int init=1);
	virtual ~KernelSem();

	virtual int wait(Time maxTimeToWait);
	virtual int signal(int n=0);

	int val () const;
	void timerClock();

	int semValue;
	List *blockedThreads;
	List *sleepingThreads;
};



#endif
