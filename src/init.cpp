#include "schedule.h"
#include <dos.h>
#include <stdlib.h>
#include "init.h"
#include "pcb.h"
#include "thread.h"
#include "idle.h"
#include "lists.h"

class ListSem;
unsigned Init::dispatched = 0;
unsigned Init::timerCounter = defaultTimeSlice;
int nextId = 0;
void interrupt (*Init::oldRoutine)(...) = 0;
PCB* Init::running = 0;
Thread* Init::starting = 0;
Idle* Init::idle = 0;
List *Init::threads = new List();
ListSem *Init::kernelsems = new ListSem();


void Init::begin() {
	lock();
	oldRoutine = getvect(0x08);
	setvect(0x08, timerIR);

	starting = new Thread(4096, 2);
	starting->myPCB->state = READY;
	running = starting->myPCB;

	idle = new Idle();
	idle->start();
	unlock();
}

void Init::terminate() {
	lock();
	setvect(0x08, oldRoutine);
	delete starting;
	delete idle;
	delete threads;
	delete kernelsems;
	unlock();
}

static unsigned tsp, tss;
static ListSem::NodeSem *tek;

void interrupt Init::timerIR(...) {

	if (!dispatched) {
		for (tek=kernelsems->head; tek; tek=tek->next)
			tek->sem->timerClock();

		tick(); 
		(*oldRoutine)();
		if (running->pcbTimeSlice != 0) timerCounter++;
		if (timerCounter < running->pcbTimeSlice || running->pcbTimeSlice == 0) return;
	} 

	dispatched = 0; 

	asm {
		mov tsp, sp
		mov tss, ss
	}

	running->sp = tsp;
	running->ss = tss;

	if (running->state == READY && running != idle->myPCB) Scheduler::put(running);

	running = Scheduler::get();
	if (running == 0) running = idle->myPCB;

	timerCounter = 0;

	tsp = running->sp;
	tss = running->ss;
		
	asm {
		mov sp, tsp
		mov ss, tss
	}
}

void wrapper() {
	Init::running->myThread->run();

	lock();
	Init::running->state = OVER;

	PCB* temp = Init::running->waitList->get();
	while (temp != 0) {
		temp->state = READY;
		Scheduler::put(temp);
		temp = Init::running->waitList->get();
	}

	dispatch();
}
