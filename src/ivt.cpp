/*
 * ivt.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */
#include "Ivt.h"
#include <DOS.H>
#include <IOSTREAM.H>
#include "Lock.h"
#include "KerEve.h"
#include "Timer.h"
#include "Event.h"




IVTEntry::IVTEntry(IVTNo num , pInterrupt  newRout){
	Lock::lock();
			//	cout<<" 				IVTEntry::IVTentry"<< endl; //OVO ISPISE
    IVTEntry::IVT[num] = this;
    		//	if (IVTEntry::IVT[9]!=0) cout<< "IVT[9] je razl od nule "<<endl;
    		//	else cout<< "IVT[9] je nula "<<endl;
	this->entryNum= num;
			//	cout<< "num "<<(int)  IVTEntry::IVT[(int)entryNum]->entryNum<< "!"<<endl;
	//this->myKerEvent=?;
#ifndef BCC_BLOCK_IGNORE
	oldRout = getvect(entryNum);
	setvect(entryNum, newRout); // ja bi ovde malo signal da posaljem koa newRout
#endif
	Lock::unlock();
}
IVTEntry::~IVTEntry(){
	Lock::lock();
	this->setBackOld();
    IVTEntry::IVT[entryNum] = 0; 	/* nisam siguran
    								 * da ovo treba
    								 */
    this->myKerEvent=0;

    Lock::unlock();
}
 void IVTEntry::callOld(){
	 /* ako bude gresaka nekih, ovde mozes if (old) old();
	  */
	 oldRout();
 }
 void IVTEntry::signal(){
	 /* Logicno ali nisam siguran da treba ovako
	  * */
	 if (myKerEvent) myKerEvent->signal(); // jer moze da ne bude jos dodeljen
	 	 	 	 	 else cout<<"NIJE SIGNAL DODELJEN";
 }

 void IVTEntry::setKernelEvent(KernelEv* ke, IVTNo num){
			     //	cout<<"				IVTEntry::setKernelEvent"<<endl;
	 	 	 	 //	 iz 66 puta proradilo sa ovom metodom
	IVTEntry::IVT[(int)num]->myKerEvent= ke;
}

void IVTEntry::setBackOld(){
#ifndef BCC_BLOCK_IGNORE
	setvect(entryNum,oldRout);
#endif
}


