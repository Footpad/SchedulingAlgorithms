/*
 * EDFScheduler.cpp
 *
 * Concrete subclass of a Scheduler implementing the
 * Earliest Deadline First scheduling algorithm.
 *
 *  Created on: Mar 13, 2013
 *      Author: dam7633
 */

#include "EDFScheduler.h"

EDFScheduler::EDFScheduler() {
}

EDFScheduler::~EDFScheduler() {
}

void EDFScheduler::scheduleTasks() {
	// Initialize variables for determining the earliest deadline task.
	int earliestIdx = -1;
	long earliestDeadline = LONG_MAX;
	long currentEarliestDeadline;

	// Iterate over all the tasks
	for (std::size_t i = 0; i < taskSet.size(); i++) {
		Task *task = taskSet[i];

		//if this task is running, set it back to ready
		if(task->getPriority() != TP_READY) {
			task->setPriority(TP_READY);
		}

		//look at the deadlines of those tasks which are ready or running
		if(task->getState() == TP_READY || task->getState() == TP_RUNNING) {
			//The time-to-deadline is equal to the deadline - the time elapsed since the last deadline
			currentEarliestDeadline = task->getRemainingDeadline();
			if(currentEarliestDeadline < earliestDeadline) {
				earliestIdx = i;
				earliestDeadline = currentEarliestDeadline;
			}
		}
	}

	//With the earliest deadline known, set that task to running...
	if(earliestIdx != -1) {
		taskSet[earliestIdx]->setPriority(TP_RUNNING);
	}
}
