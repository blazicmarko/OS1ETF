#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "list.h"
#include "init.h"

enum State;
class List;

class PCB {
public:
	PCB(Thread* myT, StackSize stackSize, Time timeSlice);
	~PCB();

	Thread* myThread;
	unsigned int sp, ss;

	State state;
	int returnValue;

	Time pcbTimeSlice, timeToWait;
	StackSize pcbStackSize;
	unsigned *pcbSP;
	int id;

	List* waitList;

	void createStack();
};

#endif
