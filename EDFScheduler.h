/*
 * EDFScheduler.h
 *
 * Concrete subclass of a Scheduler implementing the
 * Earliest Deadline First scheduling algorithm.
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#ifndef EDFSCHEDULER_H_
#define EDFSCHEDULER_H_

#include "Scheduler.h"

class EDFScheduler: public Scheduler {
public:
	EDFScheduler();
	virtual ~EDFScheduler();

	/**
	 * Template method implementing the EDF algorithm.
	 */
	void scheduleTasks();
};

#endif /* EDFSCHEDULER_H_ */
