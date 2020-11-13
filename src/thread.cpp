#include "init.h"
#include "pcb.h"
#include "thread.h"
#include "schedule.h"
#include <dos.h>
#include <stdlib.h>
#include "list.h"

class List;

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock();
	myPCB = new PCB(this, stackSize, timeSlice);
	unlock();
}

Thread::~Thread() {
	lock();
	waitToComplete();
	delete myPCB;
	unlock();
}

void Thread::start() {
	lock();
	myPCB->state = READY;
	myPCB->createStack();
	Scheduler::put(myPCB);
	unlock();
}

void Thread::waitToComplete() {
	lock();
	if (myPCB->state == OVER || myPCB->state == NEW) { unlock(); return; }
	if (myPCB == Init::running) { unlock(); return; }
	if (this == Init::starting) { unlock(); return; }
	if (this == Init::idle) { unlock(); return; }

	Init::running->state = BLOCKED;
	myPCB->waitList->insert(Init::running);
	dispatch();
	unlock();
}

void dispatch() {
	lock();
	Init::dispatched = 1;
	Init::timerIR();
	unlock();
}

ID Thread::getId() { return myPCB->id; }
ID Thread::getRunningId() { return Init::running->id; }
Thread* Thread::getThreadById(ID id) {
	Thread *x = 0;
	lock();
	for (List::Node *tek = Init::threads->head; tek; tek = tek->next)
		if (tek->pcb->id == id) {
			x = tek->pcb->myThread;
			break;
		}
	unlock();
	return x;
}
