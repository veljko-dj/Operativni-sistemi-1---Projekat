/*
 * KerSem.cpp
 *
 *  Created on: Aug 21, 2019
 *      Author: OS1
 */
#include "Queue.h"
#include "KerSem.h"
#include "Lock.h"
#include "SemQue.h"
#include "Pcb.h"
#include "Timer.h"
#include "SCHEDULE.H"
#include "IOSTREAM.H"

//SemQueue* KernelSem::semaphores=0;

KernelSem::KernelSem(Semaphore* Sem, int val) {
	this->mySem = Sem;
	this->val = val;
	this->waitQue= new Queue();
	KernelSem::semaphores->addElem(this);
}
KernelSem::~KernelSem() {
		semaphores->deleteThreadBySem(this->mySem);
		/*
		 * STA AKO IMA BLOKIRANIH NITI A POZVAO SAM DESTRKTOR
		 * ZA SADA ME BAS BRIGA
		 * I ako obrise niti one svakako ne bi nikad vise proradile
		 */
		delete waitQue;
		/* AKO NESTO NE RADI PRE NEGO STO OBRISES WAITQUE IZBACI SVE
		 * I ubaci ih u Scheduler
		 */
	}

int KernelSem::wait(Time maxTimeToWait) {
	int timeToWait;
	if (maxTimeToWait>0) timeToWait= (int)maxTimeToWait;
	else timeToWait=-1;
	Lock::lock();
	if (--val < 0) {
		PCB::running->myStatus = BLOCKED;
		int k= PCB::running->myID;
		waitQue->addElem((PCB*)PCB::running,timeToWait);
		//if (PCB::running->myID!=3)cout<<endl<<"__"<<PCB::running->myID<<"__";
		Lock::unlock();
		Timer::dispatch();
		Lock::lock();
		if (this->waitQue->deleteThreadByIDAndReturnTime(PCB::running->myID)==0){
			/* TEK OVDE IZBACUJEM IZ LISTE, IZBACUJE OVA FUNK GORE
			 * a tamo stavim samo u sch, stavim i READY ali ne izbacujem, i svaki put nece
			 * dekrementirati jer stavi if >0 -- i nemas frku, kad dobije red svoj time ce i dalje biti 0
			 * u metodi u na signal izbacujes neke niti sa cekanja, stavi im time na -1 da ne bi mogao
			 * tick kasnije da im smanji na nula pa da greskom ovde vratis 0
			 *  i u QUEUE.H napravi tick metodu da prodje ceo niz,
			 * sve niti i svakom elementu da smanji za jedan
			 * a onda ovde jednu static metodu koja ce da prolazi kroz sve semafore
			 * i za svaki sem da pokrece tu metodu.
			 * */
			val++;

					/*
					 * Lock::lock();
					 * this->waitQue->countById(-1);
				     * Lock::unlock();
					 */
			Lock::unlock();
			return 0;
		}else Lock::unlock();
						/*	Lock::lock();
							this->waitQue->countById(-2);
							Lock::unlock();*/
	} else {
		Lock::unlock();
	}
	return 1;
}

int KernelSem::signal(int n) {
	if (n<0) return n;

	int ret=0;
	if (n == 0) {
		//Ako ovde pukne na slicnost, molim osobu koja pregleda da pogleda prezentaciju
		// 151 jer je bukvalno copy/paste sa prezentacije
		if (val++ < 0) {
				/*cout<<"Pre uzimanja prve slobodne";
				waitQue->countById( -3);*/
				Lock::lock();
				PCB * tmp= this->waitQue->getFirstElem();
				if (tmp){
					/*cout<<"OSLOBADJAM  nit "<< tmp->myID<<endl;
					waitQue->countById( tmp->myID);*/
					tmp->myStatus = READY;
					Scheduler::put(tmp);
				}
				Lock::unlock();
		}
	}else{
		int i=0;
		int n1=n;
		while (n1>0){
			Lock::lock();
			n1--;
						/*Lock::lock();
						cout<<" 	val :: "<<val<<" 	n1:: "<< n1<<endl;
						Lock::unlock();*/
			if (val++<0){
					PCB * tmp= this->waitQue->getFirstElem();
					//if (tmp==0){cout<<" TMP==0 u signal"<<endl; break;}
					if (tmp!=0){
						//	cout<<"signal(150)  nit "<< tmp->myID<<endl;
							tmp->myStatus = READY;
							Scheduler::put(tmp);
							i++;
						}
					/*Lock::lock();
					cout<<" 	I::"<<i<<endl;
					Lock::unlock();*/
			}
			Lock::unlock();
		}
						/*Lock::lock();
						cout<<" 	val :: "<<val<<" 	n1:: "<< n1<<endl;
						Lock::unlock();*/
		ret=i;
	}
	return ret;
}


int KernelSem::value() const{
		return val;
}
