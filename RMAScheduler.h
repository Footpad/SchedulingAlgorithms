/*
 * RMAScheduler.h
 *
 * Concrete subclass of a Scheduler implementing the
 * Rate-Monotonic scheduling algorithm.
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#ifndef RMASCHEDULER_H_
#define RMASCHEDULER_H_

#include "Scheduler.h"

class RMAScheduler: public Scheduler {
public:
	RMAScheduler();
	virtual ~RMAScheduler();

	/**
	 * Template method implementing the RMA algorithm.
	 *
	 * This scheduler fully utilizes the QNX fixed-priority
	 * scheduler by assigning descending priorities starting
	 * from TP_SCHEDULER.
	 *
	 * Because of this, RMA can only be run with a task set
	 * smaller in size than TP_SCHEDULER, and will cause an
	 * assertion failure otherwise.
	 */
	void scheduleTasks();

private:
	/**
	 * Flag used to only schedule tasks on the first run,
	 * since RMA is a fixed-priority scheduler.
	 */
	bool taskPrioritiesSet;
};

#endif /* RMASCHEDULER_H_ */
