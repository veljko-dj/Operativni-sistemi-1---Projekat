/*
 * KerSem.h
 *
 *  Created on: Aug 21, 2019
 *      Author: OS1
 */
#include "Thread.h"
#ifndef KERSEM_H_
#define KERSEM_H_
class Semaphore;
class SemQueue;
class Queue;


class KernelSem{
public:
	KernelSem(Semaphore* mySem, int v);
	virtual ~KernelSem();

	int wait(Time maxTimeToWait);
	int  signal(int n);
	int value() const;

	//void updateWaitQue();
	static SemQueue* semaphores;
protected:
	friend class Semaphore;
	friend class SemQueue;
private:
	Queue* waitQue;
	Semaphore* mySem;
	int val;

};



#endif /* KERSEM_H_ */
