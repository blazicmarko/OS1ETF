#include <dos.h>

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

	IVTEntry(int number, void interrupt (*newRoutine)(...));
   ~IVTEntry();
	void signal();
	void callOldInterrupt();
};

IVTEntry* IVTEntry::entries[256];

IVTEntry::IVTEntry(int number, void interrupt (*newRoutine)(...)) {
	lock();
	myEvent = 0;
	ivtNo = number;
	entries[ivtNo] = this;
	Init::oldRoutine = getvect(ivtNo);
	setvect(ivtNo, newRoutine);
	unlock();
}

IVTEntry::~IVTEntry() {
	lock();
	Init::oldRoutine();
	setvect(ivtNo, Init::oldRoutine);
	unlock();
}

void IVTEntry::signal() {
	lock();
	if (myEvent != 0) {
		myEvent->signal();
	}
	unlock();
}

void IVTEntry::callOldInterrupt() {	Init::oldRoutine(); }

