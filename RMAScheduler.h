/*
 * RMAScheduler.h
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

	void scheduleTasks();

private:
	bool taskPrioritiesSet;
};

#endif /* RMASCHEDULER_H_ */
