/*
 * Timer.cpp
 *
 *  Created on: Aug 17, 2019
 *      Author: OS1
 */
#include <DOS.H>
#include <IOSTREAM.H>
#include "SCHEDULE.h"
#include "Idle.h"
#include "param.h"
#include "Pcb.h"
#include "Queue.h"
#include "Thread.h"
#include "Lock.h"
#include "Timer.h"
#include "KerSem.h"
#include "SemQue.h"



extern void tick();
volatile unsigned tsp, tss, tbp;
pInterrupt Timer::oldEntry=0;

void Timer::inic(){
	Lock::lock(); //nepotrebno ali poziva se samo jednom pa nije strasno
						/*
						 * cout<<"		TIMER::INIC"<<endl;
						 */

#ifndef BCC_BLOCK_IGNORE
		Timer::oldEntry = getvect(0x08);
		setvect(0x60,Timer::oldEntry);
		setvect(0x08, Timer::timer);
#endif


		PCB::idle= new Idle();
		PCB::idle->start();
							/*
							 * Lock::lock();
							 * cout<<"Sledeci PCB je za MAIN_THR"<<endl;
							 * Lock::unlock();
							 */
		PCB::main_thr = new PCB(defaultStackSize,defaultTimeSlice,0);
		PCB::running = PCB::main_thr;
		PCB::running->myStatus= RUNNING;
		PCB::threads->addElem((PCB*)PCB::running);
		/*
		 * Nisam lock-ovao jer ima samo jedna nit trenutno
		 */


	Lock::unlock();
}


void Timer::restore(){

	Lock::lock();
					/*
					 * cout<<"		   		TIMER::restore"<<endl;
					 * if (PCB::running != PCB::main_thr)
					 *	cout<<"NESTO NE VALJA TIMER::LOCK"<<endl;
					 */
	#ifndef BCC_BLOCK_IGNORE
		setvect(0x08, Timer::oldEntry);
	#endif
		PCB::threads->deleteThreadByID(PCB::main_thr->myID);
		delete PCB::main_thr;
		delete PCB::threads;
		delete PCB::idle;
		delete KernelSem::semaphores;


	Lock::unlock();
}

 void interrupt Timer::timer(...){
	if (PCB::on_demand==0){
		tick();
							/*
							 * cout<<" 		TIMER::tick()"<<endl;
							 * #ifndef BCC_BLOCK_IGNORE
							 *	 asm {cli;}
						     * #endif
							 */
		if (PCB::running_cnt>0) PCB::running_cnt--;
		#ifndef BCC_BLOCK_IGNORE
				asm int 60h;
		#endif
		KernelSem::semaphores->updateSemaphore_tick(); //ovo ima cout
	}

	if (PCB::running_cnt==0 || PCB::on_demand==1 || Lock::saved_demand==1){
		if (Lock::val<=0){
							 /*
							  * cout<<"Promena kont"<<endl;
							  * #ifndef BCC_BLOCK_IGNORE
							  * 	asm {cli;}
							  *	#endif
							  */

	#ifndef BCC_BLOCK_IGNORE
			asm {
				mov  tsp, sp
				mov tss, ss
				mov tbp, bp
			}
	#endif
			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			if (PCB::running->myStatus !=  BLOCKED &&
					PCB::running->myStatus!=  DONE &&
						PCB::running->myStatus!=  CREATED &&
							PCB::running->myThread!= PCB::idle){
				PCB::running->myStatus=  READY;
				Scheduler::put((PCB*)PCB::running);
								//cout<<" 	Ovu nit stavljam u SCHEDULER"<<endl;
			}
			PCB::running= Scheduler::get();
			PCB::running->myStatus=  RUNNING;//ovo RUNNING ti za sada nista ne treba!
			if	(PCB::running== 0)	{
						/*
						 * cout<<"			Nema spremnih niti"<<endl;
						 * cout<<"\\";
						 * #ifndef BCC_BLOCK_IGNORE
						 * 	asm {cli;}
						 * #endif
						 */
				PCB::running= PCB::idle->myPCB;
			}else {
						/*
						 *  cout<<'/';
						 * #ifndef BCC_BLOCK_IGNORE
						 * asm {cli;}
						 * #endif
						 */
			}
			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			if (PCB::running->myTimeSlice==0) PCB::running_cnt=-1;
			else PCB::running_cnt=PCB::running->myTimeSlice;

		#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
		#endif
			PCB::on_demand= Lock::saved_demand=0;
		} else Lock::saved_demand=1;
	}
 }

 void Timer::dispatch(){
	/*
	 * #ifndef BCC_BLOCK_IGNORE
	 * asm cli
	 * #endif
	 * 		Ovo ti ne treba jer ako ti uskoci neki interr
	 * 		izmedju ova dva reda nema veze, to si hteo
	 * 		Ako stavis ovo, posle ovoga mora sti
	 */
	PCB::on_demand = 1;
	Timer::timer();
	//PCB::on_demand= 0;
}
