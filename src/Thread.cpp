/*
 * Thread.cpp
 *
 *  Created on: Aug 17, 2019
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

void dispatch(){
	Timer::dispatch();
}

Thread::Thread(StackSize stackSize, Time timeSlice){
	// Treba lock jer taj neko ne vidi lock.h
	Lock::lock();
							//cout<< "	THREAD::THREAD"<< endl;
	this->myPCB= new PCB(stackSize,timeSlice,this);
	PCB::threads->addElem(this->myPCB);
	Lock::unlock();
}

Thread::~Thread(){
	//waitToComplete()???
	Lock::lock();
						//cout<<"	THREAD::~THREAD"<<endl;
	PCB::threads->deleteThreadByID(this->myPCB->myID);
	delete myPCB;
	Lock::unlock();
	myPCB=0;
}


void Thread::start(){
				/*	Lock::lock();
				 * cout<<"		THREAD::START"<<endl;
				 * Lock::unlock();
				 * if (this->myPCB->myStatus== CREATED)  cout<<"Status je CREATED"<<endl;
				 */
	if (this->myPCB->myStatus!= CREATED) return;
	Lock::lock();
	this->myPCB->myStatus=  READY;
	Scheduler::put(this->myPCB);
	Lock::unlock();
	/* if (this!=PCB::idle){
	 * 		Sredjeno u IDLE::start()
	 * 		da se ne bih ovde mucio.
	 */
}
void Thread::waitToComplete(){
								/* Lock::lock();
								 * cout<<"		___THREAD::WAIT TO COMPLETE"<<endl;
								 * Lock::unlock();
								 */
	if (this->myPCB!= PCB::running &&
			//this->myPCB->myStatus!= CREATED &&
				this->myPCB->myStatus!=  DONE
					//&& (this->myPCB== idle->myPCB)
				&& (this!= PCB::idle)
				){
									//cout<<"BLOKIRAM NIT"<< PCB::running->myID<<endl;
		this->myPCB->myWaitQue->addElem((PCB*)PCB::running);
		PCB::running->myStatus= BLOCKED;
		Timer::dispatch();
	}
}

ID Thread::getId(){
	return this->myPCB->myID;
}
ID Thread::getRunningId(){
	return PCB::running->myID;
}
Thread* Thread::getThreadById(ID idd){
	Lock::lock();
							//cout<<"	THREAD::getThreadbyId"<<endl;
	Thread *tmp=  PCB::threads->findThreadById(idd);
	Lock::unlock();
	return tmp;
}
