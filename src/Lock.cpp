/*
 * Lock.cpp
 *
 *  Created on: Aug 20, 2019
 *      Author: OS1
 */

#include "Lock.h"
#include "Pcb.h"
#include "Timer.h"

//OVO JE ZA TIMER
int Lock::val=0;
int Lock::saved_demand=0;


void Lock::lock(){
	val++;
}

void Lock::unlock(){
	if(--val == 0)
		if (Lock::saved_demand == 1)
			Timer::dispatch();
	//if (val<0) val=0; za sada nepotrebno
}

