/*
 * Event.cpp
 *
 *  Created on: Aug 24, 2019
 *      Author: OS1
 */


#include "Event.h"
#include "KerEve.h"
#include "Lock.h"


 Event::Event(IVTNo num	){
	 Lock::lock();
	 this->myImpl= new KernelEv(num);
	 Lock::unlock();
 }
 void Event::signal(){
	 myImpl->signal();
 }
 void Event::wait(){
	 myImpl->wait();
 }
 Event::~Event(){
	 Lock::lock();
	 delete myImpl;
	 Lock::unlock();
 }

