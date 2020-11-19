/*
 * Event.h
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_
#include "param.h"

typedef unsigned char IVTNo;
class KernelEv;
#include "Ivt.h"

class Event {
public:
	Event (IVTNo num);
	~Event();
	void wait ();

protected:
	void signal();
	friend class KernelEv;

private:
	KernelEv* myImpl;
};




#endif /* EVENT_H_ */
