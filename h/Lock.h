/*
 * Lock.h
 *
 *  Created on: Aug 20, 2019
 *      Author: OS1
 */

#ifndef LOCK_H_
#define LOCK_H_

class Lock{
public:
	static int val;
	static int saved_demand;
	static void lock();
	static void unlock();
};



#endif /* LOCK_H_ */
