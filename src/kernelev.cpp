#include "init.h"

class PCB;
class KernelSem;
typedef unsigned char IVTNo;

class KernelEv {
public:
	KernelEv(IVTNo number);
	~KernelEv();
	void wait();
	void signal();
private:
	IVTNo ivtNo;
	PCB *maker;
	KernelSem *mySem;
};

class IVTEntry {
public:
	static IVTEntry* entries[256];

	unsigned ivtNo;
	KernelEv* myEvent;
	void interrupt (*oldRoutine)(...);

	IVTEntry(int number, void interrupt (*newRoutine)(...));
   ~IVTEntry();
	void signal();
	void callOldInterrupt();
};

KernelEv::KernelEv(IVTNo number) {
	lock();
	maker = Init::running;
	ivtNo = number;
	mySem = new KernelSem(0);
	IVTEntry::entries[ivtNo]->myEvent = this;
	unlock();
}

KernelEv::~KernelEv () {
	lock();
	IVTEntry::entries[ivtNo]->myEvent = 0;
	delete mySem;
	unlock();
}

void KernelEv::wait () {
	lock();
	if (Init::running != maker) {
		unlock();
		return;
	}
	mySem->wait(0);
	unlock();
}

void KernelEv::signal() {
	lock();
	if (mySem->val() == 1) {
		unlock();
		return;
	}
	mySem->signal();
	unlock();
}
