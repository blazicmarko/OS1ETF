#include "SCHEDULE.H"
#include "semaphor.h"
#include "kernels.h"
#include "init.h"
#include "list.h"


KernelSem::KernelSem(int init) {
	lock();
	semValue = init;
	blockedThreads = new List();
	sleepingThreads = new List();
	unlock();
}

KernelSem::~KernelSem() {
	lock();
	delete blockedThreads;
	delete sleepingThreads;
	unlock();
}

int KernelSem::wait(Time maxTimeToWait) {
	lock();
	Init::running->returnValue = 1;
	if (--semValue < 0) {
		Init::running->state = BLOCKED;
		if (maxTimeToWait == 0) {
			blockedThreads->insert(Init::running);
		} else {
			Init::running->timeToWait = maxTimeToWait;
			sleepingThreads->insert(Init::running);
		}
		dispatch();
	}
	unlock();
	return Init::running->returnValue;
}

int KernelSem::signal(int n) {
	lock();
	if (n < 0) {
		unlock();
		return n;
	}
	if (n == 0) {
		if (++semValue <= 0) {
			PCB *x = blockedThreads->get();
			if (x == 0) x = sleepingThreads->get();
			x->state = READY;
			Scheduler::put(x);
		}
		unlock();
		return 0;
	} else {
		int num = 0;
		for (int i=0; i<n; i++) {
			if (++semValue <= 0) {
				PCB *x = blockedThreads->get();
				if (x == 0) x = sleepingThreads->get();
				x->state = READY;
				Scheduler::put(x);
				num++;
			}
		}
		unlock();
		return num;
	}
}

int KernelSem::val() const {
	lock();
	int x = semValue;
	unlock();
	return x;
}

void KernelSem::timerClock() {
	lock();
	List::Node *cur = sleepingThreads->head, *prev = 0, *old = 0;

	while (cur) {
		if (cur->pcb->timeToWait > 0) cur->pcb->timeToWait--;

		if (cur->pcb->timeToWait == 0) {
			cur->pcb->state = READY;
			cur->pcb->returnValue = 0;

			Scheduler::put(cur->pcb);

			old = cur;
			cur = cur->next;
			if (!prev)
				sleepingThreads->head = cur;
			else
				prev->next = cur;
			delete old;
			semValue++;
		} else {
			prev = cur;
			cur = cur->next;
		}
	}
	unlock();
}
