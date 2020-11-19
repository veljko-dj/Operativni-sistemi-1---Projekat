/*
 * SemQue.h
 *
 *  Created on: Aug 21, 2019
 *      Author: OS1
 */

#ifndef SEMAFOR_SEMQUE_H_
#define SEMAFOR_SEMQUE_H_
class Semaphore;

class KernelSem;
 /* 		Klasa koja ce sluziti samo za listu svih semafora
  * 		i svaki semafor ce u sebi imati listu niti koje cekaju
  * 		klasicna lista
  */
class SemQueue{
public:
	SemQueue();
	~SemQueue();
	void addElem( KernelSem* elem);
	KernelSem* getFirstElem();
	int getLength();
	void deleteThreadBySem(Semaphore* sem);
	void updateSemaphore_tick(); //NISI ODRADIO

private:
	struct Elem{
		KernelSem* elem_sem;
		Elem* next;
		Elem(KernelSem* sem, Elem* sled=0){
			elem_sem=sem;
			next= sled;  	}
	};
	Elem *first, *last;
	int length;
};



#endif /* SEMAFOR_SEMQUE_H_ */
