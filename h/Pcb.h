/*
 * Pcb.h
 *
 *  Created on: Aug 16, 2019
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "param.h"
#include "Thread.h"

class Thread; //navodim da postoji da bih
class Queue; //da bih mogao pokazivac

class PCB{
public:
	PCB(StackSize stSize, Time tiSlice, Thread* Thr);
	~PCB();
	static void wrapper();

	//pokusaj ubacivanja ovde
	// PREBACUJEM OVDE DA NE BIH IMAO PROBLEM duplicated in methode
	// bilo u parametri.h :( :( :(
	static volatile PCB* running;
	static Queue* threads;
	static Idle* idle;
	static PCB* main_thr;
	static volatile int running_cnt;
	static volatile int on_demand;



protected:
	friend class Thread; //omogucavam priv pod
	friend class Queue;
	friend class Idle;
	friend class Timer;
	friend class KernelSem;
	friend class KernelEv;
private:
	unsigned sp;  		// registri
	unsigned ss;
	unsigned bp;

	unsigned *myStack;
	StackSize myStackSize;

	//int kvant, za sada nepotrebno
	Time myTimeSlice;

	volatile pcbStatus myStatus;

	Thread* myThread;

	ID myID;

	Queue* myWaitQue;

	//privatne metode
	void createStack();
	void notifyImDone();
};

#endif /* PCB_H_ */
