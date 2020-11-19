/*
 * Idle.cpp
 *
 *  Created on: Aug 17, 2019
 *      Author: OS1
 */

#include <DOS.H>
#include <IOSTREAM.H>
#include "SCHEDULE.h"
#include "Idle.h"
#include "param.h"
#include "Queue.h"
#include "Thread.h"
#include "Timer.h"
#include "Lock.h"


Idle::Idle():Thread(defaultStackSize/16, defaultTimeSlice/2){
					/*Lock::lock();
					cout<< "			Idle::Idle()"<<endl;
					Lock::unlock();*/
}

void Idle::run(){
	volatile unsigned char x=5;
	while(x++){
		if (x==100) x=10;
	}
}
void Idle::start(){
	/*
	 * Za sada su mi apsolutno nebitni podaci tipa status i tako to
	 */
}
