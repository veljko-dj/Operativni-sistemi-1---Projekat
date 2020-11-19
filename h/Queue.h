/*
 * Queue.h
 *
 *  Created on: Aug 16, 2019
 *      Author: OS1
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "Thread.h"
class PCB;
class Thread;
// RED IZ ZBIRKE ZA C++, STRANA 67!
class Queue{
public:
	Queue();
	//Queue(const Queue& que); za sada nije potrebno
	~Queue();
	void addElem( PCB* elem, int time=-1);
	PCB* getFirstElem();
	int getLength();
	Thread* deleteThreadByID(ID idd);
	Thread* findThreadById(ID idd);
	Time deleteThreadByIDAndReturnTime(ID idd);
	void  updateWaitQue_tick();

	void  countById(ID idd);

private:
	struct Elem{
		PCB* elem_pcb;
		Elem* next;
		int waitSemTime;
		Elem(PCB* pcb, Elem* sled=0, int wait=-1){
			elem_pcb=pcb;
			next= sled;
			waitSemTime=wait;
		}
	};
	Elem *first, *last;
	int length;



};



#endif /* QUEUE_H_ */
