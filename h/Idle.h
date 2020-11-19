/*
 * Idle.h
 *
 *  Created on: Aug 17, 2019
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include "Thread.h"

class Thread;

class Idle: public Thread {
public:
	Idle();
	virtual void run();
	void start();
	friend class Thread;
};




#endif /* IDLE_H_ */
