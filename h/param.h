/*
 * param.h
 *
 *  Created on: Aug 16, 2019
 *      Author: OS1
 */

#ifndef PARAM_H_
#define PARAM_H_

#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
#include <IOSTREAM.H>
#include <stdlib.h>
#include "Lock.h"
class Queue;
class Idle;
class PCB;

/*static void lock(){
#ifndef BCC_BLOCK_IGNORE
	asm {cli;}
#endif
}

static void unlock(){
#ifndef BCC_BLOCK_IGNORE
	asm {sti;}
#endif
}*/

enum pcbStatus {CREATED, DONE, READY, RUNNING, BLOCKED};

/*
 volatile PCB* running;
 volatile int running_cnt=0;
 volatile int on_demand=0;

typedef unsigned long StackSize; //ovo sve izvuceno iz THREAD.h
typedef unsigned int Time;
typedef int ID;
const StackSize defaultStackSize=4096;
const Time defaultTimeSlice=2;


int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
	Lock::lock();
		va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	Lock::unlock();
		return res;
}*/

/*
 /*ova dva reda dole sam prebacio u inic ! ! !
 Queue* threads= new Queue();
Idle* idle= new Idle();
Queue* threads;
Idle* idle;

PCB* main_thr;
 */
/* Samo sam iskopirao od prethodnih godina
 * jer i Uros(ja mislim) je napisao u onom fajlu 2018/19 da
 * svi to prepisemo, tako da sam racunao da je copy/paste okej
 */





#define PREPAREENTRY( ivtNo, callOldR) \
void interrupt inter##ivtNo(...); \
IVTEntry ivtEntry##ivtNo(ivtNo,inter##ivtNo); \
void interrupt inter##ivtNo(...) { \
	ivtEntry##ivtNo.signal(); \
	if(callOldR == 1) \
		ivtEntry##ivtNo.callOld(); \
}

#endif /* PARAM_H_ */
