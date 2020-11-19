/*
 * SemQue.cpp
 *
 *  Created on: Aug 21, 2019
 *      Author: OS1
 */
#include "SemQue.h"
#include "KerSem.h"
#include "Queue.h"
#include "Lock.h"
#include <IOSTREAM.H>
//Izbaci lock odavde !!!

SemQueue::SemQueue(){
					/*	Lock::lock();
					 *	cout<< " 		SemQueue::SemQueue"<<endl;
					 *	Lock::unlock();*/
	first=last= 0;
	SemQueue::length=0;
}
SemQueue::~SemQueue(){
					/*	Lock::lock();
								cout<< "			SemQueue::~SemQueue"<<endl;
						Lock::unlock();*/
	while (first){
		Elem* old= first;
		first= first->next;
		delete old;
	}
}
void SemQueue::addElem(KernelSem* elem){
					//		cout<< "			SemQueue::addElem"<<endl;
	last= (!first ? first : last->next)= new Elem(elem);
	this->length++;
						//cout<<SemQueue::length<<endl;
}
KernelSem* SemQueue::getFirstElem(){
							//cout<< "			SemQueue::getFirstElem"<<endl;
	if (first==0) return 0; //AKO JE PRAZNA VRACA NULL
	Lock::lock();
	this->length--;
	Elem* tmp= first;
	first= first->next;
	tmp->next= 0; // Za svaki slucaj prekidan vezu
	if (this->length==0) last= first=0;
	Lock::unlock();
	KernelSem* tmpp= tmp->elem_sem;
	tmp->elem_sem=0;
	delete tmp;
	return tmpp;
}
int SemQueue::getLength(){
	return this->length;
}
void SemQueue::deleteThreadBySem(Semaphore* sem){
	if (first==0) return;
	SemQueue::length--;
						//cout<< "			SEMQUEUE::deleteThreadbySem"<<endl;
	Elem* tmp=first;
	Elem* prev= 0;
	while (tmp){
		if (tmp->elem_sem->mySem== sem) break;
		prev= tmp;
		tmp= tmp->next;
	}
	if (prev==0){ //prvi
		first=first->next;
		tmp->next=0;
		if (length==0) last=first;
	} else if (tmp==last){ //poslednji
		last= prev;
		last->next= 0;
	} else {	//izmedju
		prev->next= tmp->next;
		tmp->next=0;
	}
	delete tmp;
}

void SemQueue::updateSemaphore_tick(){
							/* Lock::lock();
							 * cout<< " 		SEMQUEUE::updateSemaphore_tick"<<endl;
							 * Lock::unlock();
						  	 * #ifndef BCC_BLOCK_IGNORE
							 *		//asm {cli;}
							 * #endif
							 * OVAJ LOCK MOZDA MOZE UNUTRA, RAZMISLI, ili uopste ne treba jer
							 * u timeru niko nece da preuzme kontrolu
							 */
	Elem* tmp=first;
	while (tmp){
		tmp->elem_sem->waitQue->updateWaitQue_tick();
		tmp= tmp->next;
	}
	Lock::unlock();
}

