/*
 * Semaphor.cpp
 *
 *  Created on: Aug 21, 2019
 *      Author: OS1
 */

#include "Semaphor.h"
#include "Lock.h"
#include "KerSem.h"
#include "SemQue.h"


Semaphore::Semaphore(int init ){
	Lock::lock();
	myImpl= new KernelSem(this, init);
	Lock::unlock();
}
Semaphore::~Semaphore(){
	Lock::lock();
	delete myImpl;
	Lock::unlock();
}
int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}
int Semaphore::signal( int n){
	return myImpl->signal(n);
}

int  Semaphore::val() const{
	return myImpl->value();
}



