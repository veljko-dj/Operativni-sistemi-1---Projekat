/*
 * KerEve.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#include "KerEve.h"
#include "Ivt.h"
#include "Lock.h"
#include "Pcb.h"
#include "Timer.h"
#include "SCHEDULE.H"
#include <iostream.h>

KernelEv::KernelEv(IVTNo num) {
					// cout<<"	    KernelEV:Kernel"<<endl;
	val =1; //koja je def vrednost?
	myIVTNo = num;
	myOwner =(PCB*) PCB::running;
	blocked=0;
					// if (IVTEntry::IVT[num]==0) cout<<" NULAAAAAAAA"<<endl;
	IVTEntry::IVT[(int)num]->setKernelEvent(this, myIVTNo);
}
KernelEv::~KernelEv(){
	this->myOwner=0;
	this->myEvent=0;
	IVTEntry::IVT[this->myIVTNo]=0;
}



void KernelEv::wait() {
	Lock::lock();
	if (PCB::running!= myOwner ) {
					cout<< "		PCB::running!= myOwner	return"<<endl;
		Lock::unlock();
		return;
	}
	if (val==1){
		val--;
		Lock::unlock();
	}else {
		PCB::running->myStatus = BLOCKED;
		blocked=1;
				//cout<<endl<<"__BLOKIRANA_NA_EVENTU_"<<PCB::running->myID<<"__";
		Lock::unlock();
		Timer::dispatch();
	}
}
void KernelEv::signal(){
	Lock::lock();
						//	cout<<endl<< "      KernelEv::signal() ";
		if (val==1) {Lock::unlock(); return; }
		if (val == 0) {
			if (blocked){
				blocked=0;
				/* AKO OVAJ SIGNAL SME DA ODBLOKIRA NESTO STO JE ZAKUCANO NA
				 * SEMAFORU A NE EVENTU ONDA OVDE MOZE USLOV DA BUDE OWNERSTATUS==BLOCKED
				 */
				myOwner->myStatus= READY;
				Scheduler::put(myOwner);
			} else val=1;
		}
	Lock::unlock();
}
