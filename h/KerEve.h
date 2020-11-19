/*
 * KerEve.h
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#ifndef KEREVE_H_
#define KEREVE_H_

class PCB;
class Event;
typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;

class KernelEv{

public:
	KernelEv(IVTNo num);
	~KernelEv();

	void signal();
	void wait();
protected:
	friend class IVTEntry;
private:
	int val;
	Event* myEvent; //ispade da nije potreban
	PCB* myOwner;
	IVTNo myIVTNo;
	int blocked;
};



#endif /* KEREVE_H_ */
