/*
 * Timer.h
 *
 *  Created on: Aug 17, 2019
 *      Author: OS1
 */

#ifndef TIMER_H_
#define TIMER_H_

typedef void interrupt (*pInterrupt)(...);
extern void tick();

class Timer{
public:
	Timer();

	static void inic();
	static void dispatch();
	static void restore();

	static void interrupt timer(...);
	static pInterrupt oldEntry;

	~Timer();
private:

	//static volatile unsigned tsp, tss, tbp;

};

#endif /* TIMER_H_ */
