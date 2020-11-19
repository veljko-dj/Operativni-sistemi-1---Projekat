/*
 * Ivt.h
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#ifndef IVT_H_
#define IVT_H_

class KernelEv;
typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;


class IVTEntry{
public:
	IVTEntry(IVTNo num, pInterrupt  newRout);
	~IVTEntry();

	void callOld();
	void signal();

	static void setKernelEvent(KernelEv* ke, IVTNo n);

	static IVTEntry* IVT[256];
protected:
	friend class KernelEv;
private:
	/* OVO AKO JE SLICNO KAO NECIJI RAD TO JE JER CITAM DIREKTNO
	 * sa ZIZINOG FAJLA !!!
	 */
	pInterrupt oldRout;
	KernelEv* myKerEvent;
	IVTNo entryNum;

	void setBackOld();
};



#endif /* IVT_H_ */
