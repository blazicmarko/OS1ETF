#include "SCHEDULE.H"
#include "semaphor.h"
#include "kernels.h"
#include "init.h"

Semaphore::Semaphore(int init) {
	lock();
	myImpl = new KernelSem(init);
	Init::kernelsems->insert(myImpl);
	unlock();
}

Semaphore::~Semaphore() {
	lock();
	delete myImpl;
	unlock();
}

int Semaphore::wait(Time maxTimeToWait) {
	int r;
	lock();
	r = myImpl->wait(maxTimeToWait);
	unlock();
	return r;
}

int Semaphore::signal(int n) {
	int r;
	lock();
	r = myImpl->signal(n);
	unlock();
	return r;
}

int Semaphore::val() const {
	lock();
	int x = myImpl->val();
	unlock();
	return x;
}
