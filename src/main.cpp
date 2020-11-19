/*
 * main.cpp
 *
 *  Created on: Aug 18, 2019
 *      Author: OS1
 */

#include "Timer.h"
#include "SemQue.h"
#include "KerSem.h"
#include "Queue.h"
#include "Pcb.h"
#include "Ivt.h"

SemQueue* KernelSem::semaphores= new SemQueue();
Queue* PCB::threads= new Queue();
IVTEntry* IVTEntry::IVT[256];

/*	 MAIN
  	ZA KORISNIKA	*/
extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	Timer::inic();

	int ret_value = userMain(argc, argv);

	Timer::restore();

	return ret_value;
}



