#include "pcb.h"
#include "init.h"
#include <stdlib.h>
#include <dos.h>

int n;
unsigned *stack;

PCB::PCB(Thread* myT, StackSize stackSize, Time timeSlice) {
	lock();
	Init::threads->insert(this);
	state = NEW;
	pcbStackSize = stackSize / 2;
	myThread = myT;
	pcbSP = 0;
	id = nextId++;
	pcbTimeSlice = timeSlice;
	waitList = new List();
	unlock();
}

PCB::~PCB() {
	lock();
	delete waitList;
	delete pcbSP;
	unlock();
}

void PCB::createStack() {
  	lock();
	n = pcbStackSize;
	stack = new unsigned[n];

	stack[n-1] = 0x200;
	stack[n-2] = FP_SEG(&wrapper);
	stack[n-3] = FP_OFF(&wrapper);

	sp = FP_OFF(stack+n-12);
	ss = FP_SEG(stack+n-12);
	pcbSP = stack;
	unlock();
}
