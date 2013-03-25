/*
 * SCTScheduler.h
 *
 * Concrete subclass of a Scheduler implementing the
 * Shortest Completion Time scheduling algorithm.
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#ifndef SCTSCHEDULER_H_
#define SCTSCHEDULER_H_

#include "Scheduler.h"

class SCTScheduler: public Scheduler {
public:
	SCTScheduler();
	virtual ~SCTScheduler();

	/**
	 * Template method implementing the SCT algorithm.
	 */
	void scheduleTasks();
};

#endif /* SCTSCHEDULER_H_ */
