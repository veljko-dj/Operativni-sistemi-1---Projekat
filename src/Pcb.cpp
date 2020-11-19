/*
 * Pcb.cpp
 *
 *  Created on: Aug 16, 2019
 *      Author: OS1
 */
#include <DOS.H>
#include <IOSTREAM.H>
#include "SCHEDULE.h"
#include "Idle.h"
#include "param.h"
#include "Pcb.h"
#include "Queue.h"
#include "Thread.h"
#include "Timer.h"
#include "Lock.h"

/*running_cnt=0;
on_demand=0;*/

ID pocID=0;

//  Queue* PCB::threads=0;
volatile PCB* PCB::running=0;
Idle* PCB::idle=0;
PCB* PCB::main_thr=0;
volatile int PCB::running_cnt=0;
volatile int PCB::on_demand=0;


void PCB::wrapper(){
						/*Lock::lock();
						cout<<"	PCB::WRAPPER"<<endl;
						Lock::unlock();*/
	PCB::running->myThread->run();
	((PCB*)PCB::running)->notifyImDone();
	PCB::running->myStatus= DONE;
	Timer::dispatch();
}
PCB::PCB(StackSize stSize, Time tiSlice, Thread *thr){
						/*Lock::lock();
 	 	 	 	 	 	 cout << "Konstruktor PCB" <<endl;
						Lock::unlock();*/
	//Lock::lock();
	/* Sklanjam lock jer cu komandu NEW PCB uvek pozivati iz osiguranog dela. */
	this->myID =++pocID;
	this->myStackSize= stSize/sizeof(unsigned);
	if (stSize>65536) myStackSize= 65000;
	this->myStatus=CREATED;
	this->myTimeSlice= tiSlice;
	this->myThread= thr;
	this->myWaitQue= new Queue();
	this->createStack();
	//Lock::unlock();
}

void PCB::createStack(){
	myStack= new unsigned[myStackSize];
	myStack[myStackSize-5]= 0x200;
#ifndef BCC_BLOCK_IGNORE
		myStack[myStackSize-1] = FP_SEG(myThread);
		myStack[myStackSize-2] = FP_OFF(myThread);

		myStack[myStackSize-6] = FP_SEG(PCB::wrapper);
		myStack[myStackSize-7] = FP_OFF(PCB::wrapper);
		this->sp = FP_OFF(myStack+myStackSize-16);
		this->ss = FP_SEG(myStack+myStackSize-16);
#endif
	this->bp= this->sp;
}
PCB::~PCB(){
					/*Lock::lock();
					cout<<"	PCB::~PCB"<< endl;
					Lock::unlock();
	//iz PCB::threads izbaci u ~Thread   */
	//nema potrebe za LOCKovima jer pozivam iz osiguranog LOCK
	if (this->myWaitQue) delete myWaitQue;
	if (this->myStack) delete myStack;
	this->myThread=0;
}
void PCB::notifyImDone(){
					/*Lock::lock();
					 cout<<"		___PCB::NOTIFY IM DONE"<<endl;
					Lock::unlock();*/
	PCB* tmp;
	while(1){														//!!! OVDE JE BIO LOCK ALI MOZDA NEMA POTREBE
		//Lock::lock();
		tmp= this->myWaitQue->getFirstElem();
		if (tmp==0) { /*Lock::unlock();*/ break; }
		tmp->myStatus = READY;
		Lock::lock();
		Scheduler::put(tmp);
		Lock::unlock();
	}
}


