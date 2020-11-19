/*
 * Thread.h
 *
 *  Created on: Aug 17, 2019
 *      Author: OS1
 */

#ifndef THREAD_H_
#define THREAD_H_
// Param su u parametri.h
typedef unsigned long StackSize; //ovo sve izvuceno iz THREAD.h
typedef unsigned int Time;
typedef int ID;
const StackSize defaultStackSize=4096;//2048
const Time defaultTimeSlice=2;
class PCB;

class Thread{
public:
	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);
protected:
	friend class Timer;
	friend class PCB;
	friend class Idle;
	Thread(StackSize stackSize=defaultStackSize, Time timeSlice= defaultTimeSlice);
	virtual void run() {}
private:
	PCB* myPCB;
};

void dispatch();
#endif /* THREAD_H_ */
