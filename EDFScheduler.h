/*
 * EDFScheduler.h
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

	void scheduleTasks();
};

#endif /* EDFSCHEDULER_H_ */
