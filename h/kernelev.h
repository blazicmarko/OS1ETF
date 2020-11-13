#ifndef _KERNELEV_H_
#define _KERNELEV_H_

#include "event.h"
typedef unsigned char IVTNo;
class PCB;
class KernelSem;

class KernelEv {
public:
	KernelEv(IVTNo number);
	~KernelEv();
	void wait();
	void signal();

	IVTNo ivtNo;
	PCB *maker;
	KernelSem *mySem;
};

#endif

