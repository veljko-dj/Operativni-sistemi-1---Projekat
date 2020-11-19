/*
 * Queue.cpp
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


Queue::Queue(){
	first=last= 0;
	length=0;
}
Queue::~Queue(){
	while (first){
		Elem* old= first;
		first= first->next;
		delete old;
	}
}
void Queue::addElem(PCB* elem, int time){
			/*	Lock::lock();
				cout<< "			QUEUE::addELEM";
				this->countById(-5);
				Lock::unlock();*/
	last= (!first ? first : last->next)= new Elem(elem,0,time);
	length++;
	//cout<<endl<<"dodao jednu";
	//this->countById(elem->myID);
}
PCB* Queue::getFirstElem(){
				/*Lock::lock();
				cout<< "			QUEUE::getFirstElem"<<endl;
				Lock::unlock();*/
	if (first==0) return 0; //AKO JE PRAZNA VRACA NULL
	length--;
	Elem* tmp= first;
	Elem *prev=0;
	while (tmp){ //ovako radim jer je problem kad izbacujem iz liste
				//preko signal, moze da se desi da ubacim u sch neku nit koja je vec
				//u sch
		if (tmp->waitSemTime!=0) break;
		prev=tmp;
		tmp=tmp->next;
	}
	if (tmp==0) return 0;
	if (prev==0) {
		first=first->next;
		tmp->next=0;
		if (length==0) last=first;
	}else if (tmp==last){ //poslednji
		last= prev;
		last->next= 0;
	} else {	//izmedju
		prev->next= tmp->next;
		tmp->next=0;
	}// Za svaki slucaj prekidan vezu

	PCB *tmpp= tmp->elem_pcb;
	tmp->elem_pcb=0;
	delete tmp;
	return tmpp;
}
int Queue::getLength(){
	return length;
}
Thread* Queue::deleteThreadByID(ID idd){
				/*Lock::lock();
				cout<< "			QUEUE::deleteThreadbyID"<<endl;
				Lock::unlock();*/
	if (first==0)return 0;
	Elem* tmp=first;
	Elem* prev= 0;
	while (tmp){
		if (tmp->elem_pcb->myID==idd) break;
		prev= tmp;
		tmp= tmp->next;
	}
	if (prev==0){ //prvi
		first=first->next;
		tmp->next=0;
		if (length==1) last=first;
	} else if (tmp==last){ //poslednji
		last= prev;
		last->next= 0;
	} else {	//izmedju
		prev->next= tmp->next;
		tmp->next=0;
	}
	length--;
	Thread* tmpp= tmp->elem_pcb->myThread;
	tmp->elem_pcb->myThread=0;
	delete tmp;
	return tmpp;
}
Thread* Queue::findThreadById(ID idd){
				/*Lock::lock();
				cout<< "			QUEUE::findThreadbyID"<<endl;
				Lock::unlock();*/
	Elem* tmp=first;
	Elem* prev= 0;
	while (tmp){
		if (tmp->elem_pcb->myID==idd) break;
		prev= tmp;
		tmp= tmp->next;
	}
	Thread* tmpp= tmp->elem_pcb->myThread;
	tmp->elem_pcb=0;
	delete tmp;
	return tmpp;
}
Time Queue::deleteThreadByIDAndReturnTime(ID idd){
				//cout<< "			QUEUE::deleteThreadbyID-"<<idd<<endl;
	if (first==0)	return 123;
	Elem* tmp=first;
	Elem* prev= 0;
//	this->countById(idd);
	while (tmp){
		if (tmp->elem_pcb->myID==idd) break;
		prev= tmp;
		tmp= tmp->next;
	}
	if (tmp==0)	return 123;		/* kad se deesi signal izbacim iz liste, ali
								 * posle wait-a on ispituje ovom funkciom, i ako
								 * nije u listi moram da vratim nesto
								 * razlicitio od nule!!!!
								 */
//	cout<<"Pronasao sam tmp->elem_pcb->myID"<<tmp->elem_pcb->myID;
	if (prev==0){ //prvi
		first=first->next;
		tmp->next=0;
		if (length<=2) last=first;
	} else if (tmp==last){ //poslednji
		last= prev;
		last->next= 0;
	} else {	//izmedju
		prev->next= tmp->next;
		tmp->next=0;
	}
	length--;
	/*Lock::lock();
	this->countById(idd);
	Lock::unlock();*/
	int tmpp= tmp->waitSemTime;
	delete tmp;
	return tmpp;
}
void Queue::countById(ID idd){
	Elem* tmp=first;
	Elem* prev= 0;
	int i=0,j=0;
	while (tmp){
		if (tmp->elem_pcb->myID==idd) i++;
		prev= tmp; j++;
		tmp= tmp->next;
	}
//	cout<<endl<<"idd: "<< idd<< " u listi"<< i<<"/"<<j<"puta";
}
void Queue::updateWaitQue_tick(){
	Elem* tmp=first;
	while (tmp!=last->next){
		//NISAM SIGURAN KADA TREBA IZBACITI, kada postane nula ili  u sledecem taktu
		if (tmp->waitSemTime>0){
			tmp->waitSemTime--;
			if (tmp->waitSemTime==0) {
						Lock::lock();
								/*	cout<< endl<<"_Spremna_"<<tmp->elem_pcb->myID<<endl;
										#ifndef BCC_BLOCK_IGNORE
											//asm {cli;}
										#endif*/
						tmp->elem_pcb->myStatus= READY;
						Scheduler::put(tmp->elem_pcb);
						Lock::unlock();
						// ne izbacujem iz liste, da bih posle mogao da proverim waitTime==0
						// izbacujem posle, kada prvi put sch dobije svoj red
					}
		}
		tmp= tmp->next;
	}
}

